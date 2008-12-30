// Qt includes
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>

// ACF includes
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"
#include "icomp/TModelCompWrap.h"

// ACF packages includes
#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"
#include "CmpstrPck/CmpstrPck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.SetBoolAttr("ShowMaximized", true);
	application.SetDoubleAttr("SplashTime", 1.5);
	application.EnsureInitialized(argc, argv);

	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(icmpstr);

	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<BasePck::ApplicationInfoExt> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "ACF Compositor");
	applicationInfo.SetStringAttr("CompanyName", "ImagingTools");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtPck::Log> log;
	log.InitComponent();

	icomp::TSimComponentWrap<QtPck::DockWidgetGui> lockDockComp;
	lockDockComp.SetRef("SlaveGui", &log);
	lockDockComp.SetIntAttr("DockArea", 2);
	lockDockComp.SetStringAttr("DockTitle", "Log");
	lockDockComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../Docs/Images/CompositorSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "");
	splashScreenGui.SetStringAttr("ProductType", "ACF Tool");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<CmpstrPck::RegistryLoader> registryLoaderComp;
	registryLoaderComp.SetRef("Log", &log);
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::PackagesLoader> packagesLoaderComp;
	packagesLoaderComp.SetRef("RegistryLoader", &registryLoaderComp);
	packagesLoaderComp.SetRef("Log", &log);
	packagesLoaderComp.InitComponent();

	std::string registryFile;
	bool useDefaultRegistries = true;
	for (int index = 1; index < argc; index++){
		std::string argument = argv[index];
		if (!argument.empty() && (argument[0] == '-')){
			std::string option = argument.substr(1);

			if ((option == "h") || (option == "help")){
				QMessageBox::information(NULL, QObject::tr("Parameter help"), QObject::tr(
							"Usage"
							"\tCompositor.exe [registryName] {options}      - registry editor"
							"\t-h or -help              - showing this help"
							"\t-packageFile filePath    - append single package file"
							"\t-packageDir directory    - append packages directory"
							"\t-config configFile       - load config file"));

				return 0;
			}
			else if (index < argc - 1){
				if (option == "packageFile"){
					packagesLoaderComp.RegisterPackageFile(argv[++index]);

					useDefaultRegistries = false;
				}
				else if (option == "packageDir"){
					packagesLoaderComp.RegisterPackagesDir(argv[++index]);

					useDefaultRegistries = false;
				}
				else if (option == "config"){
					packagesLoaderComp.LoadConfigFile(argv[++index]);

					useDefaultRegistries = false;
				}
			}
		}
		else if (index == 1){
			registryFile = argument;
		}
	}

	// register default package path
	if (useDefaultRegistries){
		if (!packagesLoaderComp.LoadConfigFile("PackagesConfig.xml")){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!packagesLoaderComp.LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath("PackagesConfig.xml")))){
				packagesLoaderComp.RegisterPackagesDir(iqt::GetCString(applicationDir.absolutePath()));
			}
		}
	}

	icomp::TSimComponentWrap<BasePck::RegistryCodeSaver> codeSaverComp;
	codeSaverComp.InitComponent();

	icomp::TSimComponentWrap<CmpstrPck::PackageOverview> packageOverviewComp;
	packageOverviewComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	packageOverviewComp.InitComponent();

	// attribute editor:
	icomp::TSimComponentWrap<CmpstrPck::AttributeEditor> attributeEditorComp;
	attributeEditorComp.SetRef("AttributeSelectionObserver", &packageOverviewComp);
	attributeEditorComp.InitComponent();

	// registry model
	icomp::TSimComponentsFactory<CmpstrPck::RegistryModel> modelFactoryComp;
	modelFactoryComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	modelFactoryComp.SetRef("Log", &log);

	// registry preview
	icomp::TSimComponentWrap<CmpstrPck::RegistryPreview> registryPreviewComp;
	registryPreviewComp.InitComponent();

	// registry view
	icomp::TSimComponentsFactory<CmpstrPck::RegistryView> viewFactoryComp;
	viewFactoryComp.InsertMultiRef("RegistryElementObservers", &attributeEditorComp);
	viewFactoryComp.SetRef("RegistryCodeSaver", &codeSaverComp);
	viewFactoryComp.SetRef("RegistryPreview", &registryPreviewComp);

	icomp::TSimComponentWrap<QtPck::ExtendedDocumentTemplate> documentTemplateComp;
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &registryLoaderComp);
	documentTemplateComp.SetBoolAttr("IsEditSupported", true);
	documentTemplateComp.SetBoolAttr("IsNewSupported", true);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::MultiDocWorkspaceGui> workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.SetRef("ApplicationInfo", &applicationInfo);
	workspaceComp.SetBoolAttr("ShowViewMaximized", true);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::DockWidgetGui> attributeEditorDockComp;
	attributeEditorDockComp.SetRef("SlaveGui", &attributeEditorComp);
	attributeEditorDockComp.SetIntAttr("DockArea", 0);
	attributeEditorDockComp.SetStringAttr("DockTitle", "Attributes");
	attributeEditorDockComp.InitComponent();
	
	icomp::TSimComponentWrap<QtPck::DockWidgetGui> packageOverviewDockComp;
	packageOverviewDockComp.SetRef("SlaveGui", &packageOverviewComp);
	packageOverviewDockComp.SetStringAttr("DockTitle", "Packages");
	packageOverviewDockComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::MainWindowGui> mainWindowComp;
	mainWindowComp.SetIntAttr("IconSize", 16);
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &packageOverviewDockComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &attributeEditorDockComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &lockDockComp);
	mainWindowComp.InitComponent();

	application.SetRef("ApplicationInfo", &applicationInfo);
	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	if (!registryFile.empty()){
		if (!workspaceComp.OpenDocument(registryFile, true, "")){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot load registry\n%1").arg(registryFile.c_str()));
		}
	}

	return application.Execute(argc, argv);
}

