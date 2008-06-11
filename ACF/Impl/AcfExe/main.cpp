#include <iostream>

#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QLabel>

#include "ibase/IApplication.h"

#include "icomp/TComponentWrap.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/TMakeComponentWrap.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CCompositeComponent.h"
#include "icomp/CRegistryElement.h"

#include "iqt/CPackagesLoaderComp.h"

#include "BasePck/BasePck.h"


int main(int argc, char *argv[])
{
	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<BasePck::XmlFileSerializer> registryLoaderComp;
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CPackagesLoaderComp> packagesLoaderComp;
	packagesLoaderComp.SetRef("RegistryLoader", &registryLoaderComp);
	packagesLoaderComp.InitComponent();

	std::string registryFile = "default.arx";
	bool showApplicationInfo = false;
	bool useDefaultRegistries = true;
	std::string componentId;
	bool waitOnEnd = false;

	for (int index = 1; index < argc; index++){
		std::string argument = argv[index];
		if (!argument.empty() && (argument[0] == '-')){
			std::string option = argument.substr(1);

			if ((option == "h") || (option == "help")){
				std::cout << "Usage";
				std::cout << "\tAcf.exe [registryName] {options}      - normal registry start" << std::endl;
				std::cout << "\t-h or -help              - showing help" << std::endl;
				std::cout << "\t-id componentId          - specify component ID." << std::endl;
				std::cout << "\t-packageFile filePath    - append single package file" << std::endl;
				std::cout << "\t-packageDir directory    - append packages directory" << std::endl;
				std::cout << "\t-config configFile       - load config file" << std::endl;
				std::cout << "\t-info                    - application parameter info" << std::endl;
				std::cout << "\t-wait                    - wait on application end" << std::endl;

				return 0;
			}
			else if (option == "info"){
				showApplicationInfo = true;

				break;
			}
			else if (option == "wait"){
				waitOnEnd = true;
			}
			else if (index < argc - 1){
				if (option == "id"){
					componentId = argv[++index];
				}
				else if (option == "packageFile"){
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
		QDir registryDir = QFileInfo(registryFile.c_str()).dir();

		if (!packagesLoaderComp.LoadConfigFile(iqt::GetCString(registryDir.absoluteFilePath("PackagesConfig.xml")))){
			QDir applicationDir = QCoreApplication::applicationDirPath();
			if (!packagesLoaderComp.LoadConfigFile(iqt::GetCString(applicationDir.absoluteFilePath("PackagesConfig.xml")))){
				packagesLoaderComp.RegisterPackagesDir(iqt::GetCString(applicationDir.absolutePath()), false);
			}
		}
	}

	int retVal = 0;

	const icomp::IRegistry* registryPtr = packagesLoaderComp.GetRegistryFromFile(registryFile.c_str());
	if (registryPtr != NULL){
		icomp::CRegistryElement dummyElement(&packagesLoaderComp);

		icomp::CCompositeComponentContext compositeContext(&dummyElement, registryPtr, &packagesLoaderComp);
		icomp::TComponentWrap<icomp::CCompositeComponent> composite(&compositeContext);

		ibase::IApplication* applicationPtr = composite.GetComponentInterface<ibase::IApplication>(componentId);
		if (applicationPtr == NULL){
			std::cout << "Application interface cannot be found" << std::endl;

			retVal = -1;
		}
		else if (showApplicationInfo){
			std::cout << applicationPtr->GetHelpText().ToString();

			retVal = 0;
		}
		else{
			retVal = applicationPtr->Execute(argc, argv);
		}
	}
	else{
		std::cout << QString("Registry %1 cannot be loaded").arg(registryFile.c_str()).toStdString() << std::endl;

		retVal = -1;
	}

	if (waitOnEnd){
		QApplication a(argc, argv);

		QMessageBox::information(NULL, "Wait on end", "Application is finished");
	}

	return retVal;
}

