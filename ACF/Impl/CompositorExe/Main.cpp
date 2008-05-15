#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

#include "iqt/CPackagesLoader.h"
#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"

#include "CRegistryModelComp.h"
#include "CRegistryViewComp.h"
#include "CPackageOverviewComp.h"
#include "CAttributeEditorComp.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtdoc);

	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "ACF Compositor");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.SetRef("ApplicationInfo", &applicationInfo);
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../Docs/Images/CompositorSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductType", "ACF Tool");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<icomp::TMakeComponentWrap<icomp::IComponentStaticInfo, iqt::CPackagesLoader> > packagesLoaderComp;
	// register default package path
	packagesLoaderComp.RegisterPackagesDir(".");
	packagesLoaderComp.LoadConfigFile("./PackagesConfig.xml");
	packagesLoaderComp.InitComponent();

	// attribute editor:
	icomp::TSimComponentWrap<CAttributeEditorComp> attributeEditorComp;
	attributeEditorComp.InitComponent();

	// registry model
	icomp::TSimComponentsFactory<CRegistryModelComp> modelFactoryComp;
	modelFactoryComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	
	// registry view
	icomp::TSimComponentsFactory<CRegistryViewComp> viewFactoryComp;
	viewFactoryComp.InsertMultiRef("RegistryElementObservers", &attributeEditorComp);

	icomp::TSimComponentWrap<istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> > registryLoaderComp;
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::ExtendedDocumentTemplate> documentTemplateComp;
	documentTemplateComp.SetRef("AboutGui", &splashScreenGui);
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &registryLoaderComp);
	documentTemplateComp.InsertMultiAttr("FileFilters", istd::CString("Component registry files (*.arx)"));
	documentTemplateComp.InsertMultiAttr("FileExtensions", istd::CString("arx"));
	documentTemplateComp.SetBoolAttr("IsEditSupported", true);
	documentTemplateComp.SetBoolAttr("IsNewSupported", true);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::MultiDocWorkspaceGui> workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<CPackageOverviewComp> packageOverviewComp;
	packageOverviewComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	packageOverviewComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::DockWidgetGui> attributeEditorDockComp;
	attributeEditorDockComp.SetRef("SlaveGui", &attributeEditorComp);
	attributeEditorDockComp.SetIntAttr("DockArea", 3);
	attributeEditorDockComp.SetStringAttr("DockTitle", "Attributes");
	attributeEditorDockComp.InitComponent();
	
	icomp::TSimComponentWrap<QtPck::DockWidgetGui> packageOverviewDockComp;
	packageOverviewDockComp.SetRef("SlaveGui", &packageOverviewComp);
	packageOverviewDockComp.SetStringAttr("DockTitle", "Packages");
	packageOverviewDockComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::MainWindowGui> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &packageOverviewDockComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &attributeEditorDockComp);
	mainWindowComp.InitComponent();

	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

