#ifndef OLD_ACF_SUPPORT


#include <QDir>

#include "icomp/export.h"

#include "QtPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtdoc);
	}
} instance;


namespace QtPck
{


I_EXPORT_PACKAGE("Qt", "Standard Qt package", "Qt Standard");

I_EXPORT_COMPONENT(LoginGui, "Gui used to user login", "User Login Qt GUI");
I_EXPORT_COMPONENT(GuiApplication, "Main entry point for window based Qt application", "Application Main Entry Start Begin Qt");
I_EXPORT_COMPONENT(SplashScreen, "Splash screen", "Splash Screen Start Begin Qt GUI");
I_EXPORT_COMPONENT(Log, "Applcation log as simple list", "Log Info List Qt GUI");
I_EXPORT_COMPONENT(IconProvider, "Icon provider", "Icons Qt GUI"); 
I_EXPORT_COMPONENT(FileDialogSerializer, "File dialog serializer", "File Serialization Serializer Dialog GUI Qt"); 
I_EXPORT_COMPONENT(ExtendedDocumentTemplate, "Extended serialized document template", "Serialized Document Template Qt");
I_EXPORT_COMPONENT(MainWindowGui, "Main Qt application window", "Main Window Start Begin GUI Qt");
I_EXPORT_COMPONENT(MultiDocWorkspaceGui, "Multi document workspace", "Workspace Document Model View Observer MVC GUI Qt");
I_EXPORT_COMPONENT(Bitmap, "QImage based bitmap implementation", "QImage Image Bitmap Qt");
I_EXPORT_COMPONENT(ImageView, "Display bitmap using observer pattern", "Image Bitmap Display Viewer Observer Qt");
I_EXPORT_COMPONENT(BitmapLoader, "Load and save bitmaps in binary format", "Image Bitmap Binary Loader Load Save JPG JPEG PNG BMP Qt");
I_EXPORT_COMPONENT(DockWidgetGui, "Allows to use GUI object as docked views", "View Dock Document GUI Qt");
I_EXPORT_COMPONENT(SnapImageGui, "Show snapped image and allow to change snap parameters", "View Image Bitmap Camera Snap Parameters GUI Qt");
I_EXPORT_COMPONENT(ComposedParamsSetGui, "Allow to edit set of parameter using simple model observer GUI", "Model Observer Parameter Set Editor GUI Qt");
I_EXPORT_COMPONENT(FileAcquisitionParamsGui, "File acquisition parameter editor", "Model Observer File Acquisition Snap Camera Parameter Editor GUI Qt");


} // namespace QtPck


#else OLD_ACF_SUPPORT


#include "CQtPackage.h"

#include "iqt/CTabContainerGuiComp.h"
#include "iqt/CLoginGuiComp.h"
#include "iqt/CLogGuiComp.h"


CQtPackage::CQtPackage()
:	BaseClass()
{
	acf::ComponentLibraryInfo info;
	info.SetLibName("Qt");
	SetLibraryInfo(info);

	RegisterComponent<iqt::CTabContainerGuiComp>("TabContainerGui");
	RegisterComponent<iqt::CLoginGuiComp>("LoginGui");
}


EXPORT_COMPONENT_LIBRARY(CQtPackage);

#endif // OLD_ACF_SUPPORT
