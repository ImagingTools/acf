#include "icomp/CCompositeComponent.h"


#include "istd/CClassInfo.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CCompositeComponentContext.h"


namespace icomp
{


CCompositeComponent::CCompositeComponent()
:	m_blockCreating(true),
	m_isAutoInitBlockCount(0)
{
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
			iter = interfaceInfos.find(interfaceType);
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


void CCompositeComponent::SetComponentContext(
			const icomp::IComponentContext* contextPtr,
			const IComponent* parentPtr,
			bool isParentOwner)
{
	BaseClass::SetComponentContext(contextPtr, parentPtr, isParentOwner);

	m_autoInitComponentIds.clear();

	const CCompositeComponentContext* compositeContextPtr = dynamic_cast<const CCompositeComponentContext*>(contextPtr);
	if (compositeContextPtr != NULL){
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
	}
}


IComponent* CCompositeComponent::GetSubcomponent(const std::string& componentId) const
{
	if (m_blockCreating){
		ComponentMap::const_iterator iter = m_componentMap.find(componentId);
		if (iter != m_componentMap.end()){
			return iter->second.componentPtr.GetPtr();
		}
		else{
			return NULL;
		}
	}
	else{
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isInitialized){
			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &componentInfo.componentPtr, true);

			componentInfo.isInitialized = true;
		}

		return componentInfo.componentPtr.GetPtr();
	}
}


const IComponentContext* CCompositeComponent::GetSubcomponentContext(const std::string& componentId) const
{
	if (m_blockCreating){
		ComponentMap::const_iterator iter = m_componentMap.find(componentId);
		if (iter != m_componentMap.end()){
			return iter->second.contextPtr.GetPtr();
		}
		else{
			return NULL;
		}
	}
	else{
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isInitialized){
			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, NULL, true);
		}

		return componentInfo.contextPtr.GetPtr();
	}
}


IComponent* CCompositeComponent::CreateSubcomponent(const std::string& componentId) const
{
	if (m_blockCreating){
		return NULL;
	}
	else{
		ComponentPtr retVal;

		ComponentInfo& componentInfo = m_componentMap[componentId];

		CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &retVal, false);

		return retVal.PopPtr();
	}
}


void CCompositeComponent::OnSubcomponentDeleted(const IComponent* subcomponentPtr)
{
	for (		ComponentMap::iterator iter = m_componentMap.begin();
				iter != m_componentMap.end();
				++iter){
		ComponentInfo& info = iter->second;
		if (info.componentPtr.IsValid()){
			info.componentPtr->SetComponentContext(NULL, NULL, false);

			info.isInitialized = false;

			if (info.componentPtr == subcomponentPtr){
				info.componentPtr.PopPtr();
			}
		}
	}

	m_componentMap.clear();

	delete this;
}


// protected methods

void CCompositeComponent::CreateSubcomponentInfo(
			const std::string& componentId,
			ContextPtr& subContextPtr,
			ComponentPtr* subComponentPtr,
			bool isOwned) const
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		return;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(componentId);
	if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
		const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;

		const IComponentEnvironmentManager& envManager = contextPtr->GetEnvironmentManager();
		const IComponentStaticInfo* componentInfoPtr = envManager.GetComponentMetaInfo(elementInfoPtr->address);
		if (componentInfoPtr == NULL){
			return;
		}

		if (!subContextPtr.IsValid()){
			int componentType = componentInfoPtr->GetComponentType();
			if (componentType == IComponentStaticInfo::CT_COMPOSITE){
				// create composed component
				const icomp::IRegistry* subRegistryPtr = envManager.GetRegistry(elementInfoPtr->address);
				if (subRegistryPtr != NULL){
					subContextPtr.SetPtr(new CCompositeComponentContext(
								&registryElement,
								componentInfoPtr,
								subRegistryPtr,
								&envManager,
								contextPtr));
				}
			}
			else if (componentType == IComponentStaticInfo::CT_REAL){
				// create real component
				subContextPtr.SetPtr(new CComponentContext(
							&registryElement,
							componentInfoPtr,
							contextPtr,
							componentId));
			}
		}

		if (subContextPtr.IsValid() && (subComponentPtr != NULL)){
			subComponentPtr->SetPtr(componentInfoPtr->CreateComponent());
			if (subComponentPtr->IsValid()){
				(*subComponentPtr)->SetComponentContext(subContextPtr.GetPtr(), this, isOwned);
			}
		}
	}
}


bool CCompositeComponent::EnsureAutoInitComponentsCreated() const
{
	while ((m_isAutoInitBlockCount <= 0) && !m_autoInitComponentIds.empty()){
		std::string autoInitId = *m_autoInitComponentIds.begin();

		m_autoInitComponentIds.erase(m_autoInitComponentIds.begin());

		ComponentInfo& autoInitInfo = m_componentMap[autoInitId];
		if (!autoInitInfo.isInitialized){
			CreateSubcomponentInfo(autoInitId, autoInitInfo.contextPtr, &autoInitInfo.componentPtr, true);

			autoInitInfo.isInitialized = true;
		}
	}

	return m_autoInitComponentIds.empty();
}


// reimplemented (icomp::IComponent)

void CCompositeComponent::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_blockCreating = false;

	EnsureAutoInitComponentsCreated();
}


void CCompositeComponent::OnComponentDestroyed()
{
	m_blockCreating = true;

	for (		ComponentMap::iterator iter = m_componentMap.begin();
				iter != m_componentMap.end();
				++iter){
		ComponentInfo& info = iter->second;
		if (info.componentPtr.IsValid()){
			info.componentPtr->SetComponentContext(NULL, NULL, false);

			info.isInitialized = false;
		}
	}

	BaseClass::OnComponentDestroyed();
}


}//namespace icomp


