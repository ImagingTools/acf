#include <ifilegui/CFilePreviewGuiComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrent>
#else
#include <QtCore/QtConcurrentRun>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>


namespace ifilegui
{


// public methods

CFilePreviewGuiComp::CFilePreviewGuiComp()
	:m_previewWasGenerated(false)
{
}


// protected methods

// reimplemented (imod::TGuiObserverWrap)

void CFilePreviewGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (m_previewObjectPtr.IsValid() && m_objectObserverCompPtr.IsValid()){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_previewObjectPtr.GetPtr());
		if ((modelPtr != NULL) && !modelPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
			modelPtr->AttachObserver(m_objectObserverCompPtr.GetPtr());
		}
	}
}


void CFilePreviewGuiComp::OnGuiModelDetached()
{
	if (m_previewObjectPtr.IsValid() && m_objectObserverCompPtr.IsValid()){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_previewObjectPtr.GetPtr());
		if ((modelPtr != NULL) && modelPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
			modelPtr->DetachObserver(m_objectObserverCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();
}


void CFilePreviewGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	if (!m_fileLoaderCompPtr.IsValid()){
		return;
	}

	UpdateFilePreview();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CFilePreviewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_fileLoaderCompPtr.EnsureInitialized();

	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->CreateGui(ObjectViewFrame);	
	}

	connect(&m_fileSystemObserver, SIGNAL(fileChanged(const QString&)), this, SLOT(UpdateFilePreview()));
	connect(&m_previewGenerationWatcher, SIGNAL(finished()), this, SLOT(OnPreviewGenerationFinished()));

	PreviewStack->setCurrentIndex(0);

	if (m_noAvailableLabelAttrPtr.IsValid()){
		NoPreviewAvailableLabel->setText(*m_noAvailableLabelAttrPtr);
	}

	if (m_noAvailableIconPathAttrPtr.IsValid()){
		NoPreviewAvailableIcon->setPixmap(QPixmap(QString(*m_noAvailableIconPathAttrPtr)));
	}

	m_timer.setSingleShot(true);
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(UpdateFilePreview()));
}


void CFilePreviewGuiComp::OnGuiDestroyed()
{
	m_previewGenerationWatcher.waitForFinished();

	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->DestroyGui();
	}

	m_timer.disconnect();
	if (m_timer.isActive()){
		m_timer.stop();
	}

	m_lastModificationTimeStamp = QDateTime();
	m_lastFilePath.clear();
	m_previewWasGenerated = false;

	BaseClass::OnGuiDestroyed();
}


void CFilePreviewGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	if (m_noAvailableLabelAttrPtr.IsValid()){
		NoPreviewAvailableLabel->setText(*m_noAvailableLabelAttrPtr);
	}
}


// reimplemented (icomp::CComponentBase)

void CFilePreviewGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_objectFactoryCompPtr.IsValid()){
		m_previewObjectPtr.SetPtr(m_objectFactoryCompPtr.CreateInstance());
		m_workingObjectPtr.SetPtr(m_objectFactoryCompPtr.CreateInstance());
	}
}


// private slots

void CFilePreviewGuiComp::UpdateFilePreview()
{
	if (m_currentPreviewObjectCompPtr.IsValid()){
		m_currentPreviewObjectCompPtr->ResetData();
	}

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QString newFilePath = objectPtr->GetPath();

		int fileQueryFlags = ifile::IFilePersistence::QF_LOAD;

		QFileInfo newFileInfo(newFilePath);
		if (newFileInfo.isFile()){
			fileQueryFlags |= ifile::IFilePersistence::QF_FILE;
		}
		else if (newFileInfo.isDir()){
			fileQueryFlags |= ifile::IFilePersistence::QF_DIRECTORY;
		}

		if (m_fileLoaderCompPtr->IsOperationSupported(NULL, NULL, fileQueryFlags)){
			if (m_lastFilePath != newFilePath){
				m_lastFilePath = newFilePath;

				m_fileSystemObserver.removePaths(m_fileSystemObserver.files());
				m_fileSystemObserver.addPath(m_lastFilePath);

				m_lastModificationTimeStamp = QDateTime();

				if (*m_isAsynchronPreviewGenerationEnabledAttrPtr){
					m_previewGenerationWatcher.setFuture(QtConcurrent::run(this, &CFilePreviewGuiComp::UpdateObjectFromFile));
				}
				else{
					UpdateObjectFromFile();
					OnPreviewGenerationFinished();
				}
			}
		}
		else{
			m_lastFilePath = QString();

			ResetPreview();

			if (!newFilePath.isEmpty()){
				if (!m_timer.isActive()){
					m_timer.start();
				}
			}
		}
	}
	else{
		ResetPreview();
	}
}


void CFilePreviewGuiComp::OnPreviewGenerationFinished()
{
	QMutexLocker lock(&m_mutex);

	if (m_workingObjectPtr.IsValid()){
		if (m_previewObjectPtr.IsValid()){
			// Copy data from working object to the preview:
			istd::CChangeNotifier changePtr(m_previewObjectPtr.GetPtr());

			m_previewObjectPtr->CopyFrom(*m_workingObjectPtr.GetPtr());
		}

		if (m_currentPreviewObjectCompPtr.IsValid()){
			istd::CChangeNotifier copyChangePtr(m_currentPreviewObjectCompPtr.GetPtr());

			m_currentPreviewObjectCompPtr->CopyFrom(*m_workingObjectPtr);
		}

		int pageIndex = (!m_previewWasGenerated || m_lastFilePath.isEmpty()) ? 0 : 1;

		PreviewStack->setCurrentIndex(pageIndex);
	}
}


// private methods

void CFilePreviewGuiComp::UpdateObjectFromFile()
{
	QMutexLocker lock(&m_mutex);

	if (!m_workingObjectPtr.IsValid()){
		return;
	}

	QFileInfo fileInfo(m_lastFilePath);

	if (!fileInfo.exists()){
		m_workingObjectPtr->ResetData();

		m_previewWasGenerated = false;

		return;
	}

	if (fileInfo.lastModified() != m_lastModificationTimeStamp){
		m_lastModificationTimeStamp = fileInfo.lastModified();

		if (m_fileLoaderCompPtr.IsValid()){
			int retVal = m_fileLoaderCompPtr->LoadFromFile(*m_workingObjectPtr.GetPtr(), m_lastFilePath);
			if (retVal != ifile::IFilePersistence::OS_OK){
				m_workingObjectPtr->ResetData();

				m_previewWasGenerated = false;
			}
			else{
				m_previewWasGenerated = true;
			}
		}
	}
}


void CFilePreviewGuiComp::ResetPreview()
{
	Q_ASSERT(IsGuiCreated());

	if (IsGuiCreated()){
		m_previewWasGenerated = false;

		if (m_previewObjectPtr.IsValid()){
			m_previewObjectPtr->ResetData();
		}

		PreviewStack->setCurrentIndex(0);
	}
}


} // namespace ifilegui


