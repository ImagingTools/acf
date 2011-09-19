#ifndef icomp_IPackagesManager_included
#define icomp_IPackagesManager_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CString.h"


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
	virtual bool LoadPackages(const istd::CString& configFilePath = istd::CString::GetEmpty()) = 0;

	/**
		Get type of package as defined in \c PackageType.
	*/
	virtual int GetPackageType(const std::string& packageId) const = 0;

	/**
		Get directory path of specified component package.
		\param	packageId	ID of component package.
		\return				package path or empty string if specified package is not registered.
	*/
	virtual istd::CString GetPackagePath(const std::string& packageId) const = 0;
};


}//namespace icomp


#endif // !icomp_IPackagesManager_included


