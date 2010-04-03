#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtGuiPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtGuiPck
{


using namespace icomp;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Acf/Qt/Gui", "Standard Qt package", "Qt Standard");

I_EXPORT_COMPONENT(
			GuiApplication,
			"Main entry point for window based Qt application",
			"Application Main Entry Start Begin Qt");

I_EXPORT_COMPONENT(
			SplashScreen,
			"Splash screen",
			"Splash Screen Start Begin Qt GUI");

I_EXPORT_COMPONENT(
			ComposedGui,
			"Allow to combine more other GUI's",
			"Composed Combine Collection Composition Slaves Qt GUI");

I_EXPORT_COMPONENT(
			LogGui,
			"Application log as simple list",
			"Log Info List Qt GUI");

I_EXPORT_COMPONENT(
			IconProvider,
			"Icon set provider",
			II_META_DESCRIPTION(I_SERVICE, "Icons Qt GUI"));

I_EXPORT_COMPONENT(
			FileDialogLoader,
			"File dialog loader",
			"File Loader Serialization Serializer Dialog GUI Qt");

I_EXPORT_COMPONENT(
			DockWidgetGui,
			"Allows to use GUI object as docked views",
			"View Dock Document GUI Qt");

I_EXPORT_COMPONENT(
			ToolBarWidgetGui,
			"Allows to use GUI object as part of a tool bar",
			"View Dock Document GUI Qt Toolbar");

I_EXPORT_COMPONENT(
			FreeWidgetGui,
			"Display selected GUI object in separated window",
			"Free Main Window Separated GUI Qt");

I_EXPORT_COMPONENT(
			SelectionParamGui,
			"Single selection parameter from list of options",
			"Model Observer Parameters Set Selection Options List GUI Qt");

I_EXPORT_COMPONENT(
			TabContainer,
			"Tab container for Qt widgets",
			"Qt Composed Combine Tab Widget Slaves GUI");

I_EXPORT_COMPONENT(
			AboutGui,
			"About GUI for the application",
			"Qt Help About Widget GUI");

I_EXPORT_COMPONENT(
			TabbedMultiViewGui,
			"Tabbed view container for different observer widgets",
			"Qt Widget GUI Observer View Model");

I_EXPORT_COMPONENT(
			FilePersistenceGui,
			"Show load and save button for specified object persistence",
			"Qt Widget GUI Layout Persistence Load Save Object Loader");

I_EXPORT_COMPONENT(
			LoginGui,
			"Gui used to user login",
			"User Login Qt GUI");

I_EXPORT_COMPONENT(
			SimpleMainGui,
			"Simple UI for a main window based application",
			"Qt GUI");

I_EXPORT_COMPONENT(
			DialogGui,
			"Dialog wrapper of an UI component",
			"Qt GUI Dialog");

I_EXPORT_COMPONENT(
			ModelDialogGui,
			"Dialog based model editor",
			"Qt GUI Model Editor Dialog");

I_EXPORT_COMPONENT(
			StatusBarWidget,
			"Status bar widget",
			"Qt GUI Status Bar Widget Main Window");

I_EXPORT_COMPONENT(
			CommandsBinder,
			"Component for binding of several commands provider into a single one",
			II_META_DESCRIPTION(I_SERVICE, "Qt GUI Menu Action Command"));

I_EXPORT_COMPONENT(
			FileNameParamGui,
			"File name parameter editor",
			"Model Observer File Directory Url Parameter Editor GUI Qt");

I_EXPORT_COMPONENT(
			ComposedParamsSetGui,
			"Allow to edit set of parameter using simple model observer GUI",
			"Model Observer Parameter Set Editor GUI Qt");

I_EXPORT_COMPONENT(
			ParamsManagerGui,
			"Allow to manage parameter set",
			"Model Observer Parameters Set Manager GUI Qt");

I_EXPORT_COMPONENT(
			ModelEditorGui,
			"Connection between GUI and model components",
			"Model Observer Editor GUI Connection Connector Qt");

I_EXPORT_COMPONENT(
			MainWindowGui,
			"Main Qt application window",
			"Main Window Start Begin GUI Qt");

I_EXPORT_COMPONENT(
			SingleDocWorkspaceGui,
			"Single document workspace",
			"Workspace Single SDI Document Model View Observer MVC GUI Qt");

I_EXPORT_COMPONENT(
			MultiDocWorkspaceGui,
			"Multi document workspace",
			"Workspace Multi MDI Document Model View Observer MVC GUI Qt");

I_EXPORT_COMPONENT(
			HtmlHelpViewer,
			"Help viewer showing html document",
			"Help Viewer Document Html Description GUI Qt");

I_EXPORT_COMPONENT(
			ProgressManagerGui,
			"Progress manager GUI using progress bar widget to visualize progress",
			"Progress Manager Qt GUI");

I_EXPORT_COMPONENT(
			AcquisitionProcessorGui,
			"Processor acquisition GUI",
			"Acquisition Processor Parameter GUI View");

I_EXPORT_COMPONENT(
			IterationParamsGui,
			"Iterative processor params",
			"Filter Parameter Qt Processing");


} // namespace QtGuiPck


