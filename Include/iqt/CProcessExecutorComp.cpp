#include "iqt/CProcessExecutorComp.h"


// Qt includes
#include <QProcess>
#include <QMetaType>


namespace iqt
{


// public methods

CProcessExecutorComp::CProcessExecutorComp()
{
	qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");

	connect(&m_applicationProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(OnError(QProcess::ProcessError)));
	connect(&m_applicationProcess, SIGNAL(readyReadStandardError()), this, SLOT(OnReadyReadStandardError()));
	connect(&m_applicationProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadyReadStandardOutput()));
}


// reimplemented (ibase::IProcessExecutor)
	
void CProcessExecutorComp::SetEnvironment(const isys::IApplicationEnvironment& processEnvironment)
{
	if (m_applicationProcess.state() != QProcess::NotRunning){
		SendErrorMessage(0, "Process already running. Environment cannot be set.");

		return;
	}

	isys::IApplicationEnvironment::EnvironmentVariables environmentVariables = processEnvironment.GetEnvironmentVariables();

	QProcessEnvironment qtProcessEvironment;

	for (		isys::IApplicationEnvironment::EnvironmentVariables::const_iterator index = environmentVariables.begin();
				index != environmentVariables.end();
				index++){
		qtProcessEvironment.insert(iqt::GetQString(index->first), iqt::GetQString(index->second));
	}

	m_applicationProcess.setProcessEnvironment(qtProcessEvironment);

	m_applicationProcess.setWorkingDirectory(iqt::GetQString(processEnvironment.GetWorkingDirectory()));
}


int CProcessExecutorComp::Execute(const istd::CString& executablePath, const istd::CStringList& processArguments)
{
	m_applicationProcess.start(iqt::GetQString(executablePath), iqt::GetQStringList(processArguments));

	m_applicationProcess.waitForFinished(-1);

	return m_applicationProcess.exitCode();
}


// reimplemented (icomp::IComponent)

void CProcessExecutorComp::OnComponentDestroyed()
{
	m_applicationProcess.kill();

	BaseClass::OnComponentDestroyed();
}


// protected slots:

void CProcessExecutorComp::OnError(QProcess::ProcessError error)
{   
	switch (error){
		case QProcess::FailedToStart:
			SendErrorMessage(0, "Process could not be started.");
			break;
		case QProcess::Crashed:
			SendErrorMessage(0, "The process crashed during execution.");
			break;
		case QProcess::ReadError:
			SendErrorMessage(0, "An error occurred when attempting to read from the process.");
			break;
		case QProcess::WriteError:
			SendErrorMessage(0, "An error occurred when attempting to write to the process.");
			break;
		case QProcess::UnknownError:
		default:
			SendErrorMessage(0, "An unknown error is occured.");
	}
}


void CProcessExecutorComp::OnReadyReadStandardError()
{
	istd::CString errorOutput = iqt::GetCString(m_applicationProcess.readAllStandardError());

	SendErrorMessage(0, errorOutput);
}


void CProcessExecutorComp::OnReadyReadStandardOutput()
{
	istd::CString infoOutput = iqt::GetCString(m_applicationProcess.readAllStandardOutput());

	SendInfoMessage(0, infoOutput);
}


} // namespace iqt


