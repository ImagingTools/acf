#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

#include "BasePck/BasePck.h"
#include "QtPck/QtPck.h"

#include "CLogClientGuiComp.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqt);

	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "Log");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<BasePck::XmlFileSerializer> serializerComp;
	serializerComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::FileDialogSerializer> fileDialogSerializerComp;
	fileDialogSerializerComp.InsertMultiRef("Serializers", &serializerComp);
	fileDialogSerializerComp.InsertMultiAttr("FileFilters", istd::CString("Log Files (*.xml)"));
	fileDialogSerializerComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::Log> log;
	log.SetIntAttr("MaxMessageCount", 20);
	log.SetRef("Serializer", &fileDialogSerializerComp);
	log.InitComponent();

	icomp::TSimComponentWrap<CLogClientGuiComp> logClientGui;
	logClientGui.SetRef("Log", &log);
	logClientGui.InitComponent();

	application.SetRef("MainGui", &logClientGui);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

