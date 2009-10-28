#include "iqt/CApplicationEnvironment.h"


// Qt includes
#include <QDir>
#include <QCoreApplication>
#include <QProcess>


namespace iqt
{


// public methods
	
// reimplemented (isys::IApplicationEnvironment)

istd::CString CApplicationEnvironment::GetTempDirPath() const
{
	return iqt::GetCString(QDir::tempPath());
}


istd::CString CApplicationEnvironment::GetWorkingDirectory() const
{
	return iqt::GetCString(QDir::currentPath());
}


istd::CStringList CApplicationEnvironment::GetApplicationArguments() const
{
	return iqt::GetCStringList(QCoreApplication::arguments());
}


istd::CString CApplicationEnvironment::GetModulePath(bool /*useApplicationModule = false*/, bool onlyDirectory /*= false*/) const
{
	if (!onlyDirectory){
		return iqt::GetCString(QCoreApplication::applicationFilePath());
	}

	return iqt::GetCString(QCoreApplication::applicationDirPath());
}


CApplicationEnvironment::EnvironmentVariables CApplicationEnvironment::GetEnvironmentVariables() const
{
	QStringList processEnvironment = QProcess::systemEnvironment();
	EnvironmentVariables environmentVariables;

	for (int variableIndex = 0; variableIndex < int(processEnvironment.count()); variableIndex++){
		QString variableEntry = processEnvironment[variableIndex];
		QStringList splitted = variableEntry.split('=');

		if (splitted.count() == 2){
			istd::CString variableName = iqt::GetCString(splitted[0]);
			istd::CString variableValue = iqt::GetCString(splitted[1]);

			environmentVariables[variableName] = variableValue;
		}
	}

	return environmentVariables;
}


void CApplicationEnvironment::SetEnvironmentVariableValue(const istd::CString&/* variableName*/, const istd::CString&/* value*/)
{
	// not implemented yet.
	I_CRITICAL();
}


} // namespace iqt


