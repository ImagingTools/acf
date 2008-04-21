#ifndef istdc_TFileSerializerComp_included
#define istdc_TFileSerializerComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"


namespace istdc
{


template <class ReadArchive, class WriteArchive>
class TFileSerializerComp: public icomp::CComponentBase, public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(TFileSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual const istd::CString& GetLastLoadFileName() const;
	virtual const istd::CString& GetLastSaveFileName() const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


// public methods

// reimplemented (iser::IFileLoader)

template <class ReadArchive, class WriteArchive>
bool TFileSerializerComp<ReadArchive, WriteArchive>::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* /*filePathPtr*/,
			bool /*forLoading*/,
			bool /*forSaving*/) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		return false;
	}

	return true;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const
{
	if (!IsOperationSupported(&data, &filePath, true, false)){
		return StateFailed;
	}

	ReadArchive archive(filePath);
	I_ASSERT(!archive.IsStoring());

	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
	I_ASSERT(serializablePtr != NULL);

	if (serializablePtr->Serialize(archive)){
		return StateOk;
	}
	else{
		return StateFailed;
	}
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const
{
	if (!IsOperationSupported(&data, &filePath, false, true)){
		return StateFailed;
	}

	WriteArchive archive(filePath, m_versionInfoCompPtr.GetPtr());
	I_ASSERT(archive.IsStoring());

	const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
	I_ASSERT(serializablePtr != NULL);

	if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
		return StateOk;
	}
	else{
		return StateFailed;
	}
}


template <class ReadArchive, class WriteArchive>
const istd::CString& TFileSerializerComp<ReadArchive, WriteArchive>::GetLastLoadFileName() const
{
	static istd::CString emptyPath;

	return emptyPath;
}


template <class ReadArchive, class WriteArchive>
const istd::CString& TFileSerializerComp<ReadArchive, WriteArchive>::GetLastSaveFileName() const
{
	static istd::CString emptyPath;

	return emptyPath;
}


} // namespace istdc


#endif // !istdc_TFileSerializerComp_included
