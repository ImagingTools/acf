#ifndef QtGuiPck_included
#define QtGuiPck_included


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iqtgui/CGuiApplicationComp.h>
#include <iqtgui/CDialogApplicationComp.h>
#include <iqtgui/CSplashScreenGuiComp.h>
#include <iqtgui/CComposedGuiComp.h>
#include <iqtgui/CIconProviderComp.h> 
#include <iqtgui/CDockWidgetGuiComp.h>
#include <iqtgui/CToolBarWidgetGuiComp.h>
#include <iqtgui/CFreeWidgetGuiComp.h>
#include <iqtgui/CTabContainerGuiComp.h>
#include <iqtgui/CAboutWidgetGuiComp.h>
#include <iqtgui/CSimpleMainWindowGuiComp.h>
#include <iqtgui/CDialogGuiComp.h>
#include <iqtgui/CModelDialogGuiComp.h>
#include <iqtgui/CStatusBarWidgetComp.h>
#include <iqtgui/CCommandsBinderComp.h>
#include <iqtgui/CImagePropertyGuiComp.h>
#include <iqtgui/CModelEditorGuiComp.h>
#include <iqtgui/CImagePropertiesFrameComp.h>
#include <iqtgui/CCommandsToolBarGuiComp.h>
#include <iqtgui/CCommandBasedSelectionControllerComp.h>
#include <iqtgui/CVersionInfoGuiComp.h>
#include <iqtgui/CProgressManagerGuiComp.h>
#include <iqtgui/CTableBasedModelEditorComp.h>
#include <iqtgui/CStatusGuiComp.h>
#include <iqtgui/CCalendarGuiComp.h>
#include <iqtgui/CCalendarGuiComp.h>
#include <iqtgui/CCommandsMultiplexerComp.h>
#include <iqtgui/CProcessStartCommandComp.h>


// ParamGuiPck
#include <iqtprm/CFileNameParamGuiComp.h>
#include <iqtprm/CComposedParamsSetGuiComp.h>
#include <iqtprm/CVariableParamGuiComp.h>
#include <iqtprm/CParamsManagerGuiComp.h>
#include <iqtprm/CSelectionParamGuiComp.h>
#include <iqtprm/CSelectionParamIndexGuiComp.h>
#include <iqtprm/CSelectableGuiComp.h>
#include <iqtprm/CSelectableParamsSetGuiComp.h>
#include <iqtprm/CNameParamGuiComp.h>
#include <iqtprm/COptionsManagerGuiComp.h>
#include <iqtprm/CEnableableParamGuiComp.h>
#include <iqtprm/CButtonBasedSelectionParamGuiComp.h>
#include <iqtprm/CMultiParamsManagerGuiComp.h>
#include <iqtprm/COptionsListEditorComp.h>
#include <iqtprm/COptionsManagerEditorComp.h>
#include <iqtprm/CCheckableOptionsEditorComp.h>
#include <iqtprm/CExtComposedParamsSetGuiComp.h>
#include <iqtprm/CExtParamsManagerGuiComp.h>

// DocumentGuiPck
#include <iqtdoc/CMainWindowGuiComp.h>
#include <iqtdoc/CSingleDocumentWorkspaceGuiComp.h>
#include <iqtdoc/CMultiDocumentWorkspaceGuiComp.h>
#include <iqtdoc/CHtmlHelpGuiComp.h>
#include <iqtdoc/CHtmlHelpViewerComp.h>
#include <iqtdoc/CDocumentMetaInfoEditorComp.h>
#include <iqtdoc/CSingletonDocApplicationComp.h>
#include <iqtdoc/CDocumentImportExportCommandComp.h>
#include <iqtdoc/CExternalOpenDocumentCommandComp.h>
#include <iqtdoc/CExtendedDocumentTemplateComp.h>
#include <iqtdoc/CStaticHelpFileProviderComp.h>
#include <iqtdoc/CDoxygenHelpFileProviderComp.h>

// LogGuiPck
#include <iloggui/CLogGuiComp.h>
#include <iloggui/CTextLogGuiComp.h>
#include <iloggui/CMessageBoxComp.h>


