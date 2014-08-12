#include "ifile/CFileSerializerCompBase.h"


// Qt includes
#include <QtCore/QDir>


namespace ifile
{


// reimplemented (ifile::IFilePersistence)

bool CFileSerializerCompBase::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(dataObjectPtr);
	if (serializablePtr == NULL){
		serializablePtr = CompCastPtr<iser::ISerializable>(dataObjectPtr);
	}

	if ((dataObjectPtr != NULL) && (serializablePtr == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, QObject::tr("Object is not serializable"));
		}

		return false;
	}

	if ((flags & (QF_FILE)) == 0){
		return false;
	}

	if (filePathPtr != NULL){
		if ((flags & (QF_LOAD | QF_SAVE)) == QF_LOAD){
			if (!CheckInputFile(*filePathPtr, beQuiet)){
				return false;
			}
		}

		if ((flags & QF_SAVE) != 0){
			if (!CheckOutputFile(*filePathPtr, beQuiet)){
				return false;
			}
		}

		QStringList fileExtensions;
		if (GetFileExtensions(fileExtensions, dataObjectPtr, flags)){
			int extensionsCount = int(fileExtensions.size());
			for (int i = 0; i < extensionsCount; ++i){
				const QString& extension = fileExtensions[i];
				QFileInfo fileInfo(*filePathPtr);
				QString fileExtension = fileInfo.suffix();

				if (!filePathPtr->isEmpty() &&  (fileExtension.compare(extension, Qt::CaseInsensitive) == 0)){
					return true;
				}
			}

			if (!beQuiet){
				SendInfoMessage(MI_BAD_EXTENSION, QObject::tr("File extension is not supported"));
			}

			return false;
		}
	}

	return true;
}


// protected methods

const iser::IVersionInfo* CFileSerializerCompBase::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


bool CFileSerializerCompBase::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int id = *iter;

		quint32 objectMinimalVersion = object.GetMinimalVersion(id);

		quint32 versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)){
			if (versionNumber < objectMinimalVersion){
				return false;
			}
		}
	}

	return true;
}


bool CFileSerializerCompBase::CheckInputFile(const QString filePath, bool beQuiet) const
{
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists()){
		if (!beQuiet){
			SendWarningMessage(MI_CANNOT_LOAD, QString("File %1 does not exist").arg(filePath));
		}

		return false;
	}
	
	return true;
}


bool CFileSerializerCompBase::CheckOutputFile(const QString filePath, bool beQuiet) const
{
	QFileInfo fileInfo(filePath);

	QDir directory = fileInfo.dir();
	if (!directory.exists()){
		if (!beQuiet){
			SendWarningMessage(MI_CANNOT_SAVE, QString("Save target directory %1 does not exist").arg(directory.canonicalPath()));
		}

		return false;
	}

	return true;
}


} // namespace ifile


