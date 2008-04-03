#include <QApplication>

#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLoginGuiComp.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenGui;
	splashScreenGui.SetSimpleAttr("ImagePath", istd::CString("C:\\Work\\Projects\\QUISS\\Projects\\TCVision\\Install\\ToInstall\\QUISS\\Resources\\TCVision.l\\Splash.jpg"));
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<iqt::CLoginGuiComp> loginGui;
	loginGui.InitComponent();

	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.SetRef("MainGui", &loginGui);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}


