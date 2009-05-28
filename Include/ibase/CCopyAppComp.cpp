#include "ibase/CCopyAppComp.h"


namespace ibase
{


// reimplemented (ibase::IApplication)

int CCopyAppComp::Execute(int argc, char** argv)
{
	if (!m_objectCompPtr.IsValid()){
		return 1;
	}

	if (!m_inputLoaderCompPtr.IsValid()){
		return 2;
	}

	if (!m_outputLoaderCompPtr.IsValid()){
		return 3;
	}

	istd::CString inputFilePath;
	istd::CString outputFilePath;

	for (int index = 1; index < argc; ++index){
		std::string argument = argv[index];
		if (!argument.empty() && (argument[0] == '-')){
			std::string option = argument.substr(1);

			if (index < argc - 1){
				if (option == "o"){
					outputFilePath = argv[index + 1];
					++index;
				}
			}
		}
		else if (inputFilePath.IsEmpty()){
			inputFilePath = argument;
		}
	}

	if (outputFilePath.IsEmpty()){
		istd::CStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.empty()){
			return 10;
		}

		istd::CString::size_type pointPos = inputFilePath.rfind('.');

		if (pointPos != istd::CString::npos){
			outputFilePath = inputFilePath.substr(0, pointPos + 1) + extensions.front();
		}
		else{
			outputFilePath = inputFilePath + extensions.front();
		}
	}

	if (m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputFilePath) != iser::IFileLoader::StateOk){
		return 20;
	}

	if (m_outputLoaderCompPtr->SaveToFile(*m_objectCompPtr, outputFilePath) != iser::IFileLoader::StateOk){
		return 21;
	}

	return 0;
}


istd::CString CCopyAppComp::GetHelpText() const
{
	return "General ACF copy application. Usage: <application> InputFilePath [-o OutputFilePath]";
}


} // namespace ibase


