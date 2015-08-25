#include "PackagePck.h"


#include "icomp/export.h"


namespace PackagePck
{


I_EXPORT_PACKAGE(
			"Acf/PackagePck",
			"Compositor package",
			IM_PROJECT("ACF") IM_TAG("Compositor Qt") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			Registry,
			"Model of ACF registry (ARX)",
			"Registry Components Data Connection Diagram ARP ARX" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			XpcModel,
			"Model of ACF configuration (XPC)",
			"Registry Components Configuration Packages ARP ARX XPC AWC Data Connection" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			PackagesLoader,
			"Load and cache packages from files",
			"Component Registry Cache Package Load File Configuration Components" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			RegistriesManager,
			"Load and cache registries from files",
			"Registry Manager Load File Configuration Components");

I_EXPORT_COMPONENT(
			RegistryCodeSaver,
			"Stores registry as C++ code",
			"Code C++ File Store Save Registry Compile" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Loader"));


} // namespace PackagePck


