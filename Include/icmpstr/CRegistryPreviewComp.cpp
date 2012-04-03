#include "icmpstr/CRegistryPreviewComp.h"


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMetaType>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopWidget>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/CXmlFileWriteArchive.h"

#include "iqt/CSystem.h"


Q_DECLARE_METATYPE(QProcess::ProcessState);


namespace icmpstr
{


// public methods

CRegistryPreviewComp::CRegistryPreviewComp()
:	m_isRunning(false)
{
}


// reimplemented (icmpstr::IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
	if (IsRunning() || m_tempFileName.isEmpty() || !m_commandFileNameCompPtr.IsValid()){
		return false;
	}

	iser::CXmlFileWriteArchive archive(m_tempFileName, m_versionInfoCompPtr.GetPtr());

	if (!(const_cast<icomp::IRegistry&>(registry)).Serialize(archive)){
		return false;
	}

	QString acfExeFile = iqt::CSystem::GetEnrolledPath(m_commandFileNameCompPtr->GetPath());

	QDir applicationDir(QCoreApplication::applicationDirPath());
	QString acfApplicationPath = applicationDir.absoluteFilePath(acfExeFile);

	m_process.setWorkingDirectory(applicationDir.path());

	QStringList parameters;
	parameters << m_tempFileName;

	if (m_envManagerCompPtr.IsValid()){
		QString configFilePath = m_envManagerCompPtr->GetConfigFilePath();
		if (!configFilePath.isEmpty()){
			parameters << "-config";
			parameters << configFilePath;
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


// protected methods

// reimplemented (icomp::CComponentBase)

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


// protected slots

void CRegistryPreviewComp::OnStateChanged(QProcess::ProcessState state)
{
	istd::CChangeNotifier notifier(this);

	m_isRunning = (state == QProcess::Running);
}


} // namespace icmpstr


