#include "ibase/CComposedLoaderComp.h"


// Qt includes
#include <QtCore/QStringList>


namespace ibase
{


// reimplemented (iser::IFileLoader)

bool CComposedLoaderComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr,
		int flags,
		bool beQuiet) const
{
	int slavesCount = m_slaveLoadersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const iser::IFileLoader* loaderPtr = m_slaveLoadersCompPtr[i];
		if ((loaderPtr != NULL) && loaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet)){
			return true;
		}
	}

	return false;
}


int CComposedLoaderComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
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
		iser::IFileLoader* loaderPtr = m_slaveLoadersCompPtr[i];
		if ((loaderPtr != NULL) && loaderPtr->IsOperationSupported(&data, &filePath, fileQueryFlags)){
			return loaderPtr->LoadFromFile(data, filePath);
		}
	}

	return StateFailed;
}


int CComposedLoaderComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
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
		iser::IFileLoader* loaderPtr = m_slaveLoadersCompPtr[i];
		if ((loaderPtr != NULL) && loaderPtr->IsOperationSupported(&data, &filePath, fileQueryFlags)){
			return loaderPtr->SaveToFile(data, filePath);
		}
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CComposedLoaderComp::GetFileExtensions(QStringList& result, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	bool retVal = false;

	int slavesCount = m_slaveLoadersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		iser::IFileLoader* loaderPtr = m_slaveLoadersCompPtr[i];
		if (loaderPtr != NULL){
			retVal = loaderPtr->GetFileExtensions(result, flags, true) | retVal;
		}
	}

	return retVal;
}


QString CComposedLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int slavesCount = m_slaveLoadersCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			iser::IFileLoader* loaderPtr = m_slaveLoadersCompPtr[i];
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


} // namespace ibase


