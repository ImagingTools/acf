#include "icmpstr/CRegistryPreviewComp.h"


// Qt includes
#include <QTemporaryFile>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMetaType>

#include "iser/CXmlFileWriteArchive.h"


namespace icmpstr
{


// public methods

// reimplemented (icomp::IComponent)

void CRegistryPreviewComp::OnComponentCreated()
{
	qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

	BaseClass::OnComponentCreated();

	QFont font = qApp->font();
	font.setPointSize(12);
	m_startLabel.setFont(font);
	m_startLabel.setText(QApplication::tr("Registry is starting... Please wait just a moment!"));
	m_startLabel.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);

	connect(	&m_process, 
				SIGNAL(finished(int, QProcess::ExitStatus)), 
				this, 
				SLOT(OnFinished(int, QProcess::ExitStatus)), 
				Qt::QueuedConnection);
}


void CRegistryPreviewComp::OnComponentDestroyed()
{
	if (IsRunning()){
		m_process.kill();

		if (m_process.waitForFinished(5000)){
			QFile::remove(m_tempFileName);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
	if (IsRunning()){
		return false;
	}

	m_tempFileName.clear();

	QDesktopWidget* desktopWidget = QApplication::desktop();
	if (desktopWidget != NULL){
		m_startLabel.move(desktopWidget->availableGeometry().center() - QPoint(m_startLabel.width() / 2, m_startLabel.height() / 2));
	}

	m_startLabel.show();

	bool retVal = true;

	QDir tempDir = QDir::temp();
	if (tempDir.exists()){
		m_tempFileName = tempDir.absoluteFilePath("registry_preview.arx");

		iser::CXmlFileWriteArchive archive(m_tempFileName.toStdString());

		retVal = (const_cast<icomp::IRegistry&>(registry)).Serialize(archive);
	}
	else{
		retVal = false;
	}

	if (retVal){
		static QString acfExeFile = "Acf";

		QDir applicationDir(QCoreApplication::applicationDirPath());
		QString acfApplicationPath = applicationDir.absoluteFilePath(acfExeFile);

		QFileInfo fileInfo(acfApplicationPath);
		if (fileInfo.exists()){
			m_process.setWorkingDirectory(applicationDir.path());
			m_process.start(acfApplicationPath, QStringList() << m_tempFileName);
			if (m_process.waitForStarted()){
				retVal = true;
			}
		}
		else{
			QMessageBox::critical(NULL, tr("ACF Compositor"), tr("Acf.exe was not found! The registry preview cannot be started!"));

			retVal = false;
		}
	}

	m_startLabel.hide();
	
	return retVal;
}


bool CRegistryPreviewComp::IsRunning() const
{
	if (m_process.state() == QProcess::Running){
		return true;
	}

	return false;
}


void CRegistryPreviewComp::AbortRegistry()
{
	m_process.terminate();
	if (!m_process.waitForFinished(5000)){
		m_process.kill();
	}
}


// protected slots

void CRegistryPreviewComp::OnFinished(int exitCode, QProcess::ExitStatus /*exitStatus*/)
{
	QFile::remove(m_tempFileName);
}


} // namespace icmpstr


