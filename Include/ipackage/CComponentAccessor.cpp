#include "ipackage/CComponentAccessor.h"


// ACF includes
#include "ifile/CXmlFileReadArchive.h"
#include "ifile/CXmlFileWriteArchive.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryLoader.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CRegistryElement.h"
#include "ifile/TFileSerializerComp.h"
#include "ipackage/CPackagesLoaderComp.h"


namespace
{

	
struct Loader
{
	icomp::TSimComponentWrap< ifile::TFileSerializerComp<ifile::CXmlFileReadArchive, ifile::CXmlFileWriteArchive> > registryLoaderComp;
	icomp::TSimComponentWrap<ipackage::CPackagesLoaderComp> packagesLoaderComp;

	Loader()
	{
		registryLoaderComp.InsertMultiAttr("FileExtensions", QString("arx"));
		registryLoaderComp.InitComponent();

		packagesLoaderComp.SetRef("RegistryLoader", &registryLoaderComp);
		packagesLoaderComp.InitComponent();
	}
};


}


namespace ipackage
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


} // namespace ipackage


