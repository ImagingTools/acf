#include "ibase/CExternalFileConverterComp.h"


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CExternalFileConverterComp::CopyFile(
			const istd::CString& inputFilePath,
			const istd::CString& outputFilePath,
			const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!m_executablePathCompPtr.IsValid()){
		SendErrorMessage(0, "Path for an executable was not set");

		return false;
	}

	if (m_executablePathCompPtr->GetPathType() != iprm::IFileNameParam::PT_FILE){
		SendErrorMessage(0, "Wrong executable path type. Must be path to a file.");

		return false;
	}

	if (!m_processExecuterCompPtr.IsValid()){
		SendErrorMessage(0, "Process execution component was not set.");

		return false;
	}

	istd::CStringList arguments;

	// setup command line arguments:
	if (!m_processArgumentsAttrPtr.IsValid()){
		arguments.push_back(inputFilePath);
		arguments.push_back(outputFilePath);
	}
	else{
		istd::CString applicationArguments = *m_processArgumentsAttrPtr;

		arguments = applicationArguments.Split(" ", false);

		for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
			arguments[argIndex].Replace("$(Input)", inputFilePath);
			arguments[argIndex].Replace("$(Output)", outputFilePath);
		}
	}

	return (m_processExecuterCompPtr->Execute(m_executablePathCompPtr->GetPath(), arguments) == 0);
}


} // namespace ibase


