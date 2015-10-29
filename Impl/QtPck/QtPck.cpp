#include "QtPck.h"


#include "icomp/export.h"


namespace QtPck
{


I_EXPORT_PACKAGE(
			"Acf/Qt/Base",
			"Standard Qt package",
			IM_PROJECT("ACF") IM_TAG("Qt") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

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
			"Provider for the settings taken from the INI file",
			"INI Settings QSettings Registry Saver" IM_TAG("Application Persistence"));

I_EXPORT_COMPONENT(
			ClipboardSerializer,
			"Load and store objects from and to clipboard",
			"System Clipboard Serializer Copy Paste" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			TranslationManager,
			"Text localization manager",
			"Translation" IM_TAG("Localization"));


} // namespace QtPck


