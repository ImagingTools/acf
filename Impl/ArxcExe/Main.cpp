// STL includes
#include <iostream>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>

// ACF includes
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"
#include "icomp/TModelCompWrap.h"
#include "iqt/CSystem.h"

// ACF packages includes
#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"
#include "CompositorPck/CompositorPck.h"


static void ShowUsage()
{
	std::cout << "ACF registry compiler" << std::endl;
	std::cout << "It generates C++ code and/or dependencies for qmake" << std::endl << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "\tArxc.exe [registryName] {options}" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t-h or -help              - showing this help" << std::endl;
	std::cout << "\t-o outputFile            - output file path" << std::endl;
	std::cout << "\t-config configFile       - specify ACF packages configuration file" << std::endl;
	std::cout << "\t-sources [on|off]        - enables/disables C++ sources output, default is 'on'" << std::endl;
	std::cout << "\t-depends [on|off]        - enables/disables qmake dependendency output, default is 'off'" << std::endl;
	std::cout << "\t-dependsPath path        - base path for dependencies, if not specified working directory will be taken" << std::endl;
	std::cout << "\t-v                       - enable verbose mode" << std::endl;
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
	bool sourcesEnabled = true;
	bool depandenciesEnabled = false;
	QString baseDependsPath;

	for (int index = 1; index < argc; index++){
		std::string argument = argv[index];
		if (!argument.empty() && (argument[0] == '-')){
			std::string option = argument.substr(1);

			if (option == "v"){
				verboseEnabled = true;
			}

			if ((option == "h") || (option == "help")){
				ShowUsage();

				return 0;
			}
			else if (index < argc - 1){
				if (option == "config"){
					configFile = argv[++index];
				}

				if (option == "sources"){
					std::string switchText = argv[++index];
					sourcesEnabled = (switchText == "on") || (switchText == "ON");
				}

				if (option == "depends"){
					std::string switchText = argv[++index];
					depandenciesEnabled = (switchText == "on") || (switchText == "ON");
				}

				if (option == "dependsPath"){
					baseDependsPath = argv[++index];
				}
			}
		}
	}

	if (verboseEnabled){
		std::cout << "ARX Compiler started in: " + QDir::currentPath().toStdString() << std::endl;

		iqt::CSystem::EnvironmentVariables environmentVariables = iqt::CSystem::GetEnvironmentVariables();

		for (		iqt::CSystem::EnvironmentVariables::const_iterator index = environmentVariables.begin();
					index != environmentVariables.end();
					++index){
			std::cout << index.key().toStdString() << " = " << index.value().toStdString() << std::endl;
		}
	}

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "ARX Compiler");
	applicationInfo.SetStringAttr("CompanyName", "ImagingTools");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<BasePck::ConsoleLog> log;
	log.InitComponent();

	icomp::TSimComponentWrap<CompositorPck::RegistryLoader> registryLoaderComp;
	registryLoaderComp.SetRef("Log", &log);
	registryLoaderComp.SetRef("VersionInfo", &applicationInfo);
	registryLoaderComp.SetBoolAttr("EnableVerbose", verboseEnabled);
	registryLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::RegistriesManager> registriesManagerComp;
	registriesManagerComp.SetRef("RegistryLoader", &registryLoaderComp);
	registriesManagerComp.SetRef("Log", &log);
	registriesManagerComp.SetBoolAttr("EnableVerbose", verboseEnabled);
	registriesManagerComp.InitComponent();

	registriesManagerComp.LoadPackages(configFile);

	icomp::TSimComponentWrap<BasePck::FileNameParam> dependsBasePathComp;
	dependsBasePathComp.SetStringAttr("DefaultPath", baseDependsPath);
	dependsBasePathComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::RegistryCodeSaver> codeSaverComp;
	codeSaverComp.SetRef("Log", &log);
	codeSaverComp.SetRef("PackagesManager", &registriesManagerComp);
	codeSaverComp.SetRef("RegistriesManager", &registriesManagerComp);
	codeSaverComp.SetIntAttr("WorkingMode", (sourcesEnabled? 1: 0) + (depandenciesEnabled? 2: 0));
	codeSaverComp.SetRef("BaseDependenciesPath", &dependsBasePathComp);
	codeSaverComp.InitComponent();

	// registry model
	icomp::TSimComponentWrap<CompositorPck::Registry> registryComp;
	registryComp.InitComponent();

	icomp::TSimComponentWrap<BasePck::FileCopyOverLoader> fileCopyComp;
	fileCopyComp.SetRef("Object", &registryComp);
	fileCopyComp.SetRef("InputLoader", &registryLoaderComp);
	fileCopyComp.SetRef("OutputLoader", &codeSaverComp);
	fileCopyComp.SetRef("Log", &log);
	fileCopyComp.InitComponent();

	icomp::TSimComponentWrap<BasePck::CopyApp> applicationComp;
	applicationComp.SetRef("FileCopy", &fileCopyComp);
	applicationComp.SetRef("Log", &log);
	applicationComp.InitComponent();

	return applicationComp.Execute(argc, argv);
}


