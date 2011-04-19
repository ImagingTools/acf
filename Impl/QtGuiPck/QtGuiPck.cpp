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

I_EXPORT_PACKAGE(
			"Acf/Qt/Gui",
			"Standard Qt package",
			IM_PROJECT("ACF") IM_TAG("Qt") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			GuiApplication,
			"Main entry point for window based Qt application",
			"Application Main Entry Start Begin" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			SplashScreen,
			"Splash screen",
			"Splash Screen Start Begin" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ComposedGui,
			"Allow to combine more other GUI's",
			"Composed Combine Collection Composition Slaves" IM_TAG("GUI Composition"));

I_EXPORT_COMPONENT(
			LogGui,
			"Application log as simple list",
			"Log Info List" IM_TAG("GUI Log"));

I_EXPORT_COMPONENT(
			IconProvider,
			"Icon set provider",
			"Icons GUI" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			FileDialogLoader,
			"File dialog loader",
			"Serialization Serializer Dialog" IM_TAG("GUI File Loader"));

I_EXPORT_COMPONENT(
			DockWidgetGui,
			"Allows to use GUI object as docked views",
			"View Dock Document" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			ToolBarWidgetGui,
			"Allows to use GUI object as part of a tool bar",
			"View Dock Document Toolbar" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			StatusBarWidget,
			"Status bar widget",
			"Status Bar Widget Main Window" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			FreeWidgetGui,
			"Display selected GUI object in separated window",
			"Free Main Window Separated" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			SelectionParamGui,
			"Single selection parameter from list of options",
			"Observer Parameters Set Selection Options Choice List" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			SelectionParamIndexGui,
			"Single selection parameter shown as index number",
			"Observer Parameters Set Selection Options Index Choice" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			TabContainer,
			"Tab container for Qt widgets",
			"Composed Combine Tab Widget Slaves" IM_TAG("GUI Composition"));

I_EXPORT_COMPONENT(
			AboutGui,
			"About GUI for the application",
			"Help About Widget" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			TabbedMultiViewGui,
			"Tabbed view container for different observer widgets",
			"Widget Observer View Model" IM_TAG("GUI Editor Composition"));

I_EXPORT_COMPONENT(
			FilePersistenceGui,
			"Show load and save button for specified object persistence",
			"Widget Layout Load Save Object Loader" IM_TAG("GUI Persistence"));

I_EXPORT_COMPONENT(
			LoginGui,
			"Gui used to user login",
			"User Login" IM_TAG("GUI Authorization"));

I_EXPORT_COMPONENT(
			SimpleMainGui,
			"Simple UI for a main window based application",
			"Simple Main" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			DialogGui,
			"Dialog wrapper of an UI component",
			"Dialog" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ModelDialogGui,
			"Dialog based model editor",
			"Model Dialog" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			CommandsBinder,
			"Component for binding of several commands provider into a single one",
			"Qt GUI Menu Action" IM_CATEGORY(I_SERVICE) IM_TAG("Command Composition"));

I_EXPORT_COMPONENT(
			FileSystemExplorerGui,
			"Component for file system visualization",
			"File Path Directory Observer" IM_CATEGORY(I_GUI) IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			FileNameParamGui,
			"File name parameter editor",
			"Model Observer File Directory Url Parameter" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			ComposedParamsSetGui,
			"Allow to edit set of parameter using simple model observer GUI",
			"Model Observer Parameter Set" IM_TAG("GUI Editor Composition"));

I_EXPORT_COMPONENT(
			VariableParamGui,
			"Allow to edit variable parameter using model-observer GUI",
			"Model Observer Parameter Choice Change Selection Mutable Variable" IM_TAG("GUI Editor"));

I_EXPORT_COMPONENT(
			ParamsManagerGui,
			"Allow to manage parameter set",
			"Model Observer Parameters Set Manager" IM_TAG("GUI Editor Composition"));

I_EXPORT_COMPONENT(
			ModelEditorGui,
			"Connection between GUI and model components",
			"Observer Connection Connector Qt" IM_TAG("GUI Model Editor"));

I_EXPORT_COMPONENT(
			MainWindowGui,
			"Main Qt application window",
			"Main Window Start Begin" IM_TAG("GUI Application"));

I_EXPORT_COMPONENT(
			SingleDocWorkspaceGui,
			"Single document workspace",
			"Workspace Single SDI Document Model View Observer MVC" IM_TAG("GUI Document"));

I_EXPORT_COMPONENT(
			MultiDocWorkspaceGui,
			"Multi document workspace",
			"Workspace Multi MDI Document Model View Observer MVC" IM_TAG("GUI Document"));

I_EXPORT_COMPONENT(
			HtmlHelpViewer,
			"Help viewer showing html document",
			"Help Viewer Document Html Description" IM_TAG("GUI Help"));

I_EXPORT_COMPONENT(
			ProgressManagerGui,
			"Progress manager GUI using progress bar widget to visualize progress",
			"Progress Manager" IM_TAG("GUI Processing"));

I_EXPORT_COMPONENT(
			ProcessorControlGui,
			"Processor acquisition GUI",
			"Acquisition Processor Parameter View" IM_TAG("GUI Image Processing"));

I_EXPORT_COMPONENT(
			ImagePropertyGui,
			"Image property viewer",
			"Observer Display GUI Image Bitmap Property" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ObjectPreviewGui,
			"Object preview GUI",
			"Observer Display GUI Preview" IM_TAG("GUI"));


} // namespace QtGuiPck


