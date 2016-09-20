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
}


void CFilePreviewGuiComp::OnGuiDestroyed()
{
	m_previewGenerationWatcher.waitForFinished();

	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->DestroyGui();	
	}

	BaseClass::OnGuiDestroyed();
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

			if (m_previewObjectPtr.IsValid()){
				m_previewObjectPtr->ResetData();
			}

			if (!newFilePath.isEmpty()){
				 QTimer::singleShot(1000, this, SLOT(UpdateFilePreview()));
			}
		}
	}
	else{
		if (m_previewObjectPtr.IsValid()){
			m_previewObjectPtr->ResetData();
		}
	}
}


void CFilePreviewGuiComp::OnPreviewGenerationFinished()
{
	QMutexLocker lock(&m_mutex);

	if (m_previewObjectPtr.IsValid() && m_workingObjectPtr.IsValid()){

		// Copy data from working object to the preview:
		istd::CChangeNotifier changePtr(m_previewObjectPtr.GetPtr());

		m_previewObjectPtr->CopyFrom(*m_workingObjectPtr.GetPtr());

		if (m_currentPreviewObjectCompPtr.IsValid()){
			istd::CChangeNotifier copyChangePtr(m_currentPreviewObjectCompPtr.GetPtr());

			m_currentPreviewObjectCompPtr->CopyFrom(*m_workingObjectPtr);
		}
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

		return;
	}

	if (fileInfo.lastModified() != m_lastModificationTimeStamp){
		m_lastModificationTimeStamp = fileInfo.lastModified();

		if (m_fileLoaderCompPtr.IsValid()){
			int retVal = m_fileLoaderCompPtr->LoadFromFile(*m_workingObjectPtr.GetPtr(), m_lastFilePath);
			if (retVal != ifile::IFilePersistence::OS_OK){
				m_workingObjectPtr->ResetData();
			}
		}
	}
}


} // namespace ifilegui


