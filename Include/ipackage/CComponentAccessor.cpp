#include "ipackage/CComponentAccessor.h"


// ACF includes
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryLoader.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CRegistryElement.h"
#include "ifile/TFileSerializerComp.h"
#include "ifile/CSimpleXmlFileReadArchive.h"
#include "ifile/CSimpleXmlFileWriteArchive.h"
#include "ifile/CComposedFilePersistenceComp.h"
#include "ifile/CCompactXmlFileReadArchive.h"
#include "ifile/CCompactXmlFileWriteArchive.h"
#include "ipackage/CPackagesLoaderComp.h"


namespace
{

	
struct Loader
{
	icomp::TSimComponentWrap< ifile::TFileSerializerComp<ifile::CSimpleXmlFileReadArchive, ifile::CSimpleXmlFileWriteArchive> > oldRegistrySerializerComp;
	icomp::TSimComponentWrap< ifile::TFileSerializerComp<ifile::CCompactXmlFileReadArchive, ifile::CCompactXmlFileWriteArchive> > registrySerializerComp;
	icomp::TSimComponentWrap<ifile::CComposedFilePersistenceComp> composedSerializerComp;
	icomp::TSimComponentWrap<ipackage::CPackagesLoaderComp> packagesLoaderComp;

	Loader()
	{
		oldRegistrySerializerComp.InsertMultiAttr("FileExtensions", QString("arx"));
		oldRegistrySerializerComp.InitComponent();

		registrySerializerComp.InsertMultiAttr("FileExtensions", QString("acc"));
		registrySerializerComp.InitComponent();

		composedSerializerComp.InsertMultiRef("SlaveLoaders", &oldRegistrySerializerComp);
		composedSerializerComp.InsertMultiRef("SlaveLoaders", &registrySerializerComp);
		composedSerializerComp.InitComponent();

		packagesLoaderComp.SetRef("RegistryLoader", &composedSerializerComp);
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

	QString usedRegistryFile = registryFile.isEmpty()? QString("default.acc"): registryFile;
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


