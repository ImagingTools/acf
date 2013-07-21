#include "ifilegui/CFilePreviewGuiComp.h"


// Qt includes
#include <QtCore/QFileInfo>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace ifilegui
{


// protected methods

// reimplemented (imod::TGuiObserverWrap)

void CFilePreviewGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (		m_objectModelCompPtr.IsValid() &&
				m_objectObserverCompPtr.IsValid() &&
				!m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
		m_objectModelCompPtr->AttachObserver(m_objectObserverCompPtr.GetPtr());
	}
}


void CFilePreviewGuiComp::OnGuiModelDetached()
{
	if (		m_objectModelCompPtr.IsValid() &&
				m_objectObserverCompPtr.IsValid() &&
				m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
		m_objectModelCompPtr->DetachObserver(m_objectObserverCompPtr.GetPtr());
	}

	BaseClass::OnGuiModelDetached();
}


void CFilePreviewGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	if (!m_fileLoaderCompPtr.IsValid()){
		return;
	}

	ifile::IFileNameParam* objectPtr = GetObjectPtr();
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

				UpdateObjectFromFile();
			}
		}
		else{
			m_lastFilePath = QString();
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CFilePreviewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->CreateGui(ObjectViewFrame);	
	}

	connect(&m_fileSystemObserver, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileChanged(const QString&)));
}


void CFilePreviewGuiComp::OnGuiDestroyed()
{
	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->DestroyGui();	
	}

	BaseClass::OnGuiDestroyed();
}


// private slots

void CFilePreviewGuiComp::OnFileChanged(const QString&/* filePath*/)
{
	UpdateObjectFromFile();
}


// private methods

void CFilePreviewGuiComp::UpdateObjectFromFile()
{
	QFileInfo fileInfo(m_lastFilePath);

	bool disableView = !fileInfo.exists();

	if (fileInfo.lastModified() != m_lastModificationTimeStamp){
		m_lastModificationTimeStamp = fileInfo.lastModified();

		istd::CChangeNotifier changePtr(m_objectCompPtr.GetPtr());

		if (m_fileLoaderCompPtr.IsValid()){
			int retVal = m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr.GetPtr(), m_lastFilePath);
			if (retVal != ifile::IFilePersistence::OS_OK){
				disableView = true;
			}
		}
	}

	if (m_objectModelCompPtr.IsValid() && m_objectObserverCompPtr.IsValid()){
		if (disableView){
			if (m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
				m_objectModelCompPtr->DetachObserver(m_objectObserverCompPtr.GetPtr());
			}
		}
		else{
			if (!m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
				m_objectModelCompPtr->AttachObserver(m_objectObserverCompPtr.GetPtr());
			}
		}
	}
}


} // namespace ifilegui


