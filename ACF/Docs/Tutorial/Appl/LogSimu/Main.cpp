#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

#include "istdc/TSingleFactoryComp.h"
#include "istdc/TFileSerializerComp.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"
#include "iqt/CFileDialogSerializerComp.h"

#include "CLogClientGuiComp.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqt);

	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "Log");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "See 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> SerializerComp;
	icomp::TSimComponentWrap<SerializerComp> serializerComp;
	serializerComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CFileDialogSerializerComp> fileDialogSerializerComp;
	fileDialogSerializerComp.InsertMultiRef("Serializers", &serializerComp);
	fileDialogSerializerComp.InsertMultiAttr("FileFilters", istd::CString("Log Files (*.xml)"));
	fileDialogSerializerComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CLogGuiComp> log;
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

