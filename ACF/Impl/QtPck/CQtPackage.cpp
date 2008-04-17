#ifndef OLD_ACF_SUPPORT
#include "imod/TModelWrap.h"

#include "icomp/export.h"

#include "iqt/CLoginGuiComp.h"
#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"
#include "iqt/CIconProviderComp.h" 
#include "iqt/CFileDialogSerializerComp.h"

#include "iqmain/CMainWindowGuiComp.h"
#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqmain);
	}
} instance;


I_EXPORT_PACKAGE("Qt", "Standard Qt package", "Qt Standard");
I_EXPORT_COMPONENT(iqt::CLoginGuiComp, LoginGui, "Gui used to user login", "User Login Qt GUI");
I_EXPORT_COMPONENT(iqt::CApplicationComp, GuiApplication, "Main entry point for window based application", "Application Main Entry Start Begin Qt");
I_EXPORT_COMPONENT(iqt::CSplashScreenGuiComp, SplashScreen, "Splash screen", "Splash Screen Start Begin Qt GUI");
I_EXPORT_COMPONENT(iqt::CLogGuiComp, Log, "Applcation log as simple list", "Log Info List Qt GUI");
I_EXPORT_COMPONENT(iqt::CIconProviderComp, IconProvider, "Icon provider", "Icons Qt GUI"); 
I_EXPORT_COMPONENT(iqt::CFileDialogSerializerComp, FileDialogSerializer, "File dialog serializer", "File Serialization Serializer Dialog GUI Qt"); 
I_EXPORT_COMPONENT(iqmain::CMainWindowGuiComp, MainWindowGui, "Main Qt application window", "Main Window Start Begin GUI Qt");
I_EXPORT_COMPONENT(imod::TModelWrap<iqmain::CMultiDocumentWorkspaceGuiComp>, MultiDocWorkspace, "Multi document workspace", "Workspace Document Model View Observer MVC GUI Qt");


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
