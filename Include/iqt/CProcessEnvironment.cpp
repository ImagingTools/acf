#include "iqt/CProcessEnvironment.h"


// Qt includes
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <QThread>


namespace iqt
{


/*
	Helper implementation.
*/
class CQtThread: private QThread
{
public:
	static void Sleep(double seconds)
	{
		QThread::usleep(seconds * 1000000);
	}
};


// public methods
	
// reimplemented (isys::IProcessEnvironment)

int CProcessEnvironment::GetMainThreadId() const
{
	return *static_cast<int*>(QThread::currentThreadId());
}


void CProcessEnvironment::Sleep(double seconds)
{
	CQtThread::Sleep(seconds);
}


istd::CString CProcessEnvironment::GetTempDirPath() const
{
	return iqt::GetCString(QDir::tempPath());
}


istd::CString CProcessEnvironment::GetWorkingDirectory() const
{
	return iqt::GetCString(QDir::currentPath());
}


istd::CStringList CProcessEnvironment::GetApplicationArguments() const
{
	return iqt::GetCStringList(QCoreApplication::arguments());
}


istd::CString CProcessEnvironment::GetModulePath(bool /*useApplicationModule = false*/, bool onlyDirectory /*= false*/) const
{
	if (!onlyDirectory){
		return iqt::GetCString(QCoreApplication::applicationFilePath());
	}

	return iqt::GetCString(QCoreApplication::applicationDirPath());
}


CProcessEnvironment::EnvironmentVariables CProcessEnvironment::GetEnvironmentVariables() const
{
	QStringList processEnvironment = QProcess::systemEnvironment();
	EnvironmentVariables environmentVariables;

	for (int variableIndex = 0; variableIndex < int(processEnvironment.count()); variableIndex++){
		QString variableEntry = processEnvironment[variableIndex];
		QStringList splitted = variableEntry.split('=');

		if (splitted.count() == 2){
			istd::CString variableName = iqt::GetCString(splitted[0]);
			istd::CString variableValue = iqt::GetCString(splitted[1]);

			environmentVariables[variableName.ToUpper()] = variableValue;
		}
	}

	return environmentVariables;
}


void CProcessEnvironment::SetEnvironmentVariableValue(const istd::CString&/* variableName*/, const istd::CString&/* value*/)
{
	// not implemented yet.
	I_CRITICAL();
}


} // namespace iqt


