#include "iser/CWriteArchiveBase.h"


// ACF includes
#include "iser/TVersionInfoSerializer.h"


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
	if (bytesCount == int(sizeof(quint32))){
		quint32 mask = (1 << bitsCount) - 1;
		quint32 word = *((quint32*)dataPtr);
		quint32 result = word & mask;

		return Process(result);
	}

	return false;
}


// protected methods

CWriteArchiveBase::CWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	m_versionInfoPtr(versionInfoPtr)
{
}


bool CWriteArchiveBase::SerializeAcfHeader()
{	
	return TVersionInfoSerializer<IVersionInfo>::WriteVersion(m_versionInfoPtr, *this);
}


// public methods of embedded class EmptyVersionInfo

// reimplemented (iser::IVersionInfo)

bool CWriteArchiveBase::EmptyVersionInfo::GetVersionNumber(int /*versionId*/, quint32& result) const
{
	result = 0xffffffff;

	return false;
}


QString CWriteArchiveBase::EmptyVersionInfo::GetVersionIdDescription(int /*versionId*/) const
{
	return "";
}


iser::IVersionInfo::VersionIds CWriteArchiveBase::EmptyVersionInfo::GetVersionIds() const
{
	return iser::IVersionInfo::VersionIds();
}


QString CWriteArchiveBase::EmptyVersionInfo::GetEncodedVersionName(int /*versionId*/, quint32 /*versionNumber*/) const
{
	return "";
}


// static attributes

CWriteArchiveBase::EmptyVersionInfo CWriteArchiveBase::s_emptyVersionInfo;


} // namespace iser


