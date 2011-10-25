#ifndef QtGuiPck_included
#define QtGuiPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtgui/CApplicationComp.h"
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
#include "iqtgui/CLoginGuiComp.h"
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

#include "iqtprm/CFileNameParamGuiComp.h"
#include "iqtprm/CComposedParamsSetGuiComp.h"
#include "iqtprm/CVariableParamGuiComp.h"
#include "iqtprm/CParamsManagerGuiComp.h"
#include "iqtprm/CSelectionParamGuiComp.h"
#include "iqtprm/CSelectionParamIndexGuiComp.h"
#include "iqtprm/CSelectableGuiComp.h"
#include "iqtprm/CSelectableParamsSetGuiComp.h"
#include "iqtprm/COptionsListGuiComp.h"

#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CSingleDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CHtmlHelpGuiComp.h"
#include "iqtdoc/CHtmlHelpViewerComp.h"

#include "iqtproc/CProgressManagerGuiComp.h"
#include "iqtproc/CProcessorControlGuiComp.h"
#include "iqtproc/CDocumentProcessingManagerComp.h"
#include "iqtproc/CDocumentProcessingCommandComp.h"


/**
	Standard Qt package.
*/
namespace QtGuiPck
{


typedef iqtgui::CApplicationComp GuiApplication;
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
typedef iqtgui::CLoginGuiComp LoginGui;
typedef iqtgui::CSimpleMainWindowGuiComp SimpleMainGui;
typedef iqtgui::CDialogGuiComp DialogGui;
typedef iqtgui::CModelDialogGuiComp ModelDialogGui;
typedef iqtgui::CCommandsBinderComp CommandsBinder;
typedef iqtgui::CFileSystemExplorerGuiComp FileSystemExplorerGui;
typedef iqtgui::CImagePropertyGuiComp ImagePropertyGui;
typedef iqtgui::CObjectPreviewGuiComp ObjectPreviewGui;
typedef iqtgui::CModelEditorGuiComp ModelEditorGui;
typedef iqtgui::CImagePropertiesFrameComp ImagePropertiesFrame;

typedef iqtprm::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqtprm::CVariableParamGuiComp VariableParamGui;
typedef iqtprm::CParamsManagerGuiComp ParamsManagerGui;
typedef iqtprm::CFileNameParamGuiComp FileNameParamGui;
typedef iqtprm::CSelectionParamGuiComp SelectionParamGui;
typedef iqtprm::CSelectionParamIndexGuiComp SelectionParamIndexGui;
typedef iqtprm::CSelectableGuiComp SelectableGui;
typedef iqtprm::CSelectableParamsSetGuiComp SelectableParamsSetGui;
typedef iqtprm::COptionsListGuiComp OptionsListGui;

typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CSingleDocumentWorkspaceGuiComp> SingleDocWorkspaceGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef iqtdoc::CHtmlHelpGuiComp HtmlHelpGui;
typedef iqtdoc::CHtmlHelpViewerComp HtmlHelpViewer;

typedef iqtproc::CProgressManagerGuiComp ProgressManagerGui;
typedef iqtproc::CProcessorControlGuiComp ProcessorControlGui;
typedef iqtproc::CDocumentProcessingManagerComp DocumentProcessingManager;
typedef iqtproc::CDocumentProcessingCommandComp DocumentProcessingCommand;


} // namespace QtGuiPck


#endif // !QtGuiPck_included


