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
#include "iqtgui/CSelectionParamGuiComp.h"
#include "iqtgui/CTabContainerGuiComp.h"
#include "iqtgui/CAboutWidgetGuiComp.h"
#include "iqtgui/CTabbedMultiViewGuiComp.h"
#include "iqtgui/CFilePersistenceGuiComp.h"
#include "iqtgui/CLoginGuiComp.h"
#include "iqtgui/CSimpleMainWindowGuiComp.h"
#include "iqtgui/CModelDialogGuiComp.h"

#include "iqtprm/CFileNameParamGuiComp.h"
#include "iqtprm/CComposedParamsSetGuiComp.h"
#include "iqtprm/CParamsManagerGuiComp.h"

#include "iqtdoc/CModelEditorGuiComp.h"
#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CSingleDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CHtmlHelpViewerComp.h"

#include "iqtproc/CProgressManagerGuiComp.h"
#include "iqtproc/CAcquisitonProcessorGuiComp.h"
#include "iqtproc/CIterativeProcessorParamsGuiComp.h"


/**
	Standard Qt package.
*/
namespace QtGuiPck
{


typedef iqtgui::CApplicationComp GuiApplication;
typedef iqtgui::CSplashScreenGuiComp SplashScreen;
typedef icomp::TModelCompWrap<iqtgui::CLogGuiComp> LogGui;
typedef iqtgui::CComposedGuiComp ComposedGui;
typedef iqtgui::CIconProviderComp IconProvider;
typedef iqtgui::CFileDialogLoaderComp FileDialogLoader;
typedef iqtgui::CDockWidgetGuiComp DockWidgetGui;
typedef iqtgui::CToolBarWidgetGuiComp ToolBarWidgetGui;
typedef iqtgui::CFreeWidgetGuiComp FreeWidgetGui;
typedef iqtgui::CSelectionParamGuiComp SelectionParamGui;
typedef iqtgui::CTabContainerGuiComp TabContainer;
typedef iqtgui::CAboutWidgetGuiComp AboutGui;
typedef iqtgui::CTabbedMultiViewGuiComp TabbedMultiViewGui;
typedef iqtgui::CFilePersistenceGuiComp FilePersistenceGui;
typedef iqtgui::CLoginGuiComp LoginGui;
typedef iqtgui::CSimpleMainWindowGuiComp SimpleMainGui;
typedef iqtgui::CModelDialogGuiComp ModelDialogGui;

typedef iqtprm::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqtprm::CParamsManagerGuiComp ParamsManagerGui;
typedef iqtprm::CFileNameParamGuiComp FileNameParamGui;

typedef iqtdoc::CModelEditorGuiComp ModelEditorGui;
typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CSingleDocumentWorkspaceGuiComp> SingleDocWorkspaceGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef iqtdoc::CHtmlHelpViewerComp HtmlHelpViewer;

typedef iqtproc::CProgressManagerGuiComp ProgressManagerGui;
typedef iqtproc::CAcquisitonProcessorGuiComp AcquisitionProcessorGui;
typedef iqtproc::CIterativeProcessorParamsGuiComp IterationParamsGui;


} // namespace QtGuiPck


#endif // !QtGuiPck_included


