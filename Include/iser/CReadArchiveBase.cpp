#include "iser/CReadArchiveBase.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CReadArchiveBase::IsStoring() const
{
	return false;
}


I_DWORD CReadArchiveBase::GetVersion(int versionId) const
{
	I_DWORD retVal;
	if (m_versionInfo.GetVersionNumber(versionId, retVal)){
		return retVal;
	}

	return 0;
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
	bool retVal = BeginTag(s_headerTag);

	int versionIdsCount = 0;
	retVal = retVal && BeginMultiTag(s_versionInfosTag, s_versionInfoTag, versionIdsCount);

	if (retVal){
		for (int i = 0; i < versionIdsCount; ++i){
			retVal = retVal && BeginTag(s_versionInfoTag);

			retVal = retVal && BeginTag(s_versionIdTag);
			int id = 0;
			retVal = retVal && Process(id);
			retVal = retVal && EndTag(s_versionIdTag);

			retVal = retVal && BeginTag(s_versionNumberTag);
			I_DWORD version = 0;
			retVal = retVal && Process(version);
			retVal = retVal && EndTag(s_versionNumberTag);

			retVal = retVal && BeginTag(s_versionDescriptionTag);
			istd::CString description;
			retVal = retVal && Process(description);
			retVal = retVal && EndTag(s_versionDescriptionTag);

			if (!retVal){
				return false;
			}

			m_versionInfo.InsertVersionId(id, version, description);

			retVal = retVal && EndTag(s_versionInfoTag);
		}
	}

	retVal = retVal && EndTag(s_versionInfosTag);

	retVal = retVal && EndTag(s_headerTag);

	return retVal;
}


} // namespace iser


