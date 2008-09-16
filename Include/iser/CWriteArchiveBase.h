#ifndef iser_CWriteArchiveBase_included
#define iser_CWriteArchiveBase_included


#include "iser/CArchiveBase.h"


namespace iser
{


class CWriteArchiveBase: public CArchiveBase
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsStoring() const;
	virtual I_DWORD GetVersion(int versionId = IVersionInfo::UserVersionId) const;
	virtual bool ProcessBits(void* dataPtr, int bitsCount, int bytesCount);

protected:
	/**
		Constructor.
		Please note no header will be serialized during contruction.
		You have to explicite call method SerializeAcfHeader in your derrived implementation.
	*/
	CWriteArchiveBase(const IVersionInfo* versionInfoPtr);

	/**
		Serialize standard header.
		During serialization of header list of known versions will be loaded.
	*/
	virtual bool SerializeAcfHeader();

private:
	const IVersionInfo* m_versionInfoPtr;
};


} // namespace iser


#endif // !iser_CWriteArchiveBase_included


