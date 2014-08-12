#include "ifile/CComposedFilePersistenceComp.h"


// Qt includes
#include <QtCore/QStringList>


namespace ifile
{


// reimplemented (ifile::IFilePersistence)

bool CComposedFilePersistenceComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr,
		int flags,
		bool beQuiet) const
{
	int slavesCount = m_slaveLoadersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const ifile::IFilePersistence* loaderPtr = m_slaveLoadersCompPtr[i];
		if ((loaderPtr != NULL) && loaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet)){
			return true;
		}
	}

	return false;
}


int CComposedFilePersistenceComp::LoadFromFile(istd::IChangeable& data, const QString& filePath, ibase::IProgressManager* /*progressManagerPtr*/) const
{
	int fileQueryFlags = QF_LOAD;
	if (!filePath.isEmpty()){
		fileQueryFlags |= QF_FILE | QF_DIRECTORY;
	}
	else{
		fileQueryFlags |= QF_ANONYMOUS;
	}

	int slavesCount = m_slaveLoadersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		ifile::IFilePersistence* loaderPtr = m_slaveLoadersCompPtr[i];
		if ((loaderPtr != NULL) && loaderPtr->IsOperationSupported(&data, &filePath, fileQueryFlags)){
			m_lastOpenInfo = QFileInfo(filePath);

			return loaderPtr->LoadFromFile(data, filePath);
		}
	}

	return OS_FAILED;
}


int CComposedFilePersistenceComp::SaveToFile(const istd::IChangeable& data, const QString& filePath, ibase::IProgressManager* /*progressManagerPtr*/) const
{
	int fileQueryFlags = QF_SAVE;
	if (!filePath.isEmpty()){
		fileQueryFlags |= QF_FILE | QF_DIRECTORY;
	}
	else{
		fileQueryFlags |= QF_ANONYMOUS;
	}

	int slavesCount = m_slaveLoadersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		ifile::IFilePersistence* loaderPtr = m_slaveLoadersCompPtr[i];
		if ((loaderPtr != NULL) && loaderPtr->IsOperationSupported(&data, &filePath, fileQueryFlags)){
			m_lastSaveInfo = QFileInfo(filePath);

			return loaderPtr->SaveToFile(data, filePath);
		}
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CComposedFilePersistenceComp::GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	bool retVal = false;

	int slavesCount = m_slaveLoadersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		ifile::IFilePersistence* loaderPtr = m_slaveLoadersCompPtr[i];
		if (loaderPtr != NULL){
			retVal = loaderPtr->GetFileExtensions(result, dataObjectPtr, flags, true) | retVal;
		}
	}

	return retVal;
}


QString CComposedFilePersistenceComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int slavesCount = m_slaveLoadersCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			ifile::IFilePersistence* loaderPtr = m_slaveLoadersCompPtr[i];
			if (loaderPtr != NULL){
				QStringList extensions;
				loaderPtr->GetFileExtensions(extensions);

				if (extensions.contains(*extensionPtr)){
					return loaderPtr->GetTypeDescription(extensionPtr);
				}
			}
		}
	}

	if (m_commonDescriptionAttrPtr.IsValid()){
		return *m_commonDescriptionAttrPtr;
	}

	return "";
}

// reimplemented (ifile::IFilePersistenceInfo)

QString CComposedFilePersistenceComp::GetLastFilePath(OperationType operationType, PathType pathType) const
{
	switch (operationType){
	case OT_LOAD:
		return GetPathForType(m_lastOpenInfo, pathType);

	case OT_SAVE:
		return GetPathForType(m_lastSaveInfo, pathType);

	default:
		if (!m_lastOpenInfo.filePath().isEmpty()){
			return GetPathForType(m_lastOpenInfo, pathType);
		}
		else{
			return GetPathForType(m_lastOpenInfo, pathType);
		}
	}
}

// protected methods

QString CComposedFilePersistenceComp::GetPathForType(const QFileInfo& fileInfo, PathType pathType) const
{
	switch (pathType){
	case OT_FILENAME:
		return fileInfo.fileName();

	case OT_DIR:
		return fileInfo.absolutePath();

	default:
		return fileInfo.absoluteFilePath();
	}
}


} // namespace ifile


