#ifndef OLD_ACF_SUPPORT
#include "icomp/export.h"

#include "iqt/CLoginGuiComp.h"
#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"

// TODO: Uncomment, if missing feautres implemented
//#include "iqt/CIconProviderComp.h" 
//#include "iqt/CFileDialogSerializerComp.h"

I_EXPORT_PACKAGE("Qt", "Standard Qt package", "Qt Standard");
I_EXPORT_COMPONENT(iqt::CLoginGuiComp, LoginGui, "Gui used to user login", "User Login Qt GUI");
I_EXPORT_COMPONENT(iqt::CApplicationComp, GuiApplication, "Main entry point for window based application", "Application Main Entry Start Begin Qt");
I_EXPORT_COMPONENT(iqt::CSplashScreenGuiComp, SplashScreen, "Splash screen", "Splash Screen Start Begin Qt GUI");
I_EXPORT_COMPONENT(iqt::CLogGuiComp, Log, "Log", "Simple Log GUI");

// TODO: Uncomment wenn missing feautres implemented
//I_EXPORT_COMPONENT(iqt::CIconProviderComp, IconProvider, "Icon provider", "Icons Qt GUI"); 
//I_EXPORT_COMPONENT(iqt::CFileDialogSerializerComp, FileDialogSerializer, "File dialog serializer", "File Serialization Qt"); 

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
