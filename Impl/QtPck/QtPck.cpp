#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Acf/Qt/Base", "Standard Qt package", "Qt Standard");

I_EXPORT_COMPONENT(PackagesLoader, "Load and cache packages from files", "Component Registry Cache Package Load File Configuration Components");
I_EXPORT_COMPONENT(RegistriesManager, "Load and cache registries from files", "Registry Manager Load File Configuration Components");
I_EXPORT_COMPONENT(Bitmap, "QImage based bitmap implementation", "QImage Image Bitmap Qt");
I_EXPORT_COMPONENT(BitmapSurface, "QImage based bitmap implementation with surface extension", "Sampled Function QImage Image Bitmap Qt Surface");
I_EXPORT_COMPONENT(BitmapLoader, "Loader loading bitmaps using Qt standard implementations", "Image Bitmap Qt File Loader");
I_EXPORT_COMPONENT(SettingsSerializer, "Load and save objects using QSettings as storage", "Qt QSettings Registry Serializer Loader Saver");
I_EXPORT_COMPONENT(ApplicationSettingsProvider, "Provider for the global application settings", "Qt Settings QSettings Registry Saver");
I_EXPORT_COMPONENT(IniSettingsProvider, "Provider for the settings taken from INI file", "Qt INI Settings QSettings Registry Saver");
I_EXPORT_COMPONENT(FileInfoCopy, "Copy file attaching some additional information like version and license", "Qt Copy Move File Attach License Version Tags");
I_EXPORT_COMPONENT(CopyProcessor, "Copy files from one directory to another one", "Qt Copy File Dir Filter Installation Tool");
I_EXPORT_COMPONENT(FileListProvider, "Provide list of file synchronized with some directory", "File List Provider Serialize Directory Model Observer");
I_EXPORT_COMPONENT(ClipboardSerializer, "Load and store objects from and to clipboard", "System Clipboard Loader Serializer Qt Copy Paste");

I_EXPORT_COMPONENT(ExtendedDocumentTemplate, "Extended serialized document template", "Serialized Document Template Qt");
I_EXPORT_COMPONENT(DoxygenHelpFileProvider, "Provide html help file path converting class type using doxygen format", "DoxyGen Help File Provider Document Html Description Class Interface Qt");


} // namespace QtPck


