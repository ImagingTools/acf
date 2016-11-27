#include <ifile/CAutoPersistenceComp.h>


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
#include <ifile/IFileNameParam.h>


namespace ifile
{


// public methods

CAutoPersistenceComp::CAutoPersistenceComp()
:	m_isObjectChanged(false),
	m_isLoadedFromFile(false),
	m_blockLoadingOnFileChanges(false)
{
}


// protected methods

void CAutoPersistenceComp::SaveObjectSnapshot()
{
	if (!m_objectShadowPtr.IsValid()){
		return;
	}

	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(m_objectShadowPtr.GetPtr());

	// For the serializable objects memory comparison can be done:
	if (serializablePtr != NULL){
		iser::CMemoryWriteArchive currentState;

		if (serializablePtr->Serialize(currentState)){
			if (currentState == m_lastStoredObjectState){
				return;
			}

			m_lastStoredObjectState = currentState;
		}
	}

	StoreObject(*m_objectShadowPtr.GetPtr());
}


bool CAutoPersistenceComp::LoadObject(const QString& filePath)
{
	bool retVal = false;
	if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
		if (IsVerboseEnabled()){
			SendVerboseMessage(tr("Load data from file %1").arg(filePath));
		}

		if (LockFile(filePath, false)){
			if (m_loadSaveMutex.tryLock()){

				int operationState = m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, filePath);

				m_loadSaveMutex.unlock();

				if (operationState == ifile::IFilePersistence::OS_OK){
					m_isLoadedFromFile = true;

					retVal = true;
				}
			}
		}
		else{
			SendWarningMessage(0, tr("Locking %1 unsuccesful (load)").arg(filePath));
		}
		UnlockFile();
	}

	return retVal;
}


bool CAutoPersistenceComp::StoreObject(const istd::IChangeable& object)
{
	bool retVal = false;

	if (m_fileLoaderCompPtr.IsValid()){
		QString filePath;
		if (m_filePathCompPtr.IsValid()){
			filePath = m_filePathCompPtr->GetPath();
		}

		if (IsVerboseEnabled()){
			SendVerboseMessage(tr("Save data to file %1").arg(filePath));
		}

		if (LockFile(filePath, true)){
			if (m_loadSaveMutex.tryLock()){
				if (*m_reloadOnFileChangeAttrPtr){
					m_blockLoadingOnFileChanges = true;
				}

				int operationState = m_fileLoaderCompPtr->SaveToFile(object, filePath);

				m_loadSaveMutex.unlock();

				if (operationState == ifile::IFilePersistence::OS_OK){
					retVal = true;

					m_isLoadedFromFile = true;
				}
			}
		}
		else{
			SendWarningMessage(0, tr("Locking %1 unsuccesful (store)").arg(filePath));
		}

		UnlockFile();
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CAutoPersistenceComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QString filePath;
	if (m_filePathCompPtr.IsValid()){
		filePath = m_filePathCompPtr->GetPath();
	}

	if (*m_restoreOnBeginAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			LoadObject(filePath);
		}
	}

	if (m_objectModelCompPtr.IsValid()){
		BaseClass2::RegisterModel(m_objectModelCompPtr.GetPtr(), MI_OBJECT);
	}

	if (m_filePathModelCompPtr.IsValid()){
		BaseClass2::RegisterModel(m_filePathModelCompPtr.GetPtr(), MI_FILEPATH);
	}

	if (m_storeOnBeginAttrPtr.IsValid() && *m_storeOnBeginAttrPtr && !QFile::exists(filePath)){
		m_objectCompPtr.EnsureInitialized();
		
		if (m_objectCompPtr.IsValid()){
			StoreObject(*m_objectCompPtr);
		}
	}

	if (*m_reloadOnFileChangeAttrPtr){
		m_fileWatcher.addPath(filePath);

		connect(&m_fileWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileContentsChanged(const QString&)));
	}

	if (m_runtimeStatusCompPtr.IsValid() &&	m_runtimeStatusModelCompPtr.IsValid()){
		BaseClass2::RegisterModel(m_runtimeStatusModelCompPtr.GetPtr(), MI_RUNTIME_STATUS);
	}

	connect(&m_storingTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));

	TryStartIntervalStore();
}


