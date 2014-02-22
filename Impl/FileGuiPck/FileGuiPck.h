#ifndef FileGuiPck_included
#define FileGuiPck_included


#include "ifilegui/CFileTreeViewGuiComp.h"
#include "ifilegui/CFileSystemExplorerGuiComp.h"
#include "ifilegui/CFilePersistenceGuiComp.h"
#include "ifilegui/CFileDialogLoaderComp.h"
#include "ifilegui/CFilePreviewGuiComp.h"
#include "ifilegui/CFileListProviderGuiComp.h"


/**
	File UI package.
*/
namespace FileGuiPck
{


typedef ifilegui::CFileTreeViewGuiComp FileTreeViewGui;
typedef ifilegui::CFileSystemExplorerGuiComp FileSystemExplorerGui;
typedef ifilegui::CFilePersistenceGuiComp FilePersistenceGui;
typedef ifilegui::CFileDialogLoaderComp FileDialogLoader;
typedef ifilegui::CFilePreviewGuiComp FilePreviewGui;
typedef ifilegui::CFileListProviderGuiComp FileListProviderGui;


} // namespace FileGuiPck


#endif // !FileGuiPck_included


