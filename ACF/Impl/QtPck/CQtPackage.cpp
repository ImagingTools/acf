#include "icomp/export.h"

#include "iqt/CLoginGuiComp.h"
#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"

// TODO: Uncomment, if missing feautres implemented
//#include "iqt/CIconProviderComp.h" 
//#include "iqt/CFileDialogSerializerComp.h"

I_EXPORT_PACKAGE("Qt", "Standard Qt package", "Qt Standard");
I_EXPORT_COMPONENT(iqt::CLoginGuiComp, LoginGui, "Gui used to user login", "User Login Qt GUI");
I_EXPORT_COMPONENT(iqt::CApplicationComp, GuiApplication, "Main entry point for window based application", "Application Main Entry Start Begin Qt");
I_EXPORT_COMPONENT(iqt::CSplashScreenGuiComp, SplashScreen, "Splash screen", "Splash Screen Start Begin Qt GUI");

// TODO: Uncomment wenn missing feautres implemented
//I_EXPORT_COMPONENT(iqt::CIconProviderComp, IconProvider, "Icon provider", "Icons Qt GUI"); 
//I_EXPORT_COMPONENT(iqt::CFileDialogSerializerComp, FileDialogSerializer, "File dialog serializer", "File Serialization Qt"); 