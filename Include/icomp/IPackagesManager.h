#ifndef icomp_IPackagesManager_included
#define icomp_IPackagesManager_included


// Qt includes
#include <QtCore/QString>


// ACF includes
#include "istd/IChangeable.h"


namespace icomp
{


class IPackagesManager: virtual public istd::IChangeable
{
public:
	enum PackageType
	{
		PT_UNKNOWN,
		/**
			Real packages are stored as dynamic linked libraries in \c arp files.
		*/
		PT_REAL,
		/**
			Composed packages are stored in folders containing registry files (\c arx files).
		*/
		PT_COMPOSED
	};

	/**
		Configure environment for specified file path.
		If this file path is invalid or empty, standard configuration file will be used.
	*/
	virtual bool LoadPackages(const QString& configFilePath = QString()) = 0;

	/**
		Get type of package as defined in \c PackageType.
	*/
	virtual int GetPackageType(const QByteArray& packageId) const = 0;

	/**
		Get directory path of specified component package.
		\param	packageId	ID of component package.
		\return				package path or empty string if specified package is not registered.
	*/
	virtual QString GetPackagePath(const QByteArray& packageId) const = 0;
};


} // namespace icomp


#endif // !icomp_IPackagesManager_included


