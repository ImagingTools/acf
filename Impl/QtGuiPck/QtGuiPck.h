#ifndef QtGuiPck_included
#define QtGuiPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtgui/CGuiApplicationComp.h"
#include "iqtgui/CDialogApplicationComp.h"
#include "iqtgui/CSplashScreenGuiComp.h"
#include "iqtgui/CComposedGuiComp.h"
#include "iqtgui/CLogGuiComp.h"
#include "iqtgui/CIconProviderComp.h" 
#include "iqtgui/CFileDialogLoaderComp.h"
#include "iqtgui/CDockWidgetGuiComp.h"
#include "iqtgui/CToolBarWidgetGuiComp.h"
#include "iqtgui/CFreeWidgetGuiComp.h"
#include "iqtgui/CTabContainerGuiComp.h"
#include "iqtgui/CAboutWidgetGuiComp.h"
#include "iqtgui/CTabbedMultiViewGuiComp.h"
#include "iqtgui/CFilePersistenceGuiComp.h"
#include "iqtgui/CSimpleMainWindowGuiComp.h"
#include "iqtgui/CDialogGuiComp.h"
#include "iqtgui/CModelDialogGuiComp.h"
#include "iqtgui/CStatusBarWidgetComp.h"
#include "iqtgui/CCommandsBinderComp.h"
#include "iqtgui/CFileSystemExplorerGuiComp.h"
#include "iqtgui/CImagePropertyGuiComp.h"
#include "iqtgui/CObjectPreviewGuiComp.h"
#include "iqtgui/CModelEditorGuiComp.h"
#include "iqtgui/CImagePropertiesFrameComp.h"
#include "iqtgui/CCommandsToolBarGuiComp.h"
#include "iqtgui/CSelectableCommandsProviderComp.h"
#include "iqtgui/CVersionInfoGuiComp.h"
#include "iqtgui/CFileListProviderGuiComp.h"

#include "iqtprm/CFileNameParamGuiComp.h"
#include "iqtprm/CComposedParamsSetGuiComp.h"
#include "iqtprm/CVariableParamGuiComp.h"
#include "iqtprm/CParamsManagerGuiComp.h"
#include "iqtprm/CSelectionParamGuiComp.h"
#include "iqtprm/CSelectionParamIndexGuiComp.h"
#include "iqtprm/CSelectableGuiComp.h"
#include "iqtprm/CSelectableParamsSetGuiComp.h"
#include "iqtprm/CNameParamGuiComp.h"
#include "iqtprm/COptionsManagerGuiComp.h"
#include "iqtprm/CComboParamsManagerGuiComp.h"
#include "iqtprm/CEnableableParamGuiComp.h"

#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CSingleDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CHtmlHelpGuiComp.h"
#include "iqtdoc/CHtmlHelpViewerComp.h"

#include "iqtproc/CProgressManagerGuiComp.h"
#include "iqtproc/CProcessorControlGuiComp.h"
#include "iqtproc/CDocumentProcessingManagerComp.h"
#include "iqtproc/CDocumentProcessingCommandComp.h"
#include "iqtproc/CProcessorCommandComp.h"


/**
	Standard Qt package.
*/
namespace QtGuiPck
{


typedef iqtgui::CGuiApplicationComp GuiApplication;
typedef iqtgui::CDialogApplicationComp DialogApplication;
typedef iqtgui::CSplashScreenGuiComp SplashScreen;
typedef iqtgui::CLogGuiComp LogGui;
typedef iqtgui::CComposedGuiComp ComposedGui;
typedef iqtgui::CIconProviderComp IconProvider;
typedef iqtgui::CFileDialogLoaderComp FileDialogLoader;
typedef iqtgui::CDockWidgetGuiComp DockWidgetGui;
typedef iqtgui::CToolBarWidgetGuiComp ToolBarWidgetGui;
typedef iqtgui::CStatusBarWidgetComp StatusBarWidget;
typedef iqtgui::CFreeWidgetGuiComp FreeWidgetGui;
typedef iqtgui::CTabContainerGuiComp TabContainer;
typedef iqtgui::CAboutWidgetGuiComp AboutGui;
typedef iqtgui::CTabbedMultiViewGuiComp TabbedMultiViewGui;
typedef iqtgui::CFilePersistenceGuiComp FilePersistenceGui;
typedef iqtgui::CSimpleMainWindowGuiComp SimpleMainGui;
typedef iqtgui::CDialogGuiComp DialogGui;
typedef iqtgui::CModelDialogGuiComp ModelDialogGui;
typedef icomp::TModelCompWrap<iqtgui::CCommandsBinderComp> CommandsBinder;
typedef iqtgui::CFileSystemExplorerGuiComp FileSystemExplorerGui;
typedef iqtgui::CImagePropertyGuiComp ImagePropertyGui;
typedef iqtgui::CObjectPreviewGuiComp ObjectPreviewGui;
typedef iqtgui::CModelEditorGuiComp ModelEditorGui;
typedef iqtgui::CImagePropertiesFrameComp ImagePropertiesFrame;
typedef iqtgui::CCommandsToolBarGuiComp CommandsToolBar;
typedef icomp::TModelCompWrap<iqtgui::CSelectableCommandsProviderComp> SelectableCommandsProvider;
typedef iqtgui::CVersionInfoGuiComp VersionInfoGui;
typedef iqtgui::CFileListProviderGuiComp FileListProviderGui;

typedef iqtprm::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqtprm::CVariableParamGuiComp VariableParamGui;
typedef iqtprm::CParamsManagerGuiComp ParamsManagerGui;
typedef iqtprm::CFileNameParamGuiComp FileNameParamGui;
typedef iqtprm::CSelectionParamGuiComp SelectionParamGui;
typedef iqtprm::CSelectionParamIndexGuiComp SelectionParamIndexGui;
typedef iqtprm::CSelectableGuiComp SelectableGui;
typedef iqtprm::CSelectableParamsSetGuiComp SelectableParamsSetGui;
typedef iqtprm::CNameParamGuiComp NameParamGui;
typedef iqtprm::COptionsManagerGuiComp OptionsManagerGui;
typedef iqtprm::CComboParamsManagerGuiComp ComboParamsManagerGui;
typedef iqtprm::CEnableableParamGuiComp EnableableParamGui;

typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CSingleDocumentWorkspaceGuiComp> SingleDocWorkspaceGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef iqtdoc::CHtmlHelpGuiComp HtmlHelpGui;
typedef iqtdoc::CHtmlHelpViewerComp HtmlHelpViewer;

typedef iqtproc::CProgressManagerGuiComp ProgressManagerGui;
typedef iqtproc::CProcessorControlGuiComp ProcessorControlGui;
typedef iqtproc::CDocumentProcessingManagerComp DocumentProcessingManager;
typedef iqtproc::CDocumentProcessingCommandComp DocumentProcessingCommand;
typedef iqtproc::CProcessorCommandComp ProcessorCommand;


} // namespace QtGuiPck


#endif // !QtGuiPck_included


