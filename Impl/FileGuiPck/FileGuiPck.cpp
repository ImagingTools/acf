#include "FileGuiPck.h"


#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace FileGuiPck
{


I_EXPORT_PACKAGE(
			"Acf/FileGui",
			"UI component packages for the file system access and visualization",
			IM_PROJECT("ACF") IM_TAG("Qt GUI File") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FileTreeViewGui,
			"UI for showing a tree of files",
			"File Tree View");

I_EXPORT_COMPONENT(
			FileSystemExplorerGui,
			"Component for file system visualization",
			"File Path Directory Observer" IM_CATEGORY(I_GUI));

I_EXPORT_COMPONENT(
			FilePersistenceGui,
			"Show load and save button for specified object persistence",
			"Widget Layout Load Save Object Loader" IM_TAG("Persistence"));

I_EXPORT_COMPONENT(
			FileDialogLoader,
			"File dialog loader",
			"Serialization Serializer Dialog" IM_TAG("File Loader"));

I_EXPORT_COMPONENT(
			FilePreviewGui,
			"File preview",
			"Observer Display GUI Preview");

I_EXPORT_COMPONENT(
			FileListProviderGui,
			"UI for showing a list of files",
			"File List");

I_EXPORT_COMPONENT(
			SimpleFilePathParamGui,
			"Simplified file path editor",
			"File Path");


} // namespace FileGuiPck


