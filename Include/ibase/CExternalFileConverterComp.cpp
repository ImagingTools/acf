#include "ibase/CExternalFileConverterComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

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

	QFileInfo outputFileInfo(outputFilePath);

	QString outputDirectory = outputFileInfo.dir().absolutePath();

	for (int argIndex = 0; argIndex < int(arguments.size()); argIndex++){
		arguments[argIndex].replace("$(Input)", inputFilePath);
		arguments[argIndex].replace("$(Output)", outputFilePath);
		arguments[argIndex].replace("$(OutputDir)", outputDirectory);
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
					QString expressionString = QString("$(A%1)").arg(addIndex + 1);

					iprm::INameParam* additionalArgumentPtr = m_additionalArgumentsCompPtr[addIndex];
					if (additionalArgumentPtr != NULL){
						QString replacement = additionalArgumentPtr->GetName();

						if (existingArgument.contains(expressionString)){
							existingArgument.replace(expressionString, replacement);
							
							placeHolderFound = true;
						}
					}
				}

				if (!placeHolderFound){
					arguments.push_back(argument);
				}
			}
		}
	}

	SendVerboseMessage(QString("Process started as %1 %2").arg(m_executablePathCompPtr->GetPath()).arg(arguments.join(" ")));

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


