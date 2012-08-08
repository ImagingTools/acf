// STL includes
#include <iostream>


// Qt includes
#include <QtCore/QString>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>


// ACF includes
#include "iqt/CComponentAccessor.h"

#include "ibase/IApplication.h"


int main(int argc, char *argv[])
{
	QApplication::setStyle("plastique");

	QString configFilePath;

	QString registryFilePath = "default.arx";
	bool showApplicationInfo = false;
	QByteArray componentId;
	bool waitOnEnd = false;

	for (int index = 1; index < argc; index++){
		QByteArray argument = argv[index];
		if (argument.startsWith('-')){
			QByteArray option = argument.mid(1);

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
					configFilePath = QString::fromLocal8Bit(argv[++index]);
				}
			}
		}
		else if (index == 1){
			registryFilePath = QString::fromLocal8Bit(argument);
		}
	}

	int retVal = 0;

	iqt::CComponentAccessor componentAccessor(registryFilePath, configFilePath);

	ibase::IApplication* applicationPtr = componentAccessor.GetComponentInterface<ibase::IApplication>(componentId);
	if (applicationPtr == NULL){
		std::cout << "Application interface cannot be found" << std::endl;

		retVal = -1;
	}
	
	else if (showApplicationInfo){
		std::cout << applicationPtr->GetHelpText().toLocal8Bit().constData();

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

