#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "CmpstrPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
		Q_INIT_RESOURCE(icmpstr);
	}

} instance;


namespace CmpstrPck
{

	
using namespace icomp;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"Cmpstr",
			"Compositor package",
			IM_PROJECT("ACF") IM_TAG("Compositor Qt") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			AttributeEditor,
			"GUI used to edit attributes",
			"Observer Attributes Qt" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			PackageOverview,
			"GUI showing all packages",
			"Observer Packages" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			RegistryLoader,
			"Loader for registries including component layout in *.alx files",
			"Registry Loader Serializer Layout Components" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			RegistryPreview,
			"Preview of registry object in separated application context",
			"Registry Preview Application" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ComponentHelpFileProvider,
			"Provide file path of html help for component using its address",
			"Help File Provider Component Address" IM_TAG("Help"));

I_EXPORT_COMPONENT(
			VisualRegistry,
			"Model of ACF registry with additional visual elements designed for Compositor", 
			"Components ARX Registry Visual" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			VisualRegistryScenographer,
			"Visual registry editor",
			"Components Registry Visual Observer" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			RegistryConsistInfo,
			"Provides additional information about registries, its elements and attributes",
			"Registry Element Attribute Consistency Check");

I_EXPORT_COMPONENT(
			RegistryPropEditor,
			"Allows to edit global registry properties",
			"Registry Description Keywords" IM_TAG("GUI Editor"));


} // namespace CmpstrPck


