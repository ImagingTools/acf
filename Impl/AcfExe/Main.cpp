// Qt includes
#include <QtCore/QString>
#include <QtCore/QTextStream>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#endif

// ACF includes
#include "ipackage/CComponentAccessor.h"
#include "ibase/IApplication.h"


int main(int argc, char *argv[])
{
	QString configFilePath;

	QString registryFilePath = "default.acc";
	bool showApplicationInfo = false;
	QByteArray componentId;
	bool waitOnEnd = false;
	bool isVerboseEnabled = false;
	bool isConsoleMode = false;

	QTextStream out(stdout);

	for (int index = 1; index < argc; index++){
		QByteArray argument = argv[index];
		if (argument.startsWith('-')){
			QByteArray option = argument.mid(1);

			if ((option == "h") || (option == "help")){
				out << "Usage";
				out << "\tAcf.exe [registryName] {options}      - normal registry start" << endl;
				out << "\t-h or -help              - showing help" << endl;
				out << "\t-id componentId          - specify component ID." << endl;
				out << "\t-config configFile       - load config file" << endl;
				out << "\t-info                    - application parameter info" << endl;
				out << "\t-wait                    - wait on application end" << endl;
				out << "\t-v                       - enable verbose messages" << endl;
				out << "\t-console                 - optimize the application for console mode" << endl;

				return 0;
			}
			else if (option == "info"){
				showApplicationInfo = true;

				break;
			}
			else if (option == "wait"){
				waitOnEnd = true;
			}
			else if (option == "v"){
				isVerboseEnabled = true;
			}
			else if (option == "console"){
				isConsoleMode = true;
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

	istd::TDelPtr<QCoreApplication> coreAppPtr;

	if (isConsoleMode){
		coreAppPtr.SetPtr(new QCoreApplication(argc, argv));
	}

	ipackage::CComponentAccessor componentAccessor(registryFilePath, configFilePath, isVerboseEnabled);

	ibase::IApplication* applicationPtr = componentAccessor.GetComponentInterface<ibase::IApplication>(componentId);
	if (applicationPtr == NULL){
		out << "Application interface cannot be found" << endl;

		retVal = -1;
	}
	else if (showApplicationInfo){
		out << applicationPtr->GetHelpText().toLocal8Bit().constData();

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

