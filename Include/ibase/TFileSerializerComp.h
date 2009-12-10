#ifndef istdc_TFileSerializerComp_included
#define istdc_TFileSerializerComp_included


#include "istd/CStaticServicesProvider.h"
#include "istd/TChangeNotifier.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "isys/IFileSystem.h"

#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


/**
	Template implementation of file serializer using loading and storing archive implementation.
*/
template <class ReadArchive, class WriteArchive>
class TFileSerializerComp:
			public ibase::CLoggerComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xac10,
		MI_UNSUPPORTED_VERSION
	};

	I_BEGIN_COMPONENT(TFileSerializerComp);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN_MULTI_0(m_fileExtensionsAttrPtr, "FileExtensions", "List of supported file extensions", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsAttrPtr, "TypeDescriptions", "List of descriptions for each extension", false);
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

protected:
	class ReadArchiveEx: public ReadArchive
	{
	public:
		ReadArchiveEx(const istd::CString& filePath, const TFileSerializerComp* loggerPtr)
		:	ReadArchive(filePath),
			m_loggerPtr(loggerPtr)
		{
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const MessageCategory* categoryPtr = NULL,
					const int* flagsPtr = NULL) const
		{
			return (m_loggerPtr != NULL) && m_loggerPtr->IsLogConsumed(categoryPtr, flagsPtr);
		}
		virtual bool SendLogMessage(MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource, int flags = 0) const
		{
			if (m_loggerPtr != NULL){
				istd::CString correctedMessage = message;
				istd::CString correctedMessageSource = messageSource;

				DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(category, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	private:
		const TFileSerializerComp* m_loggerPtr;
	};

	class WriteArchiveEx: public WriteArchive
	{
	public:
		WriteArchiveEx(const istd::CString& filePath, const iser::IVersionInfo* infoPtr, const TFileSerializerComp* loggerPtr)
		:	WriteArchive(filePath, infoPtr),
			m_loggerPtr(loggerPtr)
		{
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const MessageCategory* categoryPtr = NULL,
					const int* flagsPtr = NULL) const
		{
			return (m_loggerPtr != NULL) && m_loggerPtr->IsLogConsumed(categoryPtr, flagsPtr);
		}
		virtual bool SendLogMessage(MessageCategory category, int id, const istd::CString& message, const istd::CString& messageSource, int flags = 0) const
		{
			if (m_loggerPtr != NULL){
				istd::CString correctedMessage = message;
				istd::CString correctedMessageSource = messageSource;

				DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

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
	virtual void OnReadError(const ReadArchive& archive, const istd::IChangeable& data, const istd::CString& filePath) const;

	/**
		Check if minimal version of some serializable object is supported by version info.
	*/
	bool CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_MULTIATTR(istd::CString, m_fileExtensionsAttrPtr);
	I_MULTIATTR(istd::CString, m_typeDescriptionsAttrPtr);
};


// public methods

// reimplemented (iser::IFileLoader)

template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & QF_ANONYMOUS_ONLY) != 0){
		return false;
	}

	if (filePathPtr != NULL){
		if ((flags & QF_NO_SAVING) != 0){
			isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
			if (fileSystemPtr != NULL){
				if (!fileSystemPtr->IsPresent(*filePathPtr)){
					return false;
				}
			}
		}

		if (m_fileExtensionsAttrPtr.IsValid()){
			int extensionsCount = m_fileExtensionsAttrPtr.GetCount();
			for (int i = 0; i < extensionsCount; ++i){
				const istd::CString& extension = m_fileExtensionsAttrPtr[i];
				if (filePathPtr->substr(filePathPtr->length() - extension.length() - 1) == istd::CString(".") + extension.ToLower()){
					return true;
				}
			}

			if (!beQuiet){
				SendInfoMessage(MI_BAD_EXTENSION, "File extension is not supported");
			}

			return false;
		}
	}

	return true;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
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
int TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
		WriteArchiveEx archive(filePath, GetVersionInfo(), this);
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, "Archive version is not supported, possible lost of data");
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, "Cannot serialize object to file");
		}
	}

	return StateFailed;
}


template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::GetFileExtensions(istd::CStringList& result, int /*flags*/, bool doAppend) const
{
	if (m_fileExtensionsAttrPtr.IsValid()){
		if (!doAppend){
			result.clear();
		}

		int extensionsCount = m_fileExtensionsAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const istd::CString& extension = m_fileExtensionsAttrPtr[i];

			result.push_back(extension);
		}

		return true;
	}
	else{
		return false;
	}
}


template <class ReadArchive, class WriteArchive>
istd::CString TFileSerializerComp<ReadArchive, WriteArchive>::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = istd::Min(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsAttrPtr[extIndex];
			}
		}
	}

	if (m_typeDescriptionsAttrPtr.GetCount() > 0){
		return m_typeDescriptionsAttrPtr[0];
	}

	return "";
}


// protected methods

template <class ReadArchive, class WriteArchive>
const iser::IVersionInfo* TFileSerializerComp<ReadArchive, WriteArchive>::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


template <class ReadArchive, class WriteArchive>
void TFileSerializerComp<ReadArchive, WriteArchive>::OnReadError(const ReadArchive& /*archive*/, const istd::IChangeable& /*data*/, const istd::CString& filePath) const
{
	this->SendInfoMessage(MI_CANNOT_LOAD, istd::CString("Cannot load object from file ") + filePath);
}


template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int id = *iter;

		I_DWORD minimalVersionInfo = object.GetMinimalVersion(id);

		I_DWORD versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)){
			if (versionNumber < minimalVersionInfo){
				return false;
			}
		}
	}

	return true;
}


} // namespace ibase


#endif // !istdc_TFileSerializerComp_included