void CAutoPersistenceComp::OnComponentDestroyed()
{
	BaseClass2::UnregisterAllModels();

	if (!m_runtimeStatusCompPtr.IsValid() || !m_runtimeStatusModelCompPtr.IsValid()){
		disconnect(&m_storingTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
		disconnect(&m_fileWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileContentsChanged(const QString&)));

		m_storingFuture.waitForFinished();

		bool storeOnEnd = *m_storeOnEndAttrPtr || (!m_isLoadedFromFile && *m_storeOnChangeAttrPtr);
		if (storeOnEnd && m_objectCompPtr.IsValid()){
			StoreObject(*m_objectCompPtr.GetPtr());
		}
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CAutoPersistenceComp::OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet)
{
	switch (modelId)
	{
		case MI_OBJECT:
		{
			if (changeSet.Contains(istd::IChangeable::CF_NO_UNDO)){
				return;
			}

			m_isObjectChanged = true;

			if (!TryStartIntervalStore()){
				if (*m_storeOnChangeAttrPtr && m_objectCompPtr.IsValid()){
					StoreObject(*m_objectCompPtr.GetPtr());
				}
			}
		}
		break;

		case MI_FILEPATH:
		{
			if (*m_reloadOnFileChangeAttrPtr){
				m_fileWatcher.removePaths(m_fileWatcher.files());
			}

			if (*m_restoreOnBeginAttrPtr && !m_isLoadedFromFile){
				QString filePath;
				if (m_filePathCompPtr.IsValid()){
					filePath = m_filePathCompPtr->GetPath();
				}

				QFileInfo fileInfo(filePath);
				if (fileInfo.exists()){
					filePath = fileInfo.absoluteFilePath();

					LoadObject(filePath);

					if (*m_reloadOnFileChangeAttrPtr){
						m_fileWatcher.addPath(filePath);
					}
				}
			}
		}
		break;

		case MI_RUNTIME_STATUS:
			if (m_runtimeStatusCompPtr->GetRuntimeStatus() == ibase::IRuntimeStatusProvider::RS_SHUTDOWN){
				disconnect(&m_storingTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
				disconnect(&m_fileWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(OnFileContentsChanged(const QString&)));

				m_storingFuture.waitForFinished();

				bool storeOnEnd = *m_storeOnEndAttrPtr || (!m_isLoadedFromFile && *m_storeOnChangeAttrPtr);
				if (storeOnEnd && m_objectCompPtr.IsValid()){
					StoreObject(*m_objectCompPtr.GetPtr());
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
	if (!m_isObjectChanged){
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

	m_isObjectChanged = false;
}


void CAutoPersistenceComp::OnFileContentsChanged(const QString& path)
{
	if (!m_objectCompPtr.IsValid() || !m_fileLoaderCompPtr.IsValid()){
		return;
	}

	// File was updated by own call of StoreObject:
	if (*m_reloadOnFileChangeAttrPtr && m_blockLoadingOnFileChanges){
		m_blockLoadingOnFileChanges = false;

		return;
	}

	if (IsVerboseEnabled()){
		SendVerboseMessage(tr("Detected change of file %1").arg(path));
	}

	if (path.isEmpty()){
		//Path empty -> no need for copy
		LoadObject(path);

		return;
	}

	QString tempPath;

#if QT_VERSION < 0x050000
	tempPath = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#else
	tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#endif

	QString tempFileName = tempPath + "/" + QUuid::createUuid().toString() + "_" + QFileInfo(path).fileName();

	if (LockFile(path, true)){
		if (QFile::copy(path, tempFileName)){
			UnlockFile();

			LoadObject(tempFileName);

			QFile::remove(tempFileName);

			m_fileWatcher.removePaths(m_fileWatcher.files());
			m_fileWatcher.addPath(path);
		}
	}
	else{
		SendWarningMessage(0, tr("Locking %1 unsuccesful (secure load)").arg(path));
	}

	UnlockFile();
}


// private methods

bool CAutoPersistenceComp::TryStartIntervalStore()
{
	if (		m_storeIntervalAttrPtr.IsValid() &&
				!QCoreApplication::startingUp() &&
				!m_storingTimer.isActive()){
		m_storingTimer.start(qMax(1, int(*m_storeIntervalAttrPtr * 1000)));
	}

	return m_storingTimer.isActive();
}


bool CAutoPersistenceComp::LockFile(const QString& filePath, bool store) const
{
#if QT_VERSION >= 0x050000
	QFileInfo fileInfo(filePath);
	bool lockRequired = !filePath.isEmpty();
	lockRequired = lockRequired && fileInfo.exists();
	if (!store) {
		//load operation -> Lock is not required when writing is not possible
		lockRequired = lockRequired && *m_enableLockForLoadAttrPtr;
		lockRequired = lockRequired && fileInfo.isWritable();
	}
	if (lockRequired){
		QString lockFilePath = filePath + ".lock";

		if (!m_lockFilePtr.IsValid()){
			m_lockFilePtr.SetPtr(new QLockFile(lockFilePath));
			int staleLockTime = int(*m_staleLockTimeAttrPtr * 1000.0);
			m_lockFilePtr->setStaleLockTime(staleLockTime);
		}

		if (m_lockFilePtr->isLocked()){
			SendWarningMessage(0, tr("Possible deadlock conditions: %1").arg(filePath));
		}

		Q_ASSERT(m_lockFilePtr.IsValid());
		int timeout = int(*m_tryLockTimeoutAttrPtr * 1000.0);
		return m_lockFilePtr->tryLock(timeout);
	}
#endif

	return true;
}


void CAutoPersistenceComp::UnlockFile() const
{
#if QT_VERSION >= 0x050000
	if (m_lockFilePtr.IsValid()){
		m_lockFilePtr->unlock();

		m_lockFilePtr.Reset();
	}
#endif
}


} // namespace ifile


