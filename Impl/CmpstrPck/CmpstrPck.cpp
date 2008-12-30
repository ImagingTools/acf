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


} // namespace CmpstrPck


