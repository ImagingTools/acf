#include "ibase/CCopyAppComp.h"


namespace ibase
{


// reimplemented (ibase::IApplication)

bool CCopyAppComp::InitializeApplication(int /*argc*/, char** /*argv*/)
{
	return true;
}


int CCopyAppComp::Execute(int argc, char** argv)
{
	if (!m_fileCopyCompPtr.IsValid()){
		return 1;
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
				else if ((option == "input") && *m_needExplicitInputAttrPtr){
					inputFilePath = argv[index + 1];
					++index;
				}
			}
		}
		else if (!*m_needExplicitInputAttrPtr && (index == 1)){
			inputFilePath = argument;
		}
	}

	if (!m_fileCopyCompPtr->CopyFile(inputFilePath, outputFilePath)){
		return 20;
	}

	return 0;
}


istd::CString CCopyAppComp::GetHelpText() const
{
	return "General ACF copy application. Usage: <application> InputFilePath [-o OutputFilePath]";
}


} // namespace ibase


