#ifndef PackagePck_included
#define PackagePck_included


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <icomp/CXpcModel.h>

#include <ipackage/CPackagesLoaderComp.h>
#include <ipackage/CRegistriesManagerComp.h>
#include <ipackage/CRegistryCodeSaverComp.h>


/**
	Component, packages and registries manipulation package.
*/
namespace PackagePck
{


typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<icomp::CRegistry>,
			icomp::IRegistry,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> Registry;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<icomp::CXpcModel>,
			icomp::CXpcModel,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> XpcModel;
typedef ipackage::CPackagesLoaderComp PackagesLoader;
typedef ipackage::CRegistriesManagerComp RegistriesManager;
typedef ipackage::CRegistryCodeSaverComp RegistryCodeSaver;


} // namespace PackagePck


#endif // !PackagePck_included


