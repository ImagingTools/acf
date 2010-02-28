#ifndef iser_CMemoryWriteArchive_included
#define iser_CMemoryWriteArchive_included


// STL includes
#include <vector>


// ACF includes
#include "iser/CBinaryWriteArchiveBase.h"


namespace iser
{


/**
	Implementation of archive using memory buffer to store the persistent objects.
	Internal format of this buffer is compatible with class \c iser::CMemoryReadArchive.

	\ingroup Persistence
*/
class CMemoryWriteArchive: public CBinaryWriteArchiveBase
{
public:
	typedef CBinaryWriteArchiveBase BaseClass;

	CMemoryWriteArchive(
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true);

	const void* GetBuffer() const;
	int GetBufferSize() const;

	/**
		Reset internal buffer.
	*/
	void Reset();

	bool operator==(const CMemoryWriteArchive& archive) const;
	bool operator!=(const CMemoryWriteArchive& archive) const;

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	
protected:
	typedef std::vector<unsigned char> DataBuffer;

	DataBuffer m_dataBuffer;

	bool m_serializeHeader;
};


// inline methods

inline bool CMemoryWriteArchive::operator==(const CMemoryWriteArchive& archive) const
{
	return m_dataBuffer == archive.m_dataBuffer;
}


inline bool CMemoryWriteArchive::operator!=(const CMemoryWriteArchive& archive) const
{
	return m_dataBuffer != archive.m_dataBuffer;
}


} // namespace iser


#endif // !iser_CMemoryWriteArchive_included


