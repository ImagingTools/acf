#include "ibase/CExternalFileConverterComp.h"


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CExternalFileConverterComp::CopyFile(
			const istd::CString& inputFilePath,
			const istd::CString& outputFilePath,
			const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!m_applicationCompPtr.IsValid()){
		return false;
	}

	istd::CStringList arguments;

	// insert default application path argument:
	arguments.push_back(istd::CString::GetEmpty());

	// setup command line arguments:
	if (!m_applicationArgumentsAttrPtr.IsValid()){
		arguments.push_back(inputFilePath);
		arguments.push_back(outputFilePath);
	}
	else{
		istd::CString applicationArguments = *m_applicationArgumentsAttrPtr;

		applicationArguments.Replace("%1", inputFilePath);
		applicationArguments.Replace("%2", outputFilePath);

		arguments = applicationArguments.Split(" ", false);
	}

	int argc = arguments.size();

	istd::TDelPtr<char*> argv(new char*[argc]);
	for (int argIndex = 0; argIndex < argc; argIndex++){
		argv.GetPtr()[argIndex] = const_cast<char*>(arguments[argIndex].ToString().c_str());		
	}

	return (m_applicationCompPtr->Execute(argc, argv.GetPtr()) == 0);
}


} // namespace ibase