/**
	Standard Qt package.
*/
namespace QtGuiPck
{


typedef iqtgui::CGuiApplicationComp GuiApplication;
typedef iqtgui::CDialogApplicationComp DialogApplication;
typedef iqtgui::CSplashScreenGuiComp SplashScreen;
typedef iqtgui::CComposedGuiComp ComposedGui;
typedef iqtgui::CIconProviderComp IconProvider;
typedef iqtgui::CDockWidgetGuiComp DockWidgetGui;
typedef iqtgui::CToolBarWidgetGuiComp ToolBarWidgetGui;
typedef iqtgui::CStatusBarWidgetComp StatusBarWidget;
typedef iqtgui::CFreeWidgetGuiComp FreeWidgetGui;
typedef iqtgui::CTabContainerGuiComp TabContainer;
typedef iqtgui::CAboutWidgetGuiComp AboutGui;
typedef iqtgui::CSimpleMainWindowGuiComp SimpleMainGui;
typedef iqtgui::CDialogGuiComp DialogGui;
typedef iqtgui::CModelDialogGuiComp ModelDialogGui;
typedef icomp::TModelCompWrap<iqtgui::CCommandsBinderComp> CommandsBinder;
typedef iqtgui::CImagePropertyGuiComp ImagePropertyGui;
typedef iqtgui::CModelEditorGuiComp ModelEditorGui;
typedef iqtgui::CImagePropertiesFrameComp ImagePropertiesFrame;
typedef iqtgui::CCommandsToolBarGuiComp CommandsToolBar;
typedef icomp::TModelCompWrap<iqtgui::CCommandBasedSelectionControllerComp> CommandBasedSelectionController;
typedef iqtgui::CVersionInfoGuiComp VersionInfoGui;
typedef iqtgui::CProgressManagerGuiComp ProgressManagerGui;
typedef iqtgui::CTableBasedModelEditorComp TableBasedModelEditor;
typedef iqtgui::CStatusGuiComp StatusGui;
typedef iqtgui::CCalendarGuiComp CalendarGui;
typedef icomp::TModelCompWrap<iqtgui::CCommandsMultiplexerComp> CommandsMultiplexer;
typedef iqtgui::CProcessStartCommandComp ProcessStartCommand;

typedef iqtdoc::CSingletonDocApplicationComp SingletonDocApplication;
typedef iqtdoc::CDocumentImportExportCommandComp DocumentImportExportCommand;
typedef iqtdoc::CExternalOpenDocumentCommandComp ExternalOpenDocumentCommand;

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
typedef iqtprm::COptionsListEditorComp OptionsListEditor;
typedef iqtprm::COptionsManagerEditorComp OptionsManagerEditor;
typedef iqtprm::CCheckableOptionsEditorComp CheckableOptionsEditor;
typedef iqtprm::CEnableableParamGuiComp EnableableParamGui;
typedef iqtprm::CButtonBasedSelectionParamGuiComp ButtonBasedSelectionEditor;
typedef iqtprm::CMultiParamsManagerGuiComp MultiParamsManagerGui;
typedef iqtprm::CExtComposedParamsSetGuiComp ExtComposedParamsSetGui;
typedef iqtprm::CExtParamsManagerGuiComp ExtParamsManagerGui;

typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CSingleDocumentWorkspaceGuiComp> SingleDocWorkspaceGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef iqtdoc::CHtmlHelpGuiComp HtmlHelpGui;
typedef iqtdoc::CHtmlHelpViewerComp HtmlHelpViewer;
typedef iqtdoc::CDocumentMetaInfoEditorComp DocumentMetaInfoEditor;
typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CStaticHelpFileProviderComp StaticHelpFileProvider;
typedef iqtdoc::CDoxygenHelpFileProviderComp DoxygenHelpFileProvider;

typedef iloggui::CLogGuiComp LogGui;
typedef iloggui::CTextLogGuiComp TextLogGui;
typedef iloggui::CMessageBoxComp MessageBoxBasedLog;


} // namespace QtGuiPck


#endif // !QtGuiPck_included


