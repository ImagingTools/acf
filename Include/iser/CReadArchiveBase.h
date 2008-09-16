#ifndef iser_CReadArchiveBase_included
#define iser_CReadArchiveBase_included


#include "iser/CArchiveBase.h"
#include "iser/CReaderVersionInfo.h"


namespace iser
{


class CReadArchiveBase: public CArchiveBase
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsStoring() const;
	virtual I_DWORD GetVersion(int versionId = IVersionInfo::UserVersionId) const;
	virtual bool ProcessBits(void* dataPtr, int bitsCount, int bytesCount);

protected:
	/**
		Serialize standard header.
		During serialization of header list of known versions will be loaded.
	*/
	virtual bool SerializeAcfHeader();

private:
	CReaderVersionInfo m_versionInfo;
};


} // namespace iser


#endif // !iser_CReadArchiveBase_included


