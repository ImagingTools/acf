#include "iser/CReadArchiveBase.h"


// ACF includes
#include "iser/TVersionInfoSerializer.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CReadArchiveBase::IsStoring() const
{
	return false;
}


const IVersionInfo& CReadArchiveBase::GetVersionInfo() const
{
	return m_versionInfo;
}


bool CReadArchiveBase::ProcessBits(void* dataPtr, int bitsCount, int bytesCount)
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

bool CReadArchiveBase::SerializeAcfHeader()
{
	return TVersionInfoSerializer<CReaderVersionInfo>::ReadVersion(&m_versionInfo, *this);
}


} // namespace iser


