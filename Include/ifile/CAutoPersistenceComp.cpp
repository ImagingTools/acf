#include "ifile/CAutoPersistenceComp.h"


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QStandardPaths>
#else
#include <QtGui/QDesktopServices>
#include <QtCore>
#endif

// ACF includes
#include "ifile/IFileNameParam.h"


namespace ifile
{


// public methods

CAutoPersistenceComp::CAutoPersistenceComp()
	:m_isDataWasChanged(false),
	m_wasLoadingSuceeded(false),
	m_isReloading(false)
{
}


// protected methods

void CAutoPersistenceComp::SaveObjectSnapshot()
{
	bool skipStoring = false;

	if (!m_objectShadowPtr.IsValid()){
		return;
	}

	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(m_objectShadowPtr.GetPtr());

	// For the serializable objects memory comparison can be done:
	if (serializablePtr != NULL){
		iser::CMemoryWriteArchive currentState;

		if (serializablePtr->Serialize(currentState)){
			if (currentState != m_lastStoredObjectState){
				m_lastStoredObjectState = currentState;
			}
			else{
				skipStoring = true;
			}
		}
	}

	if (!skipStoring){
		StoreObject(*m_objectShadowPtr.GetPtr());
	}
}


void CAutoPersistenceComp::StoreObject(const istd::IChangeable& object) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		QString filePath;
		if (m_filePathCompPtr.IsValid()){
			filePath = m_filePathCompPtr->GetPath();
		}

		m_fileLoaderCompPtr->SaveToFile(object, filePath);
	}
}


// reimplemented (icomp::CComponentBase)

void CAutoPersistenceComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (*m_restoreOnBeginAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			QString filePath;
			if (m_filePathCompPtr.IsValid()){
				filePath = m_filePathCompPtr->GetPath();
			}

			if (*m_reloadOnFileChangeAttrPtr){
				m_fileWatcher.addPath(filePath);
			}
			
			if (m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, filePath) == ifile::IFilePersistence::OS_OK)
			{
				m_wasLoadingSuceeded = true;
			}
		}
	}

	if (m_objectModelCompPtr.IsValid())
	{
		BaseClass2::RegisterModel(m_objectModelCompPtr.GetPtr(), MI_OBJECT);
	}

	imod::IModel* filePathModelPtr = dynamic_cast<imod::IModel*>(m_filePathCompPtr.GetPtr());
	if (filePathModelPtr != NULL)
	{
		BaseClass2::RegisterModel(filePathModelPtr, MI_FILEPATH);
	}

	if (*m_reloadOnFileChangeAttrPtr){
		connect(&m_fileWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileContentsChanged(const QString&)));
	}

	EnsureTimerConnected();
}


void CAutoPersistenceComp::OnComponentDestroyed()
{
	disconnect(&m_storingTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
	disconnect(&m_fileWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileContentsChanged(const QString&)));

	m_storingFuture.waitForFinished();

	BaseClass2::UnregisterAllModels();

	if (*m_storeOnEndAttrPtr && m_objectCompPtr.IsValid() && !m_isReloading){
		StoreObject(*m_objectCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CAutoPersistenceComp::OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	switch (modelId)
	{
		case MI_OBJECT:
		{
			bool storeByTimer = m_storeIntervalAttrPtr.IsValid() ? *m_storeIntervalAttrPtr : false;
			if (storeByTimer){
				EnsureTimerConnected();
			}
			else{
				if (*m_storeOnChangeAttrPtr && m_objectCompPtr.IsValid() && !m_isReloading){
					StoreObject(*m_objectCompPtr.GetPtr());
				}
			}

			m_isDataWasChanged = true;
		}
		break;

		case MI_FILEPATH:
		{
			if (*m_reloadOnFileChangeAttrPtr){
				m_fileWatcher.removePaths(m_fileWatcher.files());
			}

			if (*m_restoreOnBeginAttrPtr && !m_wasLoadingSuceeded){
				QString filePath;
				if (m_filePathCompPtr.IsValid()){
					filePath = m_filePathCompPtr->GetPath();
				}

				QFileInfo fileInfo(filePath);
				if(fileInfo.exists()){
					filePath = fileInfo.absoluteFilePath();

					if (*m_reloadOnFileChangeAttrPtr){
						m_fileWatcher.addPath(filePath);
					}

					if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
						if(m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, filePath) == ifile::IFilePersistence::OS_OK){
							m_wasLoadingSuceeded = true;
						}
					}
				}
			}
		}
		break;

		default:
			break;
	}
}


// private slots

void CAutoPersistenceComp::OnTimeout()
{
	if (!m_isDataWasChanged){
		return;
	}

	if (m_storingFuture.isRunning()){
		return;
	}

	if (!m_objectCompPtr.IsValid()){
		return;
	}

	m_objectShadowPtr.SetPtr(m_objectCompPtr->CloneMe(istd::IChangeable::CM_WITH_REFS));
	if (!m_objectShadowPtr.IsValid()){
		return;
	}

	m_storingFuture = QtConcurrent::run(this, &CAutoPersistenceComp::SaveObjectSnapshot);

	m_isDataWasChanged = false;
}


void CAutoPersistenceComp::OnFileContentsChanged(const QString& path)
{
	if (!m_objectCompPtr.IsValid() || !m_fileLoaderCompPtr.IsValid()){
		return;
	}

	QString tempPath;

#if QT_VERSION < 0x050000
	tempPath = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#else
	tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#endif

	QString tempFileName = tempPath + "/" + QUuid::createUuid().toString() + "_" + QFileInfo(path).fileName();

	if (QFile::copy(path, tempFileName)){
		m_wasLoadingSuceeded = false;

		m_isReloading = true;

		if (m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, tempFileName) == ifile::IFilePersistence::OS_OK)
		{
			m_wasLoadingSuceeded = true;
		}

		QFile::remove(tempFileName);

		m_isReloading = false;

		
		m_fileWatcher.removePaths(m_fileWatcher.files());
		m_fileWatcher.addPath(path);
	}
}


// private methods

void CAutoPersistenceComp::EnsureTimerConnected()
{
	if (m_storeIntervalAttrPtr.IsValid() && !QCoreApplication::startingUp()){
		if (!m_storingTimer.isActive()){
			connect(&m_storingTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));

			m_storingTimer.start(*m_storeIntervalAttrPtr * 1000);
		}
	}
}


} // namespace ifile


