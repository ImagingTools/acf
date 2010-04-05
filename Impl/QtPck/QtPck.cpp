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


using namespace icomp;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"Acf/Qt/Base",
			"Standard Qt package",
			IM_PROJECT("ACF") IM_TAG("Qt Standard Base") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			PackagesLoader,
			"Load and cache packages from files",
			"Component Registry Cache Package Load File Configuration Components" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			RegistriesManager,
			"Load and cache registries from files",
			"Registry Manager Load File Configuration Components");

I_EXPORT_COMPONENT(
			Bitmap,
			"QImage based bitmap implementation",
			"QImage Bitmap" IM_TAG("Image Model"));

I_EXPORT_COMPONENT(
			BitmapSurface,
			"QImage based bitmap implementation with surface extension",
			"Sampled Function QImage Bitmap Surface" IM_TAG("Image Model 3D"));

I_EXPORT_COMPONENT(
			BitmapLoader,
			"Loader loading bitmaps using Qt standard implementations",
			"Bitmap" IM_TAG("Image File Loader"));

I_EXPORT_COMPONENT(
			SettingsSerializer,
			"Load and save objects using QSettings as storage",
			"QSettings Registry Serializer Saver" IM_TAG("Persistence Loader"));

I_EXPORT_COMPONENT(
			ApplicationSettingsProvider,
			"Provider for the global application settings",
			"Settings QSettings Registry Saver" IM_TAG("Application Persistence"));

I_EXPORT_COMPONENT(
			IniSettingsProvider,
			"Provider for the settings taken from INI file",
			"INI Settings QSettings Registry Saver" IM_TAG("Application Persistence"));

I_EXPORT_COMPONENT(
			FileInfoCopy,
			"Copy file attaching some additional information like version and license",
			"Copy Move Attach License Version Tags" IM_TAG("File Processing"));

I_EXPORT_COMPONENT(
			CopyProcessor,
			"Copy files from one directory to another one",
			"Copy Dir Filter Installation Tool" IM_TAG("File Processing"));

I_EXPORT_COMPONENT(
			FileListProvider,
			"Provide list of file synchronized with some directory",
			"List Provider Directory" IM_TAG("File"));

I_EXPORT_COMPONENT(
			ClipboardSerializer,
			"Load and store objects from and to clipboard",
			"System Clipboard Serializer Copy Paste" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
		ExtendedDocumentTemplate,
		"Extended serialized document template",
		"Serialized Template" IM_TAG("Document"));


} // namespace QtPck


