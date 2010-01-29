#include "icomp/CComponentAccessor.h"

#include "istd/CStaticServicesProvider.h"

#include "icomp/IRegistryLoaderProvider.h"


namespace icomp
{


CComponentAccessor::CComponentAccessor(
			const istd::CString& registryFile,
			const istd::CString& configFile)
:	m_registryFile(registryFile),
	m_isAutoInitBlocked(false)
{
	if (m_registryFile.IsEmpty()){
		m_registryFile = "default.arx";
	}

	icomp::IRegistryLoaderProvider* registryLoaderProviderPtr = istd::GetService<icomp::IRegistryLoaderProvider>();
	if (registryLoaderProviderPtr != NULL){
		icomp::IRegistryLoader* registryLoaderPtr = registryLoaderProviderPtr->GetRegistryLoader();
	
		if (registryLoaderPtr != NULL){
			registryLoaderPtr->ConfigureEnvironment(configFile);

			const icomp::IComponentStaticInfo* staticInfoPtr = dynamic_cast<const icomp::IComponentStaticInfo*>(registryLoaderPtr);
			const IRegistriesManager* registriesManagerPtr = dynamic_cast<const icomp::IRegistriesManager*>(registryLoaderPtr);
			if ((registriesManagerPtr != NULL) && (staticInfoPtr != NULL)){
				const icomp::IRegistry* registryPtr = registryLoaderPtr->GetRegistryFromFile(m_registryFile.c_str());
				if (registryPtr != NULL){
					static icomp::CRegistryElement dummyElement;
					dummyElement.Initialize(staticInfoPtr);


					m_composite.BeginAutoInitBlock();
					m_isAutoInitBlocked = true;

					static icomp::CCompositeComponentContext compositeContext(&dummyElement, staticInfoPtr, registryPtr, registriesManagerPtr, NULL);
					m_composite.SetComponentContext(&compositeContext, NULL, false);
				}
			}
		}
	}

}


} // namespace icomp


