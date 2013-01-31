#include "icmpstr/CComponentAccessor.h"


// ACF includes
#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryLoader.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CRegistryElement.h"
#include "ibase/TFileSerializerComp.h"
#include "icmpstr/CPackagesLoaderComp.h"


namespace
{

	
struct Loader
{
	icomp::TSimComponentWrap< ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> > registryLoaderComp;
	icomp::TSimComponentWrap<icmpstr::CPackagesLoaderComp> packagesLoaderComp;

	Loader()
	{
		registryLoaderComp.InitComponent();

		packagesLoaderComp.SetRef("RegistryLoader", &registryLoaderComp);
		packagesLoaderComp.InitComponent();
	}
};


}


namespace icmpstr
{


CComponentAccessor::CComponentAccessor(
			const QString& registryFile,
			const QString& configFile)
:	m_isAutoInitBlocked(false)
{
	static Loader loader;
	loader.packagesLoaderComp.LoadPackages(configFile);

	QString usedRegistryFile = registryFile.isEmpty()? QString("default.arx"): registryFile;
	const icomp::IRegistry* registryPtr = loader.packagesLoaderComp.GetRegistryFromFile(usedRegistryFile);
	if (registryPtr != NULL){
		static icomp::CRegistryElement dummyElement;

		m_isAutoInitBlocked = true;

		m_mainComponentStaticInfoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(
					*registryPtr,
					loader.packagesLoaderComp,
					NULL));

		m_mainComponentContextPtr.SetPtr(new icomp::CCompositeComponentContext(
					&dummyElement,
					m_mainComponentStaticInfoPtr.GetPtr(),
					registryPtr,
					&loader.packagesLoaderComp,
					NULL,
					""));
		m_mainComponent.SetComponentContext(m_mainComponentContextPtr.GetPtr(), NULL, false);
	}
}


} // namespace icmpstr


