#include <QApplication>


#include "CCompositorMainWindow.h"

#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLoginGuiComp.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<iqt::CApplicationComp> appComp;
	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenComp;
	icomp::TSimComponentWrap<iqt::CLoginGuiComp> loginGui;

	appComp.SetRef("SplashScreen", &splashScreenComp);
	appComp.SetRef("MainGui", &loginGui);
	
	return appComp.Execute(argc, argv);
}
