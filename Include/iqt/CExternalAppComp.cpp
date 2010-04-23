#include "iqt/CExternalAppComp.h"


// Qt includes
#include <QProcess>


namespace iqt
{


// public methods

CExternalAppComp::CExternalAppComp()
{
	connect(&m_applicationProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(OnError(QProcess::ProcessError)));
	connect(&m_applicationProcess, SIGNAL(readyReadStandardError()), this, SLOT(OnReadyReadStandardError()));
	connect(&m_applicationProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadyReadStandardOutput()));
}


// reimplemented (ibase::IApplication)

bool CExternalAppComp::InitializeApplication(int /*argc*/, char** /*argv*/)
{
	return true;
}


int CExternalAppComp::Execute(int argc, char** argv)
{
	if (!m_applicationPathCompPtr.IsValid()){
		SendErrorMessage(0, "Application path was not set.");
	
		return -1;
	}

	if (m_applicationPathCompPtr->GetPathType() != iprm::IFileNameParam::PT_FILE){
		SendErrorMessage(0, "Application path has wrong type. It must be a file path");

		return -1;
	}

	QString applicationPath = iqt::GetQString(m_applicationPathCompPtr->GetPath());
	if (applicationPath.isEmpty()){
		SendErrorMessage(0, "Application path is empty");

		return -1;
	}

	QStringList arguments;
	arguments.append(applicationPath);

	for (int argIndex = 1; argIndex < argc; argIndex++){
		arguments.append(argv[argc]);
	}
	
	m_applicationProcess.start(applicationPath, arguments);
	m_applicationProcess.waitForFinished(-1);

	return m_applicationProcess.exitCode();
}


istd::CString CExternalAppComp::GetHelpText() const
{
	return istd::CString::GetEmpty();
}


// protected slots:

void CExternalAppComp::OnError(QProcess::ProcessError error)
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


void CExternalAppComp::OnReadyReadStandardError()
{
	istd::CString errorOutput = iqt::GetCString(m_applicationProcess.readAllStandardError());

	SendErrorMessage(0, errorOutput);
}


void CExternalAppComp::OnReadyReadStandardOutput()
{
	istd::CString infoOutput = iqt::GetCString(m_applicationProcess.readAllStandardOutput());

	SendInfoMessage(0, infoOutput);
}


} // namespace iqt


