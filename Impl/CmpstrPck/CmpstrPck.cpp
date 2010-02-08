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


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Cmpstr", "Compositor package", "Qt Compositor Tool");

I_EXPORT_COMPONENT(AttributeEditor, "GUI used to edit attributes", "Observer Editor Attributes Qt GUI");
I_EXPORT_COMPONENT(PackageOverview, "GUI showing all packages", "Observer Packages Qt GUI");
I_EXPORT_COMPONENT(RegistryLoader, "Loader for registries including component layout in *.alx files", "Registry Loader Serializer Layout Components");
I_EXPORT_COMPONENT(RegistryModel, "Model reprentation of registry with additinal layout information stored separated to model", "Registry Model Layout");
I_EXPORT_COMPONENT(RegistryPreview, "Preview of registry object in separated application context", "Registry Preview ACF Application");
I_EXPORT_COMPONENT(RegistryView, "Registry editor using graphical component diagram", "Registry Editor Observer Diagram Components Qt GUI");
I_EXPORT_COMPONENT(ComponentHelpFileProvider, "Provide file path of html help for component using its address", "Help File Provider Component Address Qt");
I_EXPORT_COMPONENT(VisualRegistry, "Model of ACF registry with additional visual elements designed for Compositor", "Components ARX Model ACF Registry Visual Compositor");
I_EXPORT_COMPONENT(VisualRegistryScenographer, "Visual registry editor", "Components ACF Registry Visual Compositor Observer Qt GUI");
I_EXPORT_COMPONENT(RegistryConsistInfo, "Provides additional information about registries, its elements and attributes", "Registry Element Attribute Consistency Check Qt Compositor");
I_EXPORT_COMPONENT(RegistryPropEditor, "Allows to edit global registry properties", "Registry Editor Qt GUI Compositor Description Keywords");


} // namespace CmpstrPck


