#ifndef icmpstr_CPackagesLoaderExtComp_included
#define icmpstr_CPackagesLoaderExtComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QDir>

// ACF includes
#include "icmpstr/IExternalMetaInfoManager.h"
#include "icmpstr/CPackagesLoaderComp.h"


namespace icmpstr
{


class CPackagesLoaderExtComp:
			public icmpstr::CPackagesLoaderComp,
			virtual public IExternalMetaInfoManager
{
public:
	typedef icmpstr::CPackagesLoaderComp BaseClass;

	I_BEGIN_COMPONENT(CPackagesLoaderComp);
		I_REGISTER_INTERFACE(IExternalMetaInfoManager);
	I_END_COMPONENT;

	// reimplemented (iqt::CPackagesLoaderComp)
	virtual bool RegisterPackagesDir(const QString& subDir);

	// reimplemented (icmpstr::IExternalMetaInfoManager)
	virtual QString GetPackageInfoPath(const QByteArray& packageId) const;
	virtual QString GetComponentInfoPath(const icomp::CComponentAddress& address) const;

private:
	typedef QMap<QByteArray, QDir> PackageInfosMap;
	PackageInfosMap m_packageInfosMap;
};


} // namespace icmpstr


#endif // !icmpstr_CPackagesLoaderExtComp_included


