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
	virtual const IVersionInfo& GetVersionInfo() const;
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

	class EmptyVersionInfo: virtual public IVersionInfo
	{
	public:
		// reimplemented (iser::IVersionInfo)
		virtual bool GetVersionNumber(int versionId, I_DWORD& result) const;
		virtual istd::CString GetVersionIdDescription(int versionId) const;
		virtual VersionIds GetVersionIds() const;
	};

private:
	const IVersionInfo* m_versionInfoPtr;

	// static attributes
	static EmptyVersionInfo s_emptyVersionInfo;
};


} // namespace iser


#endif // !iser_CWriteArchiveBase_included


