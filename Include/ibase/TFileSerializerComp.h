#ifndef istdc_TFileSerializerComp_included
#define istdc_TFileSerializerComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


template <class ReadArchive, class WriteArchive>
class TFileSerializerComp:
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			public iser::IFileLoader
{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(TFileSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN_MULTI_0(m_extensionsAttrPtr, "Extensions", "List of supported file extensions", false);
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;

protected:
	/**
		Get working version info.
	*/
	virtual const iser::IVersionInfo* GetVersionInfo() const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_MULTIATTR(istd::CString, m_extensionsAttrPtr);
};


// public methods

// reimplemented (iser::IFileLoader)

template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* /*filePathPtr*/,
			bool /*forLoading*/,
			bool /*forSaving*/,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	return true;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const
{
	if (IsOperationSupported(&data, &filePath, true, false, false)){
		ReadArchive archive(filePath);
		I_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (serializablePtr->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_LOAD, "Cannot serialize object from file");
		}
	}

	return StateFailed;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const
{
	if (IsOperationSupported(&data, &filePath, false, true, false)){
		WriteArchive archive(filePath, GetVersionInfo());
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

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
bool TFileSerializerComp<ReadArchive, WriteArchive>::GetFileExtensions(istd::CStringList& result, bool doAppend) const
{
	if (m_extensionsAttrPtr.IsValid()){
		if (!doAppend){
			result.clear();
		}

		int extensionsCount = m_extensionsAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const istd::CString& extension = m_extensionsAttrPtr[i];

			result.push_back(extension);
		}

		return true;
	}

	return false;
}


// protected methods

template <class ReadArchive, class WriteArchive>
const iser::IVersionInfo* TFileSerializerComp<ReadArchive, WriteArchive>::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


} // namespace ibase


#endif // !istdc_TFileSerializerComp_included
