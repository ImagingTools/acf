#include "icomp/CCompositeComponent.h"


#include "istd/CClassInfo.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


CCompositeComponent::CCompositeComponent()
:	m_isAutoInitBlockCount(0),
	m_contextPtr(NULL),
	m_parentPtr(NULL),
	m_isParentOwner(false)
{
}


CCompositeComponent::~CCompositeComponent()
{
	if (m_isParentOwner && (m_parentPtr != NULL)){
		const_cast<IComponent*>(m_parentPtr)->OnSubcomponentDeleted(this);
	}

	SetComponentContext(NULL, NULL, false);
}


void CCompositeComponent::BeginAutoInitBlock()
{
	++m_isAutoInitBlockCount;
}


bool CCompositeComponent::EndAutoInitBlock()
{
	I_ASSERT(m_isAutoInitBlockCount > 0);
	if (--m_isAutoInitBlockCount <= 0){
		return EnsureAutoInitComponentsCreated();
	}

	return false;
}


// reimplemented (icomp::IComponent)

const IComponent* CCompositeComponent::GetParentComponent(bool ownerOnly) const
{
	if (!ownerOnly || m_isParentOwner){
		return m_parentPtr;
	}

	return NULL;
}


void* CCompositeComponent::GetInterface(const istd::CClassInfo& interfaceType, const std::string& subId)
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		return NULL;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	if (subId.empty()){
		const IRegistry::ExportedInterfacesMap& interfaceInfos = registry.GetExportedInterfacesMap();

		IRegistry::ExportedInterfacesMap::const_iterator iter;
		if (interfaceType.IsVoid()){
			iter = interfaceInfos.begin();
		}
		else{
			iter = interfaceInfos.find(interfaceType.GetName());
		}

		if (iter != interfaceInfos.end()){
			std::string componentId;
			std::string restId;
			SplitId(iter->second, componentId, restId);

			IComponent* subComponentPtr = GetSubcomponent(componentId);
			if (subComponentPtr != NULL){
				return subComponentPtr->GetInterface(interfaceType, restId);
			}
		}
	}
	else{
		std::string componentId;
		std::string restId;
		SplitId(subId, componentId, restId);

		const IRegistry::ExportedComponentsMap& subcomponentMap = registry.GetExportedComponentsMap();

		IRegistry::ExportedComponentsMap::const_iterator iter = subcomponentMap.find(componentId);
		if (iter != subcomponentMap.end()){
			const std::string& realComponentId = iter->second;
			std::string subComponentId;
			std::string subRestId;
			SplitId(realComponentId, subComponentId, subRestId);

			IComponent* subComponentPtr = GetSubcomponent(subComponentId);
			if (subComponentPtr != NULL){
				return subComponentPtr->GetInterface(interfaceType, JoinId(subRestId, restId));
			}
		}
	}

	return NULL;
}


const IComponentContext* CCompositeComponent::GetComponentContext() const
{
	return m_contextPtr;
}


void CCompositeComponent::SetComponentContext(
			const icomp::IComponentContext* contextPtr,
			const IComponent* parentPtr,
			bool isParentOwner)
{
	m_parentPtr = parentPtr;
	m_isParentOwner = isParentOwner;

	m_autoInitComponentIds.clear();

	const CCompositeComponentContext* compositeContextPtr = dynamic_cast<const CCompositeComponentContext*>(contextPtr);
	if (compositeContextPtr != NULL){
		m_contextPtr = contextPtr;

		const IRegistry& registry = compositeContextPtr->GetRegistry();

		IRegistry::Ids elementIds = registry.GetElementIds();
		for (		IRegistry::Ids::const_iterator iter = elementIds.begin();
					iter != elementIds.end();
					++iter){
			const std::string& elementId = *iter;
			const IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementId);
			I_ASSERT(infoPtr);	// ID must be valid, becouse it was taken using icomp::IRegistry::GetElementIds()!

			I_DWORD flags = infoPtr->elementPtr->GetElementFlags();
			if ((flags & IRegistryElement::EF_AUTO_INSTANCE) != 0){
				m_autoInitComponentIds.insert(elementId);
			}
		}

		EnsureAutoInitComponentsCreated();
	}
	else{
		m_contextPtr = NULL;

		for (		ComponentMap::iterator iter = m_componentMap.begin();
					iter != m_componentMap.end();
					++iter){
			ComponentInfo& info = iter->second;

			if (info.isInitialized){
				if (info.componentPtr.IsValid()){
					info.componentPtr->SetComponentContext(NULL, NULL, false);
				}

				info.isInitialized = false;
			}
		}
	}
}


