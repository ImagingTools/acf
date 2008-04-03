#include "icomp/CCompositeComponentContext.h"


#include "icomp/IRegistry.h"
#include "icomp/IRegistryElement.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CCompositeComponent.h"


namespace icomp
{


CCompositeComponentContext::CCompositeComponentContext(
			const IRegistryElement* elementPtr,
			const IRegistry* registryPtr,
			const IRegistriesManager* registriesManagerPtr,
			const CCompositeComponentContext* parentPtr)
:	BaseClass(elementPtr, parentPtr),
	m_registry(*registryPtr),
	m_registriesManager(*registriesManagerPtr)
{
	I_ASSERT(registryPtr != NULL);

	// TODO: implement CCompositeComponentContext!!!
}


// reimplemented (icomp::IComponentContext)

IComponent* CCompositeComponentContext::GetSubcomponent(const std::string& componentId) const
{
	ComponentMap::const_iterator iter = m_componentMap.find(componentId);
	if (iter != m_componentMap.end()){
		return iter->second.componentPtr.GetPtr();
	}
	else{
		ComponentInfo& componentInfo = m_componentMap[componentId];

		const IRegistry::ElementInfo* elementInfoPtr = m_registry.GetElementInfo(componentId);
		if (elementInfoPtr != NULL && elementInfoPtr->elementPtr.IsValid()){
			const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;

			switch (elementInfoPtr->elementType){
			case IRegistry::ET_COMPONENT:
				{
					const IComponentStaticInfo& elementStaticInfo = registryElement.GetComponentStaticInfo();

					componentInfo.contextPtr.SetPtr(new CComponentContext(&registryElement, this));
					if (componentInfo.contextPtr.IsValid()){
						componentInfo.componentPtr.SetPtr(elementStaticInfo.CreateComponent(componentInfo.contextPtr.GetPtr()));
					}
				}
				break;

			case IRegistry::ET_COMPOSITION:
				{
					const IRegistry* subRegistryPtr = m_registriesManager.GetRegistry(elementInfoPtr->packageId, elementInfoPtr->componentId, &m_registry);
					if (subRegistryPtr != NULL){
						CCompositeComponentContext* contextPtr = new CCompositeComponentContext(
									&registryElement,
									subRegistryPtr,
									&m_registriesManager,
									this);
						if (contextPtr != NULL){
							componentInfo.contextPtr.SetPtr(contextPtr);

							TComponentWrap<CCompositeComponent>* componentPtr = new TComponentWrap<CCompositeComponent>(contextPtr);

							componentInfo.componentPtr.SetPtr(componentPtr);
						}
					}
				}
				break;
			}
		}

		return componentInfo.componentPtr.GetPtr();
	}
}


}//namespace icomp


