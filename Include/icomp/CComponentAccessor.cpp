#include "icomp/CComponentAccessor.h"


#include "istd/CStaticServicesProvider.h"

#include "icomp/IRegistryLoaderProvider.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryLoader.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CRegistryElement.h"


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
		IComponentEnvironmentManager* managerPtr = registryLoaderProviderPtr->GetEnvironmentManager();

		if (managerPtr != NULL){
			managerPtr->LoadPackages(configFile);

			const icomp::IComponentStaticInfo* staticInfoPtr = dynamic_cast<const icomp::IComponentStaticInfo*>(managerPtr);
			const icomp::IRegistryLoader* registryLoaderPtr = registryLoaderProviderPtr->GetRegistryLoader();
			if ((registryLoaderPtr != NULL) && (staticInfoPtr != NULL)){
				const icomp::IRegistry* registryPtr = registryLoaderPtr->GetRegistryFromFile(m_registryFile.c_str());
				if (registryPtr != NULL){
					static icomp::CRegistryElement dummyElement;

					m_composite.BeginAutoInitBlock();
					m_isAutoInitBlocked = true;

					static icomp::CCompositeComponentContext compositeContext(
								&dummyElement,
								staticInfoPtr,
								registryPtr,
								managerPtr,
								NULL);
					m_composite.SetComponentContext(&compositeContext, NULL, false);
				}
			}
		}
	}
}


} // namespace icomp


