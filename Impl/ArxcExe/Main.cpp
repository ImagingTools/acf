// STL includes
#include <iostream>


// Qt includes
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>


// ACF includes
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"
#include "icomp/TModelCompWrap.h"

#include "iqt/CDefaultServicesProvider.h"
#include "iqt/CProcessEnvironment.h"


// ACF packages includes
#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"
#include "CmpstrPck/CmpstrPck.h"


static void ShowUsage()
{
	std::cout << "Usage";
	std::cout << "\tArxc.exe [registryName] {options}      - convertion registry to C++ code" << std::endl;
	std::cout << "\t-h or -help				 - showing this help" << std::endl;
	std::cout << "\t-o outputFile            - output file path" << std::endl;
	std::cout << "\t-config configFile       - specify ACF packages configuration file" << std::endl;
	std::cout << "\t-v			 - enable verbose mode" << std::endl;
}


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	iqt::CDefaultServicesProvider::RegisterServices();

	if (argc < 2){
		ShowUsage();

		return 0;
	}

	istd::CString configFile;
	bool verboseEnabled = false;

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
			}
		}
	}

	if (verboseEnabled){
		std::cout << "ARX Compiler started in: " + QDir::currentPath().toStdString() << std::endl;
		iqt::CProcessEnvironment processEnvironment;

		iqt::CProcessEnvironment::EnvironmentVariables environmentVariables = processEnvironment.GetEnvironmentVariables();

		for (		iqt::CProcessEnvironment::EnvironmentVariables::const_iterator index = environmentVariables.begin();
					index != environmentVariables.end();
					++index){
			std::cout << index->first.ToString() << " = " << index->second.ToString() << std::endl;
		}
	}

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetStringAttr("ApplicationName", "ARX Compiler");
	applicationInfo.SetStringAttr("CompanyName", "ImagingTools");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<BasePck::ConsoleLog> log;
	log.InitComponent();

	icomp::TSimComponentWrap<CmpstrPck::RegistryLoader> registryLoaderComp;
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

	icomp::TSimComponentWrap<BasePck::RegistryCodeSaver> codeSaverComp;
	codeSaverComp.SetRef("Log", &log);
	codeSaverComp.SetRef("PackagesManager", &registriesManagerComp);
	codeSaverComp.SetRef("RegistriesManager", &registriesManagerComp);
	codeSaverComp.InitComponent();

	// registry model
	icomp::TSimComponentWrap<CmpstrPck::Registry> registryComp;
	registryComp.InitComponent();

	icomp::TSimComponentWrap<BasePck::FileCopyOverLoader> fileCopyComp;
	fileCopyComp.SetRef("Object", &registryComp);
	fileCopyComp.SetRef("InputLoader", &registryLoaderComp);
	fileCopyComp.SetRef("OutputLoader", &codeSaverComp);
	fileCopyComp.InitComponent();

	icomp::TSimComponentWrap<BasePck::CopyApp> applicationComp;
	applicationComp.SetRef("FileCopy", &fileCopyComp);
	applicationComp.InitComponent();

	return applicationComp.Execute(argc, argv);
}


