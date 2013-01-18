#include "QtGuiPck.h"


#include "icomp/export.h"


I_REGISTER_QT_RESOURCE(iqtgui);
I_REGISTER_QT_RESOURCE(iqt2d);


namespace QtGuiPck
{


I_EXPORT_PACKAGE(
			"Acf/Gui",
			"Standard Qt package",
			IM_PROJECT("ACF") IM_TAG("Qt GUI") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			GuiApplication,
			"Main entry point for Qt applications using GUI object as main window",
			"Application Main Entry Start Begin Widget" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			DialogApplication,
			"Main entry point for Qt applications using dialog window as main window",
			"Application Main Entry Start Begin Dialog" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			SplashScreen,
			"Splash screen",
			"Splash Screen Start Begin");

I_EXPORT_COMPONENT(
			ComposedGui,
			"Allow to combine more other GUI's",
			"Composed Combine Collection Composition Slaves" IM_TAG("Composition"));

I_EXPORT_COMPONENT(
			LogGui,
			"Application log as simple list",
			"Log Info List" IM_TAG("Log"));

I_EXPORT_COMPONENT(
			IconProvider,
			"Icon set provider",
			"Icons GUI" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			FileDialogLoader,
			"File dialog loader",
			"Serialization Serializer Dialog" IM_TAG("File Loader"));

I_EXPORT_COMPONENT(
			DockWidgetGui,
			"Allows to use GUI object as docked views",
			"View Dock Document" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			ToolBarWidgetGui,
			"Allows to use GUI object as part of a tool bar",
			"View Dock Document Toolbar" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			StatusBarWidget,
			"Status bar widget",
			"Status Bar Widget Main Window" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			FreeWidgetGui,
			"Display selected GUI object in separated window",
			"Free Main Window Separated" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			SelectionParamGui,
			"Single selection parameter from list of options",
			"Observer Parameters Set Selection Options Choice List" IM_TAG("Editor"));

I_EXPORT_COMPONENT(
			SelectionParamIndexGui,
			"Single selection parameter shown as index number",
			"Observer Parameters Set Selection Options Index Choice" IM_TAG("Editor"));

I_EXPORT_COMPONENT(
			TabContainer,
			"Tab container for Qt widgets",
			"Composed Combine Tab Widget Slaves" IM_TAG("Composition"));

I_EXPORT_COMPONENT(
			AboutGui,
			"About GUI for the application",
			"Help About Widget" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			TabbedMultiViewGui,
			"Tabbed view container for different observer widgets",
			"Widget Observer View Model" IM_TAG("Editor Composition"));

I_EXPORT_COMPONENT(
			FilePersistenceGui,
			"Show load and save button for specified object persistence",
			"Widget Layout Load Save Object Loader" IM_TAG("Persistence"));

I_EXPORT_COMPONENT(
			SimpleMainGui,
			"Simple UI for a main window based application",
			"Simple Main" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			DialogGui,
			"Dialog wrapper of an UI component",
			"Dialog");

I_EXPORT_COMPONENT(
			ModelDialogGui,
			"Dialog based model editor",
			"Model Dialog" IM_TAG("Editor"));

I_EXPORT_COMPONENT(
			CommandsBinder,
			"Component for binding of several commands provider into a single one",
			"GUI Menu Action" IM_CATEGORY(I_SERVICE) IM_TAG("Command Composition"));

I_EXPORT_COMPONENT(
			FileSystemExplorerGui,
			"Component for file system visualization",
			"File Path Directory Observer" IM_CATEGORY(I_GUI));

I_EXPORT_COMPONENT(
			FileNameParamGui,
			"File name parameter editor",
			"Model Observer File Directory Url Parameter");

I_EXPORT_COMPONENT(
			ComposedParamsSetGui,
			"Allow to edit set of parameter using simple model observer GUI",
			"Model Observer Parameter Set" IM_TAG("Editor Composition"));

I_EXPORT_COMPONENT(
			VariableParamGui,
			"Allow to edit variable parameter using model-observer GUI",
			"Model Observer Parameter Choice Change Selection Mutable Variable" IM_TAG("Editor"));

I_EXPORT_COMPONENT(
			ParamsManagerGui,
			"Allow to manage parameter set",
			"Model Observer Parameters Set Manager" IM_TAG("Editor Composition"));

I_EXPORT_COMPONENT(
			ModelEditorGui,
			"Connection between GUI and model components",
			"Observer Connection Connector" IM_TAG("Model Editor"));

I_EXPORT_COMPONENT(
			MainWindowGui,
			"Main Qt application window",
			"Main Window Start Begin" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			SingleDocWorkspaceGui,
			"Single document workspace",
			"Workspace Single SDI Document Model View Observer MVC" IM_TAG("Document"));

I_EXPORT_COMPONENT(
			MultiDocWorkspaceGui,
			"Multi document workspace",
			"Workspace Multi MDI Document Model View Observer MVC" IM_TAG("Document"));

I_EXPORT_COMPONENT(
			HtmlHelpGui,
			"Help GUI showing html help document",
			"Help Viewer Document Html Description" IM_TAG("Help"));

I_EXPORT_COMPONENT(
			HtmlHelpViewer,
			"Help viewer showing html document in separated window",
			"Viewer Window Dialog Document Html Description" IM_TAG("Help"));

I_EXPORT_COMPONENT(
			ProgressManagerGui,
			"Progress manager GUI using progress bar widget to visualize progress",
			"Progress Manager" IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			TableBasedModelEditor,
			"Table-based editor for Qt item model compatible data",
			"Table");

I_EXPORT_COMPONENT(
			StatusGui,
			"Status GUI showing category of observed information provider as red, orange or green ball",
			"Table");

I_EXPORT_COMPONENT(
			ImagePropertyGui,
			"Image property viewer",
			"Observer Display GUI Image Bitmap Property");

I_EXPORT_COMPONENT(
			ObjectPreviewGui,
			"Object preview GUI",
			"Observer Display GUI Preview");

I_EXPORT_COMPONENT(
			ImagePropertiesFrame,
			"Frame with image properties",
			"Image Properties");

I_EXPORT_COMPONENT(
			CommandsToolBar,
			"General tool bar component with commands",
			"Toolbar Command");

I_EXPORT_COMPONENT(
			SelectableCommandsProvider,
			"Selectable command list provider",
			"Selection Command List Actions");

I_EXPORT_COMPONENT(
			VersionInfoGui,
			"Module version info overview",
			"Version Info Application Module Components");

I_EXPORT_COMPONENT(
			SelectableGui,
			"Component to show a GUI from the list of options",
			"Selection Stack");

I_EXPORT_COMPONENT(
			SelectableParamsSetGui,
			"Component to show a selected GUI of a parameter set",
			"Selection Stack Parameter");

I_EXPORT_COMPONENT(
			NameParamGui,
			"Component to show a name parameter",
			"Name Parameter");

I_EXPORT_COMPONENT(
			OptionsManagerGui,
			"Options parameter manager editor",
			"Parameter Manager Selection");

I_EXPORT_COMPONENT(
			ComboParamsManagerGui,
			"Parameter manager editor as a combo box",
			"Parameter Manager Selection");

I_EXPORT_COMPONENT(
			EnableableParamGui,
			"Enableable parameter editor",
			"Enableable Parameter");

I_EXPORT_COMPONENT(
			FileListProviderGui,
			"UI for showing a list of files",
			"File List");

I_EXPORT_COMPONENT(
			ButtonBasedSelectionEditor,
			"Selection parameter editor over list of buttons",
			"Selection List Options Parameter");

I_EXPORT_COMPONENT(
			MultiParamsManagerGui,
			"Allows to manage different types of parameter sets",
			"Model Observer Parameters Set Manager");


} // namespace QtGuiPck


