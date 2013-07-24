// STL includes
#include <iostream>

// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

// ACF includes
#include "istd/CSystem.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"
#include "icomp/TModelCompWrap.h"

// ACF packages includes
#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"
#include "FilePck/FilePck.h"
#include "PackagePck/PackagePck.h"


static void ShowUsage()
{
	std::cout << "ACF registry compiler" << std::endl;
	std::cout << "It generates C++ code and/or dependencies for qmake" << std::endl << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "\tArxc.exe [registryName] {options}" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t-mode [sources|depends]  - working mode - source generation or dependencies output, default is 'sources'" << std::endl;
	std::cout << "\t-o outputFile            - output file path" << std::endl;
	std::cout << "\t-config configFile       - specify ACF packages configuration file" << std::endl;
	std::cout << "\t-sources [on|off]        - enables/disables C++ sources output, default is 'on'" << std::endl;
	std::cout << "\t-v                       - enable verbose mode" << std::endl;
	std::cout << "\t-no_binary               - disable generating of binary coded registries" << std::endl;
	std::cout << "\t-h or -help              - showing this help" << std::endl;
}


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	if (argc < 2){
		ShowUsage();

		return 0;
	}

	QString configFile;
	bool verboseEnabled = false;
	int workingMode = 0;
	bool useBinaryCode = true;

	QString inputFilePath = argv[1];
	QString outputFilePath;

	for (int index = 2; index < argc; index++){
		QByteArray argument = argv[index];
		if (!argument.isEmpty() && (argument[0] == '-')){
			QByteArray option = argument.mid(1);

			if (option == "v"){
				verboseEnabled = true;
			}
			else if ((option == "h") || (option == "help")){
				ShowUsage();

				return 0;
			}
			else if (index < argc - 1){
				if (option == "config"){
					configFile = argv[++index];
				}
				else if ((option == "o") || (option == "output")){
					outputFilePath = QString::fromLocal8Bit(argv[index + 1]);
					++index;
				}
				else if (option == "mode"){
					QString modeText = argv[++index];
					if ((modeText == "d") || (modeText == "depends")){
						workingMode = 1;
					}
				}
				else if (option == "no_binary"){
					useBinaryCode = false;
				}
			}
		}
	}

	if (verboseEnabled){
		std::cout << "ARX Compiler started in: " << QDir::currentPath().toLocal8Bit().constData() << std::endl;

		istd::CSystem::EnvironmentVariables environmentVariables = istd::CSystem::GetEnvironmentVariables();

		for (		istd::CSystem::EnvironmentVariables::const_iterator index = environmentVariables.begin();
				index != environmentVariables.end();
				++index){
			std::cout << index.key().toLocal8Bit().constData() << " = " << index.value().toLocal8Bit().constData() << std::endl;
		}
	}

	if ((workingMode == 0) && outputFilePath.isEmpty()){
		std::cout << "Output file was not specified!" << std::endl;

		return 1;
	}

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "ARX Compiler");
	applicationInfo.SetStringAttr("CompanyName", "ImagingTools");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<BasePck::ConsoleLog> log;
	log.InitComponent();

	icomp::TSimComponentWrap<BasePck::XmlFileSerializer> registryLoaderComp;
	registryLoaderComp.SetRef("Log", &log);
	registryLoaderComp.SetRef("VersionInfo", &applicationInfo);
	registryLoaderComp.SetBoolAttr("EnableVerbose", verboseEnabled);
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<PackagePck::RegistriesManager> registriesManagerComp;
	registriesManagerComp.SetRef("RegistryLoader", &registryLoaderComp);
	registriesManagerComp.SetRef("Log", &log);
	registriesManagerComp.SetBoolAttr("EnableVerbose", verboseEnabled);
	registriesManagerComp.InitComponent();

	registriesManagerComp.LoadPackages(configFile);

	icomp::TSimComponentWrap<PackagePck::RegistryCodeSaver> codeSaverComp;
	codeSaverComp.SetRef("Log", &log);
	codeSaverComp.SetRef("PackagesManager", &registriesManagerComp);
	codeSaverComp.SetRef("RegistriesManager", &registriesManagerComp);
	codeSaverComp.SetIntAttr("WorkingMode", workingMode);
	codeSaverComp.SetBoolAttr("UseBinaryCode", useBinaryCode);
	codeSaverComp.InitComponent();

	// registry model
	icomp::TSimComponentWrap<PackagePck::Registry> registryComp;
	registryComp.InitComponent();

	if (registryLoaderComp.LoadFromFile(registryComp, inputFilePath) != ifile::IFilePersistence::OS_OK){
		std::cout << "Cannot read input registry file '" << inputFilePath.toLocal8Bit().constData() << "'" << std::endl;

		return 2;
	}

	if (codeSaverComp.SaveToFile(registryComp, outputFilePath) != ifile::IFilePersistence::OS_OK){
		std::cout << "Cannot write output file(s) '" << outputFilePath.toLocal8Bit().constData() << "'" << std::endl;

		return 3;
	}

	return 0;
}


