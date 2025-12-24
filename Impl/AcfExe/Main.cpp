// Qt includes
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QDir>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#endif

// ACF includes
#include <ipackage/CComponentAccessor.h>
#include <ibase/IApplication.h>


#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
	#define QT_ENDL Qt::endl
#else
	#define QT_ENDL endl
#endif


int main(int argc, char *argv[])
{
	QString configFilePath;

	QString registryFilePath = "default.acc";
	bool showApplicationInfo = false;
	QByteArray componentId;
	bool waitOnEnd = false;
	bool isVerboseEnabled = false;
	bool isConsoleMode = false;
	QString environment;

	QTextStream out(stdout);

	for (int index = 1; index < argc; index++){
		QByteArray argument = argv[index];
		if (argument.startsWith('-')){
			QByteArray option = argument.mid(1);

			if ((option == "h") || (option == "help")){
				out << "Usage";
				out << "\tAcf.exe [registryName] {options}      - normal registry start" << QT_ENDL;
				out << "\t-h or -help              - showing help" << QT_ENDL;
				out << "\t-id componentId          - specify component ID." << QT_ENDL;
				out << "\t-config configFile       - load config file" << QT_ENDL;
				out << "\t-info                    - application parameter info" << QT_ENDL;
				out << "\t-wait                    - wait on application end" << QT_ENDL;
				out << "\t-v                       - enable verbose messages" << QT_ENDL;
				out << "\t-console                 - optimize the application for console mode" << QT_ENDL;
				out << "\t-env_vars environment variables - list of external environment variables" << QT_ENDL;
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
			else if (option == "env_vars") {
				environment = argv[++index];
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

	if (!environment.isEmpty()){
		QStringList environmentVars = environment.split(";");
		for (int varIndex = 0; varIndex < environmentVars.count(); ++varIndex){
			QString variableItem = environmentVars[varIndex];
			QStringList keyValue = variableItem.split("=");
			if (keyValue.count() == 2){
				QString variableName = keyValue[0];
				QString variableValue = keyValue[1];

				qputenv(variableName.toStdString().c_str(), variableValue.toUtf8());
			}
		}
	}

	int exitCode = 0;

	istd::TDelPtr<QCoreApplication> coreAppPtr;

	if (isConsoleMode){
		coreAppPtr.SetPtr(new QCoreApplication(argc, argv));
	}

	// Save current working directory:
	QString currentWorkingDirectory = QDir::currentPath();

	ipackage::CComponentAccessor componentAccessor(registryFilePath, configFilePath, isVerboseEnabled, true);

	// Restore current working directory before executing the application:
	if (currentWorkingDirectory != QDir::currentPath()){
		out << "The working directory of the process has been changed during loading of the component packages!" << QT_ENDL;
	}

	QDir::setCurrent(currentWorkingDirectory);

	ibase::IApplication* applicationPtr = componentAccessor.GetComponentInterface<ibase::IApplication>(componentId);
	if (applicationPtr == NULL){
		out << "Application interface cannot be found" << QT_ENDL;

		exitCode = -1;
	}
	else if (showApplicationInfo){
		out << applicationPtr->GetHelpText().toLocal8Bit().constData();

		exitCode = 0;
	}
	else{
		applicationPtr->InitializeApplication(argc, argv);

		componentAccessor.EnsureAutoInitComponentsCreated();

		exitCode = applicationPtr->Execute(argc, argv);
	}

	if (applicationPtr != NULL){
		if (waitOnEnd){
			QApplication a(argc, argv);

			QMessageBox::information(NULL, "Wait on end", "Application is finished");
		}
	}

	return exitCode;
}


