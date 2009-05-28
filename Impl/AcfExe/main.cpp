// STL includes
#include <iostream>

// Qt includes
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QLabel>

// ACF includes
#include "icomp/TComponentWrap.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/CRegistryElement.h"
#include "icomp/CComponentAccessor.h"

#include "ibase/IApplication.h"

#include "iqt/CDefaultServicesProvider.h"


int main(int argc, char *argv[])
{
	QApplication::setStyle("plastique");

	iqt::CDefaultServicesProvider::RegisterServices();

	istd::CString configFile;

	std::string registryFile = "default.arx";
	bool showApplicationInfo = false;
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
				else if (option == "config"){
					configFile = argv[++index];
				}
			}
		}
		else if (index == 1){
			registryFile = argument;
		}
	}

	int retVal = 0;

	icomp::CComponentAccessor componentAccessor(registryFile, configFile);

	ibase::IApplication* applicationPtr = componentAccessor.GetComponentInterface<ibase::IApplication>(componentId);
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

	if (applicationPtr != NULL){
		if (waitOnEnd){
			QApplication a(argc, argv);

			QMessageBox::information(NULL, "Wait on end", "Application is finished");
		}
	}

	return retVal;
}

