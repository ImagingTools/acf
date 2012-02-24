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
#if defined(Q_WS_X11)
	return int(QThread::currentThreadId());
#else
	return *static_cast<int*>(QThread::currentThreadId());
#endif
}


void CProcessEnvironment::Sleep(double seconds)
{
	CQtThread::Sleep(seconds);
}


QString CProcessEnvironment::GetTempDirPath() const
{
	return QDir::tempPath();
}


QString CProcessEnvironment::GetWorkingDirectory() const
{
	return QDir::currentPath();
}


QStringList CProcessEnvironment::GetApplicationArguments() const
{
	return QCoreApplication::arguments();
}


QString CProcessEnvironment::GetModulePath(bool /*useApplicationModule = false*/, bool onlyDirectory /*= false*/) const
{
	if (!onlyDirectory){
		return QCoreApplication::applicationFilePath();
	}

	return QCoreApplication::applicationDirPath();
}


CProcessEnvironment::EnvironmentVariables CProcessEnvironment::GetEnvironmentVariables() const
{
	QStringList processEnvironment = QProcess::systemEnvironment();
	EnvironmentVariables environmentVariables;

	for (int variableIndex = 0; variableIndex < int(processEnvironment.count()); variableIndex++){
		QString variableEntry = processEnvironment[variableIndex];
		QStringList splitted = variableEntry.split('=');

		if (splitted.count() == 2){
			QString variableName = splitted[0];
			QString variableValue = splitted[1];

			environmentVariables[variableName.toUpper()] = variableValue;
		}
	}

	return environmentVariables;
}


void CProcessEnvironment::SetEnvironmentVariableValue(const QString&/* variableName*/, const QString&/* value*/)
{
	// not implemented yet.
	I_CRITICAL();
}


} // namespace iqt


