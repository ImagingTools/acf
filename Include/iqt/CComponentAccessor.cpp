#include "iqt/CComponentAccessor.h"


#include "istd/CStaticServicesProvider.h"

#include "icomp/IRegistryLoaderProvider.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryLoader.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CRegistryElement.h"

#include "iqt/CDefaultRegistryLoaderProvider.h"


namespace iqt
{


CComponentAccessor::CComponentAccessor(
			const QString& registryFile,
			const QString& configFile)
:	m_registryFile(registryFile),
	m_isAutoInitBlocked(false)
{
	if (m_registryFile.isEmpty()){
		m_registryFile = "default.arx";
	}

	static iqt::CDefaultRegistryLoaderProvider registryProvider;

	icomp::IComponentEnvironmentManager* managerPtr = registryProvider.GetEnvironmentManager();
	if (managerPtr != NULL){
		managerPtr->LoadPackages(configFile);

		const icomp::IComponentStaticInfo* staticInfoPtr = dynamic_cast<const icomp::IComponentStaticInfo*>(managerPtr);
		const icomp::IRegistryLoader* registryLoaderPtr = registryProvider.GetRegistryLoader();
		if ((registryLoaderPtr != NULL) && (staticInfoPtr != NULL)){
			const icomp::IRegistry* registryPtr = registryLoaderPtr->GetRegistryFromFile(m_registryFile);
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


} // namespace iqt


