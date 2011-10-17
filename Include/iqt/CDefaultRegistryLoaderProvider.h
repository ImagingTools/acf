#ifndef iqt_CDefaultRegistryLoaderProvider_included
#define iqt_CDefaultRegistryLoaderProvider_included


#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "ibase/TFileSerializerComp.h"

#include "icomp/IRegistryLoaderProvider.h"
#include "icomp/TSimComponentWrap.h"

#include "iqt/CPackagesLoaderComp.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CDefaultRegistryLoaderProvider: virtual public icomp::IRegistryLoaderProvider
{
public:
	CDefaultRegistryLoaderProvider();

	// reimplemented (icomp::IRegistryLoaderProvider)
	virtual icomp::IComponentEnvironmentManager* GetEnvironmentManager() const;
	virtual icomp::IRegistryLoader* GetRegistryLoader() const;
	
private:
	typedef ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
	
	icomp::TSimComponentWrap<XmlFileSerializer> m_registryLoaderComp;
	mutable icomp::TSimComponentWrap<iqt::CPackagesLoaderComp> m_packagesLoaderComp;
};


// public inline methods

inline icomp::IComponentEnvironmentManager* CDefaultRegistryLoaderProvider::GetEnvironmentManager() const
{
	return &m_packagesLoaderComp;
}


inline icomp::IRegistryLoader* CDefaultRegistryLoaderProvider::GetRegistryLoader() const
{
	return &m_packagesLoaderComp;
}


} // namespace iqt


#endif // !iqt_CDefaultRegistryLoaderProvider_included


