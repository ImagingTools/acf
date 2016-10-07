#ifndef icomp_IPackagesManager_included
#define icomp_IPackagesManager_included


// Qt includes
#include <QtCore/QString>


// ACF includes
#include <istd/IChangeable.h>
#include <icomp/CComponentAddress.h>


namespace icomp
{


class IPackagesManager: virtual public istd::IChangeable
{
public:
	enum PackageType
	{
		/**
			Package doesn't exist.
		*/
		PT_UNDIFINED,
		/**
			Type of this package cannot be found out.
		*/
		PT_UNKNOWN,
		/**
			Real packages are stored as dynamic linked libraries in \c arp files.
		*/
		PT_REAL,
		/**
			Composed packages are stored in folders containing registry files (\c acc files).
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
	/**
		Get registry file path, if it is composed registry.
		\param	address		adress of registry treated as composed component.
		\return				registry object or NULL if specified composite component is not registered.
	*/
	virtual QString GetRegistryPath(const CComponentAddress& address) const = 0;
};


} // namespace icomp


#endif // !icomp_IPackagesManager_included


