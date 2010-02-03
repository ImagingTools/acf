#include "icmpstr/CRegistryPreviewComp.h"


// Qt includes
#include <QTemporaryFile>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMetaType>

#include "istd/TChangeNotifier.h"

#include "iser/CXmlFileWriteArchive.h"

#include "iqt/CFileSystem.h"


Q_DECLARE_METATYPE(QProcess::ProcessState);


namespace icmpstr
{


// public methods

CRegistryPreviewComp::CRegistryPreviewComp()
:	m_isRunning(false)
{
}


// reimplemented (icomp::IComponent)

void CRegistryPreviewComp::OnComponentCreated()
{
	qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
	qRegisterMetaType<QProcess::ProcessState>();

	BaseClass::OnComponentCreated();

	m_tempFileName.clear();

	QDir tempDir = QDir::temp();
	if (tempDir.exists()){
		m_tempFileName = tempDir.absoluteFilePath("registry_preview.arx");
	}

	m_isRunning = false;

	connect(	&m_process, 
				SIGNAL(stateChanged(QProcess::ProcessState)), 
				this, 
				SLOT(OnStateChanged(QProcess::ProcessState)), 
				Qt::QueuedConnection);
}


void CRegistryPreviewComp::OnComponentDestroyed()
{
	if (IsRunning()){
		m_process.kill();

		m_process.waitForFinished(5000);
	}

	QFile::remove(m_tempFileName);

	BaseClass::OnComponentDestroyed();
}


// reimplemented (icmpstr::IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
	if (IsRunning() || m_tempFileName.isEmpty() || !m_commandFileNameCompPtr.IsValid()){
		return false;
	}

	iser::CXmlFileWriteArchive archive(m_tempFileName.toStdString(), m_versionInfoCompPtr.GetPtr());

	if (!(const_cast<icomp::IRegistry&>(registry)).Serialize(archive)){
		return false;
	}

	QString acfExeFile = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(m_commandFileNameCompPtr->GetPath()));

	QDir applicationDir(QCoreApplication::applicationDirPath());
	QString acfApplicationPath = applicationDir.absoluteFilePath(acfExeFile);

	m_process.setWorkingDirectory(applicationDir.path());

	QStringList parameters;
	parameters << m_tempFileName;

	if (m_envManagerCompPtr.IsValid()){
		istd::CString configFilePath = m_envManagerCompPtr->GetConfigFilePath();
		if (!configFilePath.IsEmpty()){
			parameters << "-config";
			parameters << iqt::GetQString(configFilePath);
		}
	}

	m_process.start(acfApplicationPath, parameters);

	return m_process.waitForStarted();
}


bool CRegistryPreviewComp::IsRunning() const
{
	return m_isRunning;
}


void CRegistryPreviewComp::AbortRegistry()
{
	m_process.terminate();
	if (!m_process.waitForFinished(5000)){
		m_process.kill();
	}
}


// protected slots

void CRegistryPreviewComp::OnStateChanged(QProcess::ProcessState state)
{
	istd::CChangeNotifier notifier(this);

	m_isRunning = (state == QProcess::Running);
}


} // namespace icmpstr


