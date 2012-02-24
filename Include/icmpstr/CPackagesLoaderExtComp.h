#ifndef icmpstr_CPackagesLoaderExtComp_included
#define icmpstr_CPackagesLoaderExtComp_included


// STL includes
#include <map>

// Qt includes
#include <QDir>

#include "iqt/CPackagesLoaderComp.h"

#include "icmpstr/IExternalMetaInfoManager.h"


namespace icmpstr
{


class CPackagesLoaderExtComp:
			public iqt::CPackagesLoaderComp,
			virtual public IExternalMetaInfoManager
{
public:
	typedef iqt::CPackagesLoaderComp BaseClass;

	I_BEGIN_COMPONENT(CPackagesLoaderComp);
		I_REGISTER_INTERFACE(IExternalMetaInfoManager);
	I_END_COMPONENT;

	// reimplemented (iqt::CPackagesLoaderComp)
	virtual bool RegisterPackagesDir(const QString& subDir);

	// reimplemented (icmpstr::IExternalMetaInfoManager)
	virtual QString GetPackageInfoPath(const std::string& packageId) const;
	virtual QString GetComponentInfoPath(const icomp::CComponentAddress& address) const;

private:
	typedef std::map<std::string, QDir> PackageInfosMap;
	PackageInfosMap m_packageInfosMap;
};


} // namespace icmpstr


#endif // !icmpstr_CPackagesLoaderExtComp_included


