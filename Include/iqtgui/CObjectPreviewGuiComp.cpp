#include "iqtgui/CObjectPreviewGuiComp.h"


// Qt includes
#include <QFileInfo>


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtgui
{


// public methods

// reimplemented (imod::IModelEditor)

void CObjectPreviewGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		QString qtFilePath = iqt::GetQString(objectPtr->GetPath());

		if (m_lastFilePath != qtFilePath){
			m_lastFilePath = qtFilePath;

			m_fileSystemObserver.removePaths(m_fileSystemObserver.files());

			if (QFile::exists(m_lastFilePath)){
				m_fileSystemObserver.addPath(m_lastFilePath);
			}
			else{
				m_lastFilePath = QString();
			}

			m_lastModificationTimeStamp = QDateTime();
		}

		UpdateObjectFromFile();
	}
}


void CObjectPreviewGuiComp::UpdateModel() const
{
}


// protected methods

// reimplemented (imod::TGuiObserverWrap)

void CObjectPreviewGuiComp::OnGuiModelDetached()
{
	if (m_objectObserverCompPtr.IsValid() && m_objectModelCompPtr.IsValid()){
		if (m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
			m_objectModelCompPtr->DetachObserver(m_objectObserverCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CObjectPreviewGuiComp::OnGuiCreated()
{
	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->CreateGui(ObjectViewFrame);	
	}

	connect(&m_fileSystemObserver, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileChanged(const QString&)));

	BaseClass::OnGuiCreated();
}


void CObjectPreviewGuiComp::OnGuiDestroyed()
{
	if (m_objectGuiCompPtr.IsValid()){
		m_objectGuiCompPtr->DestroyGui();	
	}

	BaseClass::OnGuiDestroyed();
}


// private slots

void CObjectPreviewGuiComp::OnFileChanged(const QString&/* filePath*/)
{
	UpdateObjectFromFile();
}


// private methods

void CObjectPreviewGuiComp::UpdateObjectFromFile()
{
	QFileInfo fileInfo(m_lastFilePath);
	if (!fileInfo.exists()){
		if (m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
			m_objectModelCompPtr->DetachObserver(m_objectObserverCompPtr.GetPtr());
		}
	}
	else{
		if (!m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
			m_objectModelCompPtr->AttachObserver(m_objectObserverCompPtr.GetPtr());
		}
	}
	
	if (fileInfo.lastModified() != m_lastModificationTimeStamp){
		m_lastModificationTimeStamp = fileInfo.lastModified();

		istd::CChangeNotifier changePtr(m_objectCompPtr.GetPtr());

		int retVal = m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr.GetPtr(), iqt::GetCString(m_lastFilePath));
		if (retVal != iser::IFileLoader::StateOk){
			if (m_objectModelCompPtr->IsAttached(m_objectObserverCompPtr.GetPtr())){
				m_objectModelCompPtr->DetachObserver(m_objectObserverCompPtr.GetPtr());
			}
		}
	}
}


} // namespace iqtgui


