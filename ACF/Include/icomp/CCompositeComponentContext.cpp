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
	ComponentInfo& componentInfo = m_componentMap[componentId];
	if (!componentInfo.isInitialized){
		CreateSubcomponentInfo(componentId, componentInfo.contextPtr, componentInfo.componentPtr);

		componentInfo.isInitialized = true;
	}

	return componentInfo.componentPtr.GetPtr();
}


IComponent* CCompositeComponentContext::CreateSubcomponent(const std::string& componentId) const
{
	ComponentPtr retVal;

	ComponentInfo& componentInfo = m_componentMap[componentId];

	CreateSubcomponentInfo(componentId, componentInfo.contextPtr, retVal);

	return retVal.PopPtr();
}


// protected methods

bool CCompositeComponentContext::CreateSubcomponentInfo(
			const std::string& componentId,
			ContextPtr& contextPtr,
			ComponentPtr& componentPtr) const
{
	const IRegistry::ElementInfo* elementInfoPtr = m_registry.GetElementInfo(componentId);
	if (elementInfoPtr != NULL && elementInfoPtr->elementPtr.IsValid()){
		const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;

		switch (elementInfoPtr->elementType){
		case IRegistry::ET_COMPONENT:
			{
				const IComponentStaticInfo& elementStaticInfo = registryElement.GetComponentStaticInfo();

				if (!contextPtr.IsValid()){
					contextPtr.SetPtr(new CComponentContext(&registryElement, this));
				}

				if (contextPtr.IsValid()){
					componentPtr.SetPtr(elementStaticInfo.CreateComponent(contextPtr.GetPtr()));
				}
			}
			break;

		case IRegistry::ET_COMPOSITION:
			{
				const IRegistry* subRegistryPtr = m_registriesManager.GetRegistry(elementInfoPtr->packageId, elementInfoPtr->componentId, &m_registry);
				if (subRegistryPtr != NULL){
					if (!contextPtr.IsValid()){
						contextPtr = new CCompositeComponentContext(
									&registryElement,
									subRegistryPtr,
									&m_registriesManager,
									this);
					}

					if (contextPtr.IsValid()){
						CCompositeComponentContext* compositeContextPtr = dynamic_cast<CCompositeComponentContext*>(contextPtr.GetPtr());
						I_ASSERT(compositeContextPtr != NULL);

						componentPtr = new TComponentWrap<CCompositeComponent>(compositeContextPtr);
					}
				}
			}
			break;
		}
	}

	return componentPtr.IsValid();
}


}//namespace icomp


