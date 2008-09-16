#include "iser/CWriteArchiveBase.h"


#include "istd/CStaticServicesProvider.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CWriteArchiveBase::IsStoring() const
{
	return true;
}


I_DWORD CWriteArchiveBase::GetVersion(int versionId) const
{
	if (m_versionInfoPtr != NULL){
		return m_versionInfoPtr->GetVersion(versionId);
	}

	return I_DWORD(IVersionInfo::UnknownVersion);
}


bool CWriteArchiveBase::ProcessBits(void* dataPtr, int bitsCount, int bytesCount)
{
	if (bytesCount == sizeof(I_DWORD)){
		I_DWORD mask = (1 << bitsCount) - 1;
		I_DWORD word = *((I_DWORD*)dataPtr);
		I_DWORD result = word & mask;

		return Process(result);
	}

	return false;
}


// protected methods

CWriteArchiveBase::CWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	m_versionInfoPtr(versionInfoPtr)
{
	if (m_versionInfoPtr == NULL){
		m_versionInfoPtr = istd::GetService<IVersionInfo>();
	}
}


bool CWriteArchiveBase::SerializeAcfHeader()
{
	I_ASSERT(IsStoring());

	bool retVal = BeginTag(s_headerTag);

	IVersionInfo::VersionIds ids;

	if (m_versionInfoPtr != NULL){
		ids = m_versionInfoPtr->GetVersionIds();
	}

	int versionIdsCount = int(ids.size());

	retVal = retVal && BeginMultiTag(s_versionInfosTag, s_versionInfoTag, versionIdsCount);

	for (		IVersionInfo::VersionIds::iterator iter = ids.begin();
				retVal && (iter != ids.end());
				++iter){
		I_ASSERT(m_versionInfoPtr != NULL);

		retVal = retVal && BeginTag(s_versionInfoTag);

		retVal = retVal && BeginTag(s_versionIdTag);
		int id = *iter;
		retVal = retVal && Process(id);
		retVal = retVal && EndTag(s_versionIdTag);

		retVal = retVal && BeginTag(s_versionNumberTag);
		I_DWORD version = m_versionInfoPtr->GetVersion(id);
		retVal = retVal && Process(version);
		retVal = retVal && EndTag(s_versionNumberTag);

		retVal = retVal && BeginTag(s_versionDescriptionTag);
		istd::CString description = m_versionInfoPtr->GetVersionIdDescription(id);
		retVal = retVal && Process(description);
		retVal = retVal && EndTag(s_versionDescriptionTag);

		retVal = retVal && EndTag(s_versionInfoTag);
	}

	retVal = retVal && EndTag(s_versionInfosTag);

	retVal = retVal && EndTag(s_headerTag);

	return retVal;
}


} // namespace iser


