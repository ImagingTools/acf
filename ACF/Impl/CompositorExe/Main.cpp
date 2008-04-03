#include <QApplication>


#include "CCompositorMainWindow.h"

#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenComp;
	splashScreenComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CLogGuiComp> logGui;
	logGui.InitComponent();

	icomp::TSimComponentWrap<iqt::CApplicationComp> appComp;
	appComp.SetRef("SplashScreen", &splashScreenComp);
	appComp.SetRef("MainGui", &logGui);
	appComp.InitComponent();

	return appComp.Execute(argc, argv);
}
