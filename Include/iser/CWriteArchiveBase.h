#ifndef iser_CWriteArchiveBase_included
#define iser_CWriteArchiveBase_included


// ACF includes
#include "iser/IVersionInfo.h"
#include "iser/CArchiveBase.h"


namespace iser
{


/**
	Basic implementation of an abstract archive for data writting.

	\ingroup Persistence
*/
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
		virtual bool GetVersionNumber(int versionId, quint32& result) const;
		virtual QString GetVersionIdDescription(int versionId) const;
		virtual VersionIds GetVersionIds() const;
		virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;
	};

private:
	const IVersionInfo* m_versionInfoPtr;

	// static attributes
	static EmptyVersionInfo s_emptyVersionInfo;
};


} // namespace iser


#endif // !iser_CWriteArchiveBase_included


