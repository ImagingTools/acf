#ifndef istdc_TFileSerializerComp_included
#define istdc_TFileSerializerComp_included


#include "istd/TSingleFactory.h"

#include "icomp/CComponentBase.h"


namespace istdc
{


template <class ReadArchive, class WriteArchive>
class TFileSerializerComp: public icomp::CComponentBase, public iser::IFileSerializer
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(TFileSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileSerializer)
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
	I_END_COMPONENT

	// reimplemented (iser::IFileSerializer)
	virtual int LoadFromFile(iser::ISerializable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const iser::ISerializable& data, const istd::CString& filePath = istd::CString()) const;
	virtual const istd::CString& GetLastSaveFileName() const;
	virtual const istd::CString& GetLastOpenFileName() const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


// public methods

// reimplemented (iser::IFileSerializer)

template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(iser::ISerializable& data, const istd::CString& filePath = istd::CString()) const
{
	ReadArchive archive(filePath);
	I_ASSERT(!archive.IsStoring());

	if (data.Serialize(archive)){
		return StateOk;
	}
	else{
		return StateFailed;
	}
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(const iser::ISerializable& data, const istd::CString& filePath = istd::CString()) const
{
	WriteArchive archive(filePath, m_versionInfoCompPtr.GetPtr());
	I_ASSERT(archive.IsStoring());

	if (const_cast<iser::ISerializable&>(data).Serialize(archive)){
		return StateOk;
	}
	else{
		return StateFailed;
	}
}


template <class ReadArchive, class WriteArchive>
const istd::CString& TFileSerializerComp<ReadArchive, WriteArchive>::GetLastSaveFileName() const
{
	static istd::CString emptyPath;

	return emptyPath;
}


template <class ReadArchive, class WriteArchive>
const istd::CString& TFileSerializerComp<ReadArchive, WriteArchive>::GetLastOpenFileName() const
{
	static istd::CString emptyPath;

	return emptyPath;
}


} // namespace istdc


#endif // !istdc_TFileSerializerComp_included
