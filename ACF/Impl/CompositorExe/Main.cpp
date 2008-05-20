#include <QMessageBox>
#include <QApplication>

#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"
#include "icomp/TModelCompWrap.h"

#include "iqt/CPackagesLoader.h"

#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"

#include "CRegistryModelComp.h"
#include "CRegistryViewComp.h"
#include "CPackageOverviewComp.h"
#include "CAttributeEditorComp.h"
#include "CRegistryPreviewComp.h"


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

	icomp::TSimComponentWrap<icomp::TMakeComponentWrap<icomp::CPackageStaticInfo, iqt::CPackagesLoader> > packagesLoaderComp;
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
					packagesLoaderComp.RegisterPackageFile(argv[++index], false);

					useDefaultRegistries = false;
				}
				else if (option == "packageDir"){
					packagesLoaderComp.RegisterPackagesDir(argv[++index], false);

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
		if (!packagesLoaderComp.LoadConfigFile("./PackagesConfig.xml")){
			packagesLoaderComp.RegisterPackagesDir(".", false);
		}
	}

	icomp::TSimComponentWrap<BasePck::RegistryCodeSaver> codeSaverComp;
	codeSaverComp.InitComponent();

	icomp::TSimComponentWrap<CPackageOverviewComp> packageOverviewComp;
	packageOverviewComp.SetRef("StaticComponentInfo", &packagesLoaderComp);
	packageOverviewComp.InitComponent();

	// attribute editor:
	icomp::TSimComponentWrap<CAttributeEditorComp> attributeEditorComp;
	attributeEditorComp.SetRef("AttributeSelectionObserver", &packageOverviewComp);
	attributeEditorComp.InitComponent();

	// registry model
	icomp::TSimComponentsFactory<icomp::TModelCompWrap<CRegistryModelComp> > modelFactoryComp;
	modelFactoryComp.SetRef("StaticComponentInfo", &packagesLoaderComp);

	// registry preview
	icomp::TSimComponentWrap<CRegistryPreviewComp> registryPreviewComp;
	registryPreviewComp.InitComponent();

	// registry view
	icomp::TSimComponentsFactory<CRegistryViewComp> viewFactoryComp;
	viewFactoryComp.InsertMultiRef("RegistryElementObservers", &attributeEditorComp);
	viewFactoryComp.SetRef("RegistryCodeSaver", &codeSaverComp);
	viewFactoryComp.SetRef("RegistryPreview", &registryPreviewComp);

	icomp::TSimComponentWrap<istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> > registryLoaderComp;
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::ExtendedDocumentTemplate> documentTemplateComp;
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
	mainWindowComp.InitComponent();

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

