#include "ibase/CExternalFileConverterComp.h"


// Qt includes
#include <QStringList>


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CExternalFileConverterComp::ConvertFile(
			const QString& inputFilePath,
			const QString& outputFilePath,
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

	QStringList arguments;

	// setup command line arguments:
	if (!m_processArgumentsAttrPtr.IsValid()){
		arguments.push_back(inputFilePath);
		arguments.push_back(outputFilePath);
	}
	else{
		QString applicationArguments = *m_processArgumentsAttrPtr;

		arguments = applicationArguments.split(" ");

		for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
			arguments[argIndex].replace("$(Input)", inputFilePath);
			arguments[argIndex].replace("$(Output)", outputFilePath);
		}
	}

	return (m_processExecuterCompPtr->ExecuteProcess(m_executablePathCompPtr->GetPath(), arguments) == 0);
}


} // namespace ibase


