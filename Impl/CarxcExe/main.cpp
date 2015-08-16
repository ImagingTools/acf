// STL includes
#include <iostream>

// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

// ACF includes
#include "iattr/TAttribute.h"
#include "ifile/IFilePersistence.h"

// ACF packages includes
#include "GeneratedFiles/Carxc/CCarxc.h"


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QString configFile;
	bool verboseEnabled = false;
	int workingMode = 0;
	bool useBinaryCode = true;
	bool showUsage = false;
	QString outputFilePath;
	QString inputFilePath;

	if (argc >= 2){
		inputFilePath = argv[1];

		for (int index = 1; index < argc; index++){
			QByteArray argument = argv[index];
			if (!argument.isEmpty() && (argument[0] == '-')){
				QByteArray option = argument.mid(1);

				if (option == "v"){
					verboseEnabled = true;
				}
				else if ((option == "h") || (option == "help")){
					showUsage = true;
				}
				else if (option == "no_binary"){
					useBinaryCode = false;
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
				}
			}
		}
	}
	else{
		showUsage = true;
	}

	icomp::CRegistryElement mainElement;

	icomp::IRegistryElement::AttributeInfo* enableVerboseAttrPtr = mainElement.InsertAttributeInfo("EnableVerbose", iattr::CBooleanAttribute::GetTypeName());
	if (enableVerboseAttrPtr != NULL){
		enableVerboseAttrPtr->attributePtr.SetPtr(new iattr::CBooleanAttribute(verboseEnabled));
	}
	icomp::IRegistryElement::AttributeInfo* useBinaryAttrPtr = mainElement.InsertAttributeInfo("UseBinaryCode", iattr::CBooleanAttribute::GetTypeName());
	if (useBinaryAttrPtr != NULL){
		useBinaryAttrPtr->attributePtr.SetPtr(new iattr::CBooleanAttribute(useBinaryCode));
	}
	icomp::IRegistryElement::AttributeInfo* workingModeAttrPtr = mainElement.InsertAttributeInfo("WorkingMode", iattr::CIntegerAttribute::GetTypeName());
	if (workingModeAttrPtr != NULL){
		workingModeAttrPtr->attributePtr.SetPtr(new iattr::CIntegerAttribute(workingMode));
	}

	CCarxc compiler(&mainElement);

	if (showUsage){
		iser::IVersionInfo* versionInfoPtr = compiler.GetInterface<iser::IVersionInfo>();
		if (versionInfoPtr == NULL){
			std::cout << "No version info!" << std::endl;

			return 5;
		}

		quint32 version = 0;
		versionInfoPtr->GetVersionNumber(iser::IVersionInfo::AcfVersionId, version);
		QString versionText = versionInfoPtr->GetEncodedVersionName(iser::IVersionInfo::AcfVersionId, version).toLocal8Bit();

		std::cout << "Extended ACF registry compiler (version " << versionText.toStdString() << ")" << std::endl;
		std::cout << "It generates C++ code and/or dependencies for qmake" << std::endl << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "\tCarxc.exe [registryName] {options}" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "\t-mode [sources|depends]  - working mode - source generation or dependencies output, default is 'sources'" << std::endl;
		std::cout << "\t-o outputFile            - output file path" << std::endl;
		std::cout << "\t-config configFile       - specify ACF packages configuration file" << std::endl;
		std::cout << "\t-sources [on|off]        - enables/disables C++ sources output, default is 'on'" << std::endl;
		std::cout << "\t-v                       - enable verbose mode" << std::endl;
		std::cout << "\t-no_binary               - disable generating of binary coded registries" << std::endl;
		std::cout << "\t-h or -help              - showing this help" << std::endl;

		return 0;
	}

	icomp::IPackagesManager* managerPtr = compiler.GetInterface<icomp::IPackagesManager>();
	if (managerPtr == NULL){
		std::cout << "No packages manager!" << std::endl;

		return 5;
	}

	ifile::IFilePersistence* inputPersistencePtr = compiler.GetInterface<ifile::IFilePersistence>("Load");
	if (inputPersistencePtr == NULL){
		std::cout << "No input loader!" << std::endl;

		return 5;
	}

	ifile::IFilePersistence* outputPersistencePtr = compiler.GetInterface<ifile::IFilePersistence>("Save");
	if (outputPersistencePtr == NULL){
		std::cout << "No output saver!" << std::endl;

		return 5;
	}

	if ((workingMode == 0) && outputFilePath.isEmpty()){
		std::cout << "Output file was not specified!" << std::endl;

		return 1;
	}

	managerPtr->LoadPackages(configFile);

	// registry model
	icomp::CRegistry registry;

	if (inputPersistencePtr->LoadFromFile(registry, inputFilePath) != ifile::IFilePersistence::OS_OK){
		std::cout << "Cannot read input registry file '" << inputFilePath.toLocal8Bit().constData() << "'" << std::endl;

		return 2;
	}

	if (outputPersistencePtr->SaveToFile(registry, outputFilePath) != ifile::IFilePersistence::OS_OK){
		std::cout << "Cannot write output file(s) '" << outputFilePath.toLocal8Bit().constData() << "'" << std::endl;

		return 3;
	}

	return 0;
}


