#ifndef icomp_IExtPackagesManager_included
#define icomp_IExtPackagesManager_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QSet>

// ACF includes
#include <icomp/IPackagesManager.h>


namespace icomp
{


class IExtPackagesManager: virtual public IPackagesManager
{
public:
	enum PathType
	{
		PT_ALL,
		/**
		*/
		PT_CONFIG,
		/**
		*/
		PT_PACKAGES_DIR,
		/**
		*/
		PT_PACKAGE,
		/**
		*/
		PT_REGISTRY
	};

	/**
		List of pathes.
	*/
	typedef QSet<QString> PathList;

	/**
		Get list of files used for configuration.
	*/
	virtual PathList GetConfigurationPathList(PathType pathType) const = 0;
};


} // namespace icomp


#endif // !icomp_IExtPackagesManager_included


