#include <iostream>

#include <QString>

#include "ibase/IApplication.h"

#include "icomp/CXmlRegistriesManager.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CCompositeComponent.h"
#include "icomp/CRegistryElement.h"

#include "iqt/CPackagesLoader.h"


int main(int argc, char *argv[])
{
	iqt::CPackagesLoader loader;

	::std::string registryFile = "default.acfr";
	bool showApplicationInfo = false;
	bool useDefaultRegistries = true;
	::std::string componentId;

	for (int index = 1; index < argc; index++){
		::std::string argument = argv[index];
		if (!argument.empty() && (argument[0] == '-')){
			::std::string option = argument.substr(1);

			if ((option == "h") || (option == "help")){
				::std::cout << "Usage";
				::std::cout << "\tAcf.exe [registryName] {options}      - normal registry start" << ::std::endl;
				::std::cout << "\t-h or -help              - showing help" << ::std::endl;
				::std::cout << "\t-id componentId          - specify component ID." << ::std::endl;
				::std::cout << "\t-packageFile filePath    - append single package file" << ::std::endl;
				::std::cout << "\t-packageDir directory    - append packages directory" << ::std::endl;
				::std::cout << "\t-config configFile       - load config file" << ::std::endl;
				::std::cout << "\t-info                    - application parameter info" << ::std::endl;

				return 0;
			}
			else if (option == "info"){
				showApplicationInfo = true;

				break;
			}
			else if (index < argc - 1){
				if (option == "id"){
					componentId = argv[++index];
				}
				else if (option == "packageFile"){
					loader.RegisterPackageFile(argv[++index], false);

					useDefaultRegistries = false;
				}
				else if (option == "packageDir"){
					loader.RegisterPackagesDir(argv[++index], false);

					useDefaultRegistries = false;
				}
				else if (option == "config"){
					loader.LoadConfigFile(argv[++index]);

					useDefaultRegistries = false;
				}
			}
		}
		else if (index == 1){
			registryFile = argument;
		}
	}

	if (useDefaultRegistries){
		loader.RegisterPackagesDir("", false);
	}

	icomp::CXmlRegistriesManager registriesManager(&loader);
	const icomp::IRegistry* registryPtr = registriesManager.GetRegistryFromFile(registryFile.c_str());
	if (registryPtr == NULL){
		::std::cout << QString("Registry %1 cannot be loaded").arg(registryFile.c_str()).toStdString() << ::std::endl;

		return -1;
	}

	icomp::CRegistryElement dummyElement(&loader);

	icomp::CCompositeComponentContext compositeContext(&dummyElement, registryPtr, &registriesManager);
	icomp::CCompositeComponent composite(&compositeContext);

	ibase::IApplication* applicationPtr = (ibase::IApplication*)composite.GetInterface(typeid(ibase::IApplication), componentId);
	if (applicationPtr == NULL){
		::std::cout << "Application interface cannot be found" << ::std::endl;

		return -1;
	}

	if (showApplicationInfo){
		::std::cout << applicationPtr->GetHelpText().ToString();

		return 0;
	}
	else{
		return applicationPtr->Execute(argc, argv);
	}
}
