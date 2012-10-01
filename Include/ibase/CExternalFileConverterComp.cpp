#include "ibase/CExternalFileConverterComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QProcess>


// ACF includes
#include "iprm/TParamsPtr.h"


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CExternalFileConverterComp::ConvertFile(
			const QString& inputFilePath,
			const QString& outputFilePath,
			const iprm::IParamsSet* paramsSetPtr) const
{
	if (!m_executablePathCompPtr.IsValid()){
		SendErrorMessage(0, "Path for an executable was not set");

		return false;
	}

	if (m_executablePathCompPtr->GetPathType() != iprm::IFileNameParam::PT_FILE){
		SendErrorMessage(0, "Wrong executable path type. Must be path to a file.");

		return false;
	}

	QStringList arguments;

	// setup command line arguments:
	if (!m_defaultProcessArgumentsAttrPtr.IsValid()){
		if (paramsSetPtr != NULL){
			iprm::TParamsPtr<iprm::INameParam> commandLineParamPtr(paramsSetPtr, *m_processArgumentsParamsIdAttrPtr);
			if (commandLineParamPtr.IsValid()){
				arguments = commandLineParamPtr->GetName().split(" ");
			}
		}
		else{
			arguments.push_back(inputFilePath);
			arguments.push_back(outputFilePath);
		}
	}
	else{
		QString applicationArguments = *m_defaultProcessArgumentsAttrPtr;

		arguments = applicationArguments.split(" ");
	}

	for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
		arguments[argIndex].replace("$(Input)", inputFilePath);
		arguments[argIndex].replace("$(Output)", outputFilePath);
	}

	if (m_additionalArgumentsCompPtr.IsValid()){
		int additionalArgumentsCount = m_additionalArgumentsCompPtr.GetCount();

		for (int addIndex = 0; addIndex < additionalArgumentsCount; addIndex++){
			iprm::INameParam* argumentPtr = m_additionalArgumentsCompPtr[addIndex];
			if (argumentPtr != NULL){
				QString argument = argumentPtr->GetName();

				bool placeHolderFound = false;
				for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
					QString& existingArgument = arguments[argIndex];
					QRegExp additionArgumentExpression("\\$\\(A[0-9]");
					if (existingArgument.contains(additionArgumentExpression)){
						int numberIndex = existingArgument.indexOf(QRegExp("[0-9]"));
						int bracketIndex = existingArgument.indexOf(")");
						QString numberString = existingArgument.mid(numberIndex, bracketIndex - numberIndex);
						int argumentNumber = numberString.toInt() - 1;
						if (argumentNumber >= 0){
							placeHolderFound = true;

							if (argumentNumber < additionalArgumentsCount){
								iprm::INameParam* addArgumentPtr = m_additionalArgumentsCompPtr[argumentNumber];
								if (addArgumentPtr != NULL){
									existingArgument = addArgumentPtr->GetName();
								}
							}
							else{
								SendVerboseMessage("Addition command line argument doesn't exist");
							}
						}
					}
				}

				if (!placeHolderFound){
					arguments.push_back(argument);
				}
			}
		}
	}

	m_conversionProcess.start(m_executablePathCompPtr->GetPath(), arguments);

	m_conversionProcess.waitForFinished(-1);

	if (m_conversionProcess.error() != QProcess::UnknownError){
		return false;
	}

	return (m_conversionProcess.exitCode() == 0);
}


// protected methods

// reimplemented (icomp::CComponentBase)
	
void CExternalFileConverterComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(&m_conversionProcess, SIGNAL(readyReadStandardError()), this, SLOT(OnReadyReadStandardError()));
	connect(&m_conversionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadyReadStandardOutput()));
}


// private slots

void CExternalFileConverterComp::OnReadyReadStandardError()
{
	QString errorOutput = m_conversionProcess.readAllStandardError();
	
	errorOutput = errorOutput.simplified();

	SendErrorMessage(0, errorOutput);
}


void CExternalFileConverterComp::OnReadyReadStandardOutput()
{
	QString standardOutput = m_conversionProcess.readAllStandardOutput();

	standardOutput = standardOutput.simplified();

	SendInfoMessage(0, standardOutput);
}



} // namespace ibase


