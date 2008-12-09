#include <QDir>

#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Qt", "Standard Qt package", "Qt Standard");

I_EXPORT_COMPONENT(PackagesLoader, "Load and cache packages from files", "Component Cache Package Load File");
I_EXPORT_COMPONENT(LoginGui, "Gui used to user login", "User Login Qt GUI");
I_EXPORT_COMPONENT(GuiApplication, "Main entry point for window based Qt application", "Application Main Entry Start Begin Qt");
I_EXPORT_COMPONENT(SplashScreen, "Splash screen", "Splash Screen Start Begin Qt GUI");
I_EXPORT_COMPONENT(Log, "Applcation log as simple list", "Log Info List Qt GUI");
I_EXPORT_COMPONENT(IconProvider, "Icon provider", "Icons Qt GUI"); 
I_EXPORT_COMPONENT(FileDialogSerializer, "File dialog serializer", "File Serialization Serializer Dialog GUI Qt"); 
I_EXPORT_COMPONENT(ModelEditorGui, "Connection between GUI and model components", "Model Observer Editor GUI Connection Connector Qt");
I_EXPORT_COMPONENT(ExtendedDocumentTemplate, "Extended serialized document template", "Serialized Document Template Qt");
I_EXPORT_COMPONENT(MainWindowGui, "Main Qt application window", "Main Window Start Begin GUI Qt");
I_EXPORT_COMPONENT(MultiDocWorkspaceGui, "Multi document workspace", "Workspace Document Model View Observer MVC GUI Qt");

I_EXPORT_COMPONENT(Bitmap, "QImage based bitmap implementation", "QImage Image Bitmap Qt");
I_EXPORT_COMPONENT(BitmapLoader, "Loader loading bitmaps using Qt standard implementations", "Image Bitmap Qt File Loader");
I_EXPORT_COMPONENT(SceneConnector, "Display scane provider, scene extender and connect it", "Scene Connector Provider Extender Display Console GUI Qt");
I_EXPORT_COMPONENT(SceneProvider, "Standard scene provider", "Scene Provider Extender Display Console GUI Qt");
I_EXPORT_COMPONENT(ImageView, "Display bitmap using observer pattern", "Image Bitmap Display Viewer Observer Qt");
I_EXPORT_COMPONENT(Line2dParamsGui, "Line parameter GUI displaying line using scene", "Line Parameters Observer Display GUI Qt");
I_EXPORT_COMPONENT(RectangleParamsGui, "Rectangle parameter GUI displaying a rectangle using scene", "Rectangle Parameters Observer Display GUI Qt");
I_EXPORT_COMPONENT(CircleParamsGui, "Circle parameter GUI displaying a cicle using scene", "Circle Parameters Observer Display GUI Qt");
I_EXPORT_COMPONENT(AnnulusParamsGui, "Annulus parameter GUI displaying a annulus using scene", "Annulus Radius Radii Parameters Observer Display GUI Qt");
I_EXPORT_COMPONENT(AnnulusSegmentParamsGui, "Annulus segment parameter GUI displaying a annulus using scene", "Annulus Segment Angle Radius Radii Parameters Observer Display GUI Qt");

I_EXPORT_COMPONENT(ComposedParamsSetGui, "Allow to edit set of parameter using simple model observer GUI", "Model Observer Parameter Set Editor GUI Qt");
I_EXPORT_COMPONENT(FileNameParamGui, "File acquisition parameter editor", "Model Observer File Acquisition Snap Camera Parameter Editor GUI Qt");
I_EXPORT_COMPONENT(ParamsManagerGui, "Allow to manage parameter set", "Model Observer Parameters Set Manager GUI Qt");
I_EXPORT_COMPONENT(DockWidgetGui, "Allows to use GUI object as docked views", "View Dock Document GUI Qt");
I_EXPORT_COMPONENT(FreeWidgetGui, "Display selected GUI object in separated window", "Free Main Window Separated GUI Qt");
I_EXPORT_COMPONENT(SelectionParamGui, "Single selection parameter from list of options", "Model Observer Parameters Set Selection Options List GUI Qt");
I_EXPORT_COMPONENT(TabContainer, "Tab container for Qt widgets", "Qt Tab Widget GUI");
I_EXPORT_COMPONENT(SettingsSerializer, "Load and save objects using QSettings as storage", "Qt QSettings Registry Serializer Loader Saver");

I_EXPORT_COMPONENT(ScintillaTextEditor, "Scintilla text editor", "Qt Text Editor GUI Scintilla");


} // namespace QtPck


