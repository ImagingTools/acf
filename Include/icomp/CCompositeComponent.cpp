#include "icomp/CCompositeComponent.h"


#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/CCompositeComponentContext.h"


namespace icomp
{


CCompositeComponent::CCompositeComponent()
:	m_blockCreating(true)
{
}


// reimplemented (icomp::IComponent)

void* CCompositeComponent::GetInterface(const type_info& interfaceType, const std::string& subId)
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		return NULL;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	if (subId.empty()){
		const IRegistry::ExportedInterfacesMap& interfaceInfos = registry.GetExportedInterfacesMap();

		IRegistry::ExportedInterfacesMap::const_iterator iter = interfaceInfos.find(interfaceType.name());
		if (iter != interfaceInfos.end()){
			std::string componentId;
			std::string restId;
			SplitComponentId(iter->second, componentId, restId);

			IComponent* subComponentPtr = GetSubcomponent(componentId);
			if (subComponentPtr != NULL){
				return subComponentPtr->GetInterface(interfaceType, restId);
			}
		}
	}
	else{
		std::string componentId;
		std::string restId;
		SplitComponentId(subId, componentId, restId);

		const IRegistry::ExportedInterfacesMap& subcomponentMap = registry.GetExportedComponentsMap();

		IRegistry::ExportedComponentsMap::const_iterator iter = subcomponentMap.find(componentId);
		if (iter != subcomponentMap.end()){
			const std::string& realComponentId = iter->second;
			IComponent* subComponentPtr = GetSubcomponent(realComponentId);
			if (subComponentPtr != NULL){
				return subComponentPtr->GetInterface(interfaceType, restId);
			}
		}
	}

	return NULL;
}


void CCompositeComponent::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_blockCreating = false;
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
			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, componentInfo.componentPtr, true);

			componentInfo.isInitialized = true;
		}

		return componentInfo.componentPtr.GetPtr();
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

		CreateSubcomponentInfo(componentId, componentInfo.contextPtr, retVal, false);

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


// static methods

void CCompositeComponent::SplitComponentId(const std::string& fullId, std::string& componentId, std::string& restId)
{
	std::string::size_type pointPos = fullId.find('.');
	if (pointPos != std::string::npos){
		componentId = fullId.substr(0, pointPos);
		restId = componentId.substr(pointPos + 1);
	}
	else{
		componentId = fullId;
		restId = "";
	}
}


// protected methods

bool CCompositeComponent::CreateSubcomponentInfo(
			const std::string& componentId,
			ContextPtr& subContextPtr,
			ComponentPtr& subComponentPtr,
			bool isOwned) const
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		return false;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(componentId);
	if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
		const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;
		const IRegistriesManager& registriesManager = contextPtr->GetRegistriesManager();

		// try to create composed component
		const icomp::IRegistry* subRegistryPtr = registriesManager.GetRegistry(elementInfoPtr->address);
		if (subRegistryPtr != NULL){
			if (!subContextPtr.IsValid()){
				subContextPtr.SetPtr(new CCompositeComponentContext(
							&registryElement,
							subRegistryPtr,
							&registriesManager,
							contextPtr));
			}

			if (subContextPtr.IsValid()){
				subComponentPtr.SetPtr(new TComponentWrap<CCompositeComponent>);
				if (subComponentPtr.IsValid()){
					subComponentPtr->SetComponentContext(subContextPtr.GetPtr(), this, isOwned);
				}
			}

			return subComponentPtr.IsValid();
		}
		else{	// create real component
			if (!subContextPtr.IsValid()){
				subContextPtr.SetPtr(new CComponentContext(&registryElement, contextPtr, componentId));
			}

			if (subContextPtr.IsValid()){
				const IComponentStaticInfo& elementStaticInfo = registryElement.GetComponentStaticInfo();

				subComponentPtr.SetPtr(elementStaticInfo.CreateComponent());
				if (subComponentPtr.IsValid()){
					subComponentPtr->SetComponentContext(subContextPtr.GetPtr(), this, isOwned);
				}
			}
		}
	}

	return subComponentPtr.IsValid();
}


}//namespace icomp


