#include "CompositorPck.h"


#include "icomp/export.h"


I_REGISTER_QT_RESOURCE(iqtgui);
I_REGISTER_QT_RESOURCE(icmpstr);


namespace CompositorPck
{


I_EXPORT_PACKAGE(
			"Acf/Compositor",
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

I_EXPORT_COMPONENT(
			QuickHelpGui,
			"Show short description and allow to show technical documentation",
			"Quick Help Technical Documentation" IM_TAG("GUI Help"));

I_EXPORT_COMPONENT(
			PackagesLoaderExt,
			"Load, cache packages from files and supports additional meta information needed for compositor",
			"Component Registry Cache Package Load File Configuration Components" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			ComponentNoteEditor,
			"Editor for user notes in the component diagram",
			"Help Technical Documentation Note Component" IM_TAG("GUI Help"));

I_EXPORT_COMPONENT(
			Registry,
			"Model of ACF registry (ARX)",
			"Registry Components Data Connection Diagram ARP ARX" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			XpcModel,
			"Model of ACF configuration (XPC)",
			"Registry Components Configuration Packages ARP ARX XPC Data Connection" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			XpcEditorGui,
			"Display a GUI for editing XPC configurations",
			IM_TAG("XPC GUI"));



} // namespace CompositorPck


