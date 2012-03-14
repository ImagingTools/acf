#ifndef ibase_TFileSerializerComp_included
#define ibase_TFileSerializerComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "isys/IFileSystem.h"

#include "ibase/TLoggerCompWrap.h"
#include "ibase/CFileTypeInfoComp.h"


namespace ibase
{


/**
	Template implementation of file serializer using loading and storing archive implementation.
*/
template <class ReadArchive, class WriteArchive>
class TFileSerializerComp:
			public ibase::TLoggerCompWrap<CFileTypeInfoComp>,
			virtual public iser::IFileLoader
{
public:	
	typedef ibase::TLoggerCompWrap<CFileTypeInfoComp> BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xac10,
		MI_UNSUPPORTED_VERSION
	};

	I_BEGIN_COMPONENT(TFileSerializerComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

protected:
	class ReadArchiveEx: public ReadArchive
	{
	public:
		typedef ReadArchive BaseClass;

		ReadArchiveEx(const QString& filePath, const TFileSerializerComp* loggerPtr)
		:	ReadArchive(filePath),
			m_loggerPtr(loggerPtr)
		{
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformation::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = NULL) const
		{
			static const istd::IInformation::InformationCategory slaveCategory = istd::IInformation::IC_INFO;

			return (m_loggerPtr != NULL) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}
		virtual bool SendLogMessage(istd::IInformation::InformationCategory category, int id, const QString& message, const QString& messageSource, int flags = 0) const
		{
			if (m_loggerPtr != NULL){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(istd::IInformation::IC_INFO, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	private:
		const TFileSerializerComp* m_loggerPtr;
	};

	class WriteArchiveEx: public WriteArchive
	{
	public:
		typedef WriteArchive BaseClass;
                
		WriteArchiveEx(const QString& filePath, const iser::IVersionInfo* infoPtr, const TFileSerializerComp* loggerPtr)
		:	WriteArchive(filePath, infoPtr),
			m_loggerPtr(loggerPtr)
		{
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformation::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = NULL) const
		{
			static const istd::IInformation::InformationCategory slaveCategory = istd::IInformation::IC_INFO;

			return (m_loggerPtr != NULL) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}
		virtual bool SendLogMessage(istd::IInformation::InformationCategory category, int id, const QString& message, const QString& messageSource, int flags = 0) const
		{
			if (m_loggerPtr != NULL){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(category, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	private:
		const TFileSerializerComp* m_loggerPtr;
	};

	/**
		Get working version info.
	*/
	virtual const iser::IVersionInfo* GetVersionInfo() const;

	/**
		Called if read error is occured.
	*/
	virtual void OnReadError(const ReadArchive& archive, const istd::IChangeable& data, const QString& filePath) const;

	/**
		Check if minimal version of some serializable object is supported by version info.
	*/
	bool CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const;

	bool CheckInputFilePath(const QString filePath) const;

	bool CheckTargetDirectory(const QString dirPath) const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


// public methods

// reimplemented (iser::IFileLoader)

template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, QObject::tr("Object is not serializable"));
		}

		return false;
	}

	if ((flags & (QF_FILE)) == 0){
		return false;
	}

	if (filePathPtr != NULL){
		if ((flags & QF_LOAD) != 0){
			isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
			if (fileSystemPtr != NULL){
				if (!fileSystemPtr->IsPresent(*filePathPtr)){
					return false;
				}
			}
		}

		QStringList fileExtensions;
		if (GetFileExtensions(fileExtensions)){
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


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	if (!CheckInputFilePath(filePath)){
		return StateFailed;
	}

	if (IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		ReadArchiveEx archive(filePath, this);

		I_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		istd::CChangeNotifier changePtr(NULL, istd::IChangeable::CF_MODEL);

		if (serializablePtr->Serialize(archive)){
			changePtr.SetPtr(&data);

			return StateOk;
		}
		else{
			OnReadError(archive, data, filePath);
		}
	}

	return StateFailed;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (!CheckTargetDirectory(filePath)){
		return StateFailed;
	}

	if (IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		WriteArchiveEx archive(filePath, GetVersionInfo(), this);
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, QObject::tr("Archive version is not supported, possible lost of data"));
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, QObject::tr("Cannot serialize object to file"));
		}
	}

	return StateFailed;
}


// protected methods

template <class ReadArchive, class WriteArchive>
const iser::IVersionInfo* TFileSerializerComp<ReadArchive, WriteArchive>::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


template <class ReadArchive, class WriteArchive>
void TFileSerializerComp<ReadArchive, WriteArchive>::OnReadError(const ReadArchive& /*archive*/, const istd::IChangeable& /*data*/, const QString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, QString(QObject::tr("Cannot load object from file ")) + filePath);
}


template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int id = *iter;

		I_DWORD objectMinimalVersion = object.GetMinimalVersion(id);

		I_DWORD versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)){
			if (versionNumber < objectMinimalVersion){
				return false;
			}
		}
	}

	return true;
}


template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::CheckInputFilePath(const QString filePath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr == NULL){
		SendWarningMessage(0, "File path could not be checked");

		return true;
	}
	
	if (!fileSystemPtr->IsPresent(filePath)){
		SendWarningMessage(MI_CANNOT_LOAD, QString("File %1 does not exist").arg(filePath));

		return false;
	}
	
	return true;
}


template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::CheckTargetDirectory(const QString dirPath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr == NULL){
		SendWarningMessage(0, "Target directory path could not be checked");

		return true;
	}

	if (!fileSystemPtr->IsPresent(fileSystemPtr->GetDirPath(dirPath))){
		SendWarningMessage(MI_CANNOT_SAVE, QString("Save target directory %1 does not exist").arg(fileSystemPtr->GetDirPath(dirPath)));

		return false;
	}

	return true;
}

} // namespace ibase


#endif // !ibase_TFileSerializerComp_included


