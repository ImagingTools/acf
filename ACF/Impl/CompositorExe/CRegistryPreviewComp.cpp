#include "CRegistryPreviewComp.h"

#include <QTemporaryFile>
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMetaType>

#include "iser/CXmlFileWriteArchive.h"


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

	connect(	this, 
				SIGNAL(finished(int, QProcess::ExitStatus)), 
				this, 
				SLOT(OnFinished(int, QProcess::ExitStatus)), 
				Qt::QueuedConnection);
}


void CRegistryPreviewComp::OnComponentDestroyed()
{
	if (IsRunning()){
		kill();

		if (waitForFinished()){
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

	QTemporaryFile tempFile("registry_preview.arx");
	if (tempFile.open()){
		m_tempFileName = tempFile.fileName();

		iser::CXmlFileWriteArchive archive(m_tempFileName.toStdString());

		retVal = (const_cast<icomp::IRegistry&>(registry)).Serialize(archive);
	}
	else{
		retVal = false;
	}

	if (retVal){
		static QString acfExeFile = "Acf.exe";

		QFileInfo fileInfo(acfExeFile);
		if (fileInfo.exists()){
			QProcess::start(acfExeFile, QStringList() << m_tempFileName);
			if (waitForStarted()){
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
	if (QProcess::state() == QProcess::Running){
		return true;
	}

	return false;
}


void CRegistryPreviewComp::AbortRegistry()
{
	QProcess::terminate();
	if (!waitForFinished()){
		kill();
	}
}


// protected slots

void CRegistryPreviewComp::OnFinished(int exitCode, ExitStatus exitStatus)
{
	QFile::remove(m_tempFileName);
}

