// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <icomp/TSimComponentsFactory.h>

// ACF package includes
#include <BasePck/BasePck.h>
#include <QtPck/QtPck.h>
#include <QtGuiPck/QtGuiPck.h>

#include <TutorialPck/TutorialPck.h>


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);

	icomp::TSimComponentWrap<QtGuiPck::GuiApplication> application;
	application.SetDoubleAttr("SplashTime", 1.5);
	application.InitializeApplication(argc, argv);

	icomp::TSimComponentWrap<BasePck::VersionInfo> versionInfo;
	versionInfo.SetIntAttr("VersionNumber", 1000);
	versionInfo.InsertMultiAttr("KnownVersions", 1000);
	versionInfo.InsertMultiAttr("KnownVersionNames", QString("1.0.0"));
	versionInfo.InitComponent();

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "DocView");
	applicationInfo.SetStringAttr("ApplicationType", "Tutorial");
	applicationInfo.SetStringAttr("LegalCopyright", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	applicationInfo.SetRef("VersionInfo", &versionInfo);
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentsFactory<BasePck::TextDocument> modelFactoryComp;
	modelFactoryComp.SetStringAttr("DefaultText", "Ahoj przygodo!");

	icomp::TSimComponentsFactory<TutorialPck::TextEditor> viewFactoryComp;

	icomp::TSimComponentWrap<BasePck::TextFileLoader> loaderComp;
	loaderComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::ExtendedDocumentTemplate> documentTemplateComp;
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &loaderComp);
	documentTemplateComp.SetBoolAttr("IsEditSupported", true);
	documentTemplateComp.SetBoolAttr("IsNewSupported", true);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::MultiDocWorkspaceGui> workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::MainWindowGui> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InitComponent();

	application.SetRef("ApplicationInfo", &applicationInfo);
	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