IComponent* CCompositeComponent::GetSubcomponent(const std::string& componentId) const
{
	if (m_contextPtr != NULL){
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isInitialized){
			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &componentInfo.componentPtr, true);

			componentInfo.isInitialized = true;
		}

		return componentInfo.componentPtr.GetPtr();
	}
	else{
		ComponentMap::const_iterator iter = m_componentMap.find(componentId);
		if (iter != m_componentMap.end()){
			return iter->second.componentPtr.GetPtr();
		}
		else{
			return NULL;
		}
	}
}


const IComponentContext* CCompositeComponent::GetSubcomponentContext(const std::string& componentId) const
{
	if (m_contextPtr != NULL){
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isInitialized){
			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, NULL, true);
		}

		return componentInfo.contextPtr.GetPtr();
	}
	else{
		ComponentMap::const_iterator iter = m_componentMap.find(componentId);
		if (iter != m_componentMap.end()){
			return iter->second.contextPtr.GetPtr();
		}
		else{
			return NULL;
		}
	}
}


IComponent* CCompositeComponent::CreateSubcomponent(const std::string& componentId) const
{
	if (m_contextPtr != NULL){
		ComponentPtr retVal;

		ComponentInfo& componentInfo = m_componentMap[componentId];

		CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &retVal, false);

		return retVal.PopPtr();
	}

	return NULL;
}


void CCompositeComponent::OnSubcomponentDeleted(const IComponent* subcomponentPtr)
{
	I_ASSERT(subcomponentPtr != NULL);

	for (		ComponentMap::iterator iter = m_componentMap.begin();
				iter != m_componentMap.end();
				++iter){
		ComponentInfo& info = iter->second;
		if (info.componentPtr == subcomponentPtr){
			info.componentPtr->SetComponentContext(NULL, NULL, false);

			info.componentPtr.PopPtr();

			info.isInitialized = false;

			delete this;

			return;
		}
	}

	I_CRITICAL();	// subcomponent not found in this composition
}


// protected methods

bool CCompositeComponent::CreateSubcomponentInfo(
			const std::string& componentId,
			ContextPtr& subContextPtr,
			ComponentPtr* subComponentPtr,
			bool isOwned) const
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		return false;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(componentId);
	if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
		const CComponentAddress& elementAddress = elementInfoPtr->address;
		const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;

		const IComponentEnvironmentManager& envManager = contextPtr->GetEnvironmentManager();

		const IComponentStaticInfo* subComponentInfoPtr = NULL;
		if (!elementAddress.GetPackageId().empty()){
			subComponentInfoPtr = envManager.GetComponentMetaInfo(elementAddress);
		}
		else{
			const IComponentStaticInfo& info = contextPtr->GetStaticInfo();
			subComponentInfoPtr = info.GetEmbeddedComponentInfo(elementAddress.GetComponentId());
		}

		if (subComponentInfoPtr == NULL){
			return false;
		}

		if (!subContextPtr.IsValid()){
			int componentType = subComponentInfoPtr->GetComponentType();
			if (componentType == IComponentStaticInfo::CT_COMPOSITE){
				// create composed component
				const icomp::IRegistry* subRegistryPtr = envManager.GetRegistry(elementAddress, &registry);
				if (subRegistryPtr != NULL){
					subContextPtr.SetPtr(new CCompositeComponentContext(
								&registryElement,
								subComponentInfoPtr,
								subRegistryPtr,
								&envManager,
								contextPtr));
				}
			}
			else if (componentType == IComponentStaticInfo::CT_REAL){
				// create real component
				subContextPtr.SetPtr(new CComponentContext(
							&registryElement,
							subComponentInfoPtr,
							contextPtr,
							componentId));
			}
		}

		if (!subContextPtr.IsValid()){
			return false;
		}

		if (subComponentPtr != NULL){
			const IRealComponentStaticInfo* realComponentInfoPtr = dynamic_cast<const IRealComponentStaticInfo*>(subComponentInfoPtr);
			if (realComponentInfoPtr == NULL){
				return false;
			}

			subComponentPtr->SetPtr(realComponentInfoPtr->CreateComponent());
			if (!subComponentPtr->IsValid()){
				return false;
			}

			(*subComponentPtr)->SetComponentContext(subContextPtr.GetPtr(), this, isOwned);
		}
	}

	return true;
}


bool CCompositeComponent::EnsureAutoInitComponentsCreated() const
{
	if (m_contextPtr != NULL){
		while ((m_isAutoInitBlockCount <= 0) && !m_autoInitComponentIds.empty()){
			std::string autoInitId = *m_autoInitComponentIds.begin();

			m_autoInitComponentIds.erase(m_autoInitComponentIds.begin());

			ComponentInfo& autoInitInfo = m_componentMap[autoInitId];
			if (!autoInitInfo.isInitialized){
				CreateSubcomponentInfo(autoInitId, autoInitInfo.contextPtr, &autoInitInfo.componentPtr, true);

				autoInitInfo.isInitialized = true;
			}
		}
	}

	return m_autoInitComponentIds.empty();
}


}//namespace icomp


