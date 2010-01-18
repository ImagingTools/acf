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
#include "BasePck/BasePck.h"
#include "QtPck/QtPck.h"
#include "QtGuiPck/QtGuiPck.h"
#include "CmpstrPck/CmpstrPck.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(icmpstr);

	icomp::TSimComponentWrap<QtGuiPck::GuiApplication> application;
	application.SetDoubleAttr("SplashTime", 1.5);
	application.InitializeApplication(argc, argv);

	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "ACF CompositorMini");
	applicationInfo.SetStringAttr("CompanyName", "ImagingTools");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtPck::ApplicationSettingsProvider> applicationSettingsProvider;
	applicationSettingsProvider.SetRef("ApplicationInfo", &applicationInfo);
	applicationSettingsProvider.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", ":/Icons/CompositorSplashScreen");
	splashScreenGui.SetStringAttr("ProductName", "");
	splashScreenGui.SetStringAttr("ProductType", "Mini");
	splashScreenGui.SetStringAttr("CopyrightText", "Copyright 2009 ImagingTools\nThis tool is a part of ACF project");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::LogGui> log;
	log.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::DockWidgetGui> lockDockComp;
	lockDockComp.SetRef("SlaveGui", &log);
	lockDockComp.SetIntAttr("DockArea", 2);
	lockDockComp.SetStringAttr("DockTitle", "Log");
	lockDockComp.InitComponent();

	icomp::TSimComponentWrap<CmpstrPck::RegistryLoader> registryLoaderComp;
	registryLoaderComp.SetRef("Log", &log);
	registryLoaderComp.SetRef("VersionInfo", &applicationInfo);
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::PackagesLoader> packagesLoaderComp;
	packagesLoaderComp.SetRef("RegistryLoader", &registryLoaderComp);
	packagesLoaderComp.SetRef("Log", &log);
	packagesLoaderComp.InitComponent();

	istd::CString configFile;
	istd::CString registryFile;
	for (int index = 1; index < argc; index++){
		std::string argument = argv[index];
		if (!argument.empty() && (argument[0] == '-')){
			std::string option = argument.substr(1);

			if ((option == "h") || (option == "help")){
				QMessageBox::information(NULL, QObject::tr("Parameter help"), QObject::tr(
							"Usage"
							"\tCompositorMini.exe [registryName] {options}      - registry editor"
							"\t-h or -help              - showing this help"
							"\t-config configFile       - load config file"));

				return 0;
			}
			else if (index < argc - 1){
				if (option == "config"){
					configFile = argv[++index];
				}
			}
		}
		else if (index == 1){
			registryFile = argument;
		}
	}

	packagesLoaderComp.ConfigureEnvironment(configFile);

	icomp::TSimComponentWrap<CmpstrPck::PackageOverview> packageOverviewComp;
	packageOverviewComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	packageOverviewComp.SetRef("PackagesManager", &packagesLoaderComp);
	packageOverviewComp.InitComponent();

	// attribute editor:
	icomp::TSimComponentWrap<CmpstrPck::AttributeEditor> attributeEditorComp;
	attributeEditorComp.SetRef("AttributeSelectionObserver", &packageOverviewComp);
	attributeEditorComp.InitComponent();

	// registry model
	icomp::TSimComponentsFactory<CmpstrPck::RegistryModel> modelFactoryComp;
	modelFactoryComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	modelFactoryComp.SetRef("Log", &log);

	// registry view
	icomp::TSimComponentsFactory<CmpstrPck::RegistryView> viewFactoryComp;
	viewFactoryComp.InsertMultiRef("RegistryElementObservers", &attributeEditorComp);
	viewFactoryComp.SetRef("PackagesManager", &packagesLoaderComp);

	icomp::TSimComponentWrap<QtPck::ExtendedDocumentTemplate> documentTemplateComp;
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &registryLoaderComp);
	documentTemplateComp.SetBoolAttr("IsEditSupported", true);
	documentTemplateComp.SetBoolAttr("IsNewSupported", true);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::SingleDocWorkspaceGui> workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::DockWidgetGui> attributeEditorDockComp;
	attributeEditorDockComp.SetRef("SlaveGui", &attributeEditorComp);
	attributeEditorDockComp.SetIntAttr("DockArea", 0);
	attributeEditorDockComp.SetStringAttr("DockTitle", "Selected component");
	attributeEditorDockComp.InitComponent();
	
	icomp::TSimComponentWrap<QtGuiPck::DockWidgetGui> packageOverviewDockComp;
	packageOverviewDockComp.SetRef("SlaveGui", &packageOverviewComp);
	packageOverviewDockComp.SetStringAttr("DockTitle", "Packages");
	packageOverviewDockComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::AboutGui> aboutGuiComp;
	aboutGuiComp.SetRef("ApplicationInfo", &applicationInfo);
	aboutGuiComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::MainWindowGui> mainWindowComp;
	mainWindowComp.SetIntAttr("IconSize", 32);
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &packageOverviewDockComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &attributeEditorDockComp);
	mainWindowComp.InsertMultiRef("MainWindowComponents", &lockDockComp);
	mainWindowComp.SetRef("ApplicationInfo", &applicationInfo);
	mainWindowComp.SetRef("AboutGui", &aboutGuiComp);
	mainWindowComp.SetRef("SettingsProvider", &applicationSettingsProvider);
	mainWindowComp.InitComponent();

	application.SetRef("ApplicationInfo", &applicationInfo);
	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.SetStringAttr("StyleSheet", ":/Style/Resources/Style/AcfStyle.ass");
	application.InitComponent();

	if (!registryFile.empty()){
		if (!workspaceComp.FileOpen(NULL, &registryFile)){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot load registry\n%1").arg(iqt::GetQString(registryFile)));
		}
	}

	return application.Execute(argc, argv);
}


