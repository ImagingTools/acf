#include "iser/CWriteArchiveBase.h"


// ACF includes
#include "istd/CStaticServicesProvider.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CWriteArchiveBase::IsStoring() const
{
	return true;
}


const IVersionInfo& CWriteArchiveBase::GetVersionInfo() const
{
	if (m_versionInfoPtr != NULL){
		return *m_versionInfoPtr;
	}
	else{
		return s_emptyVersionInfo;
	}
}


bool CWriteArchiveBase::ProcessBits(void* dataPtr, int bitsCount, int bytesCount)
{
	if (bytesCount == int(sizeof(I_DWORD))){
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
		I_IF_DEBUG(I_DWORD dummyVersion;I_ASSERT(m_versionInfoPtr->GetVersionNumber(id, dummyVersion)));	// all known IDs must have its version.
		retVal = retVal && Process(id);
		retVal = retVal && EndTag(s_versionIdTag);

		retVal = retVal && BeginTag(s_versionNumberTag);
		I_DWORD versionNumber;
		m_versionInfoPtr->GetVersionNumber(id, versionNumber);
		retVal = retVal && Process(versionNumber);
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


// public methods of embedded class EmptyVersionInfo

// reimplemented (iser::IVersionInfo)

bool CWriteArchiveBase::EmptyVersionInfo::GetVersionNumber(int /*versionId*/, I_DWORD& result) const
{
	result = 0xffffffff;

	return false;
}


istd::CString CWriteArchiveBase::EmptyVersionInfo::GetVersionIdDescription(int /*versionId*/) const
{
	return "";
}


iser::IVersionInfo::VersionIds CWriteArchiveBase::EmptyVersionInfo::GetVersionIds() const
{
	return iser::IVersionInfo::VersionIds();
}


istd::CString CWriteArchiveBase::EmptyVersionInfo::GetEncodedVersionName(int /*versionId*/, I_DWORD /*versionNumber*/) const
{
	return "";
}


// static attributes

CWriteArchiveBase::EmptyVersionInfo CWriteArchiveBase::s_emptyVersionInfo;


} // namespace iser


