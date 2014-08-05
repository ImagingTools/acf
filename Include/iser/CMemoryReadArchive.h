#ifndef iser_CMemoryReadArchive_included
#define iser_CMemoryReadArchive_included


// ACF includes
#include "iser/CBinaryReadArchiveBase.h"


namespace iser
{


class ISerializable;
class CMemoryWriteArchive;


/**
	Implementation of archive using memory buffer to read the persistent objects.
	Internal format of this buffer is compatible with class \c iser::CMemoryWriteArchive.

	\ingroup Persistence
*/
class CMemoryReadArchive: public CBinaryReadArchiveBase
{
public:
	typedef CBinaryReadArchiveBase BaseClass;

	CMemoryReadArchive(
				const void* bufferPtr,
				int bufferSize,
				bool serializeHeader = true);
	CMemoryReadArchive(
				const CMemoryWriteArchive& writeArchive,
				bool serializeHeader = true);

	/**
		Seeks internal cursor to the begin of data.
	*/
	virtual void ResetPosition();

	/**
		Returns \c true if the archive is in valid state 
		and internal position cursor has not reached end of archive data
	*/
	virtual bool IsValid() const;

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);

	// static methods
	/**
		Clone serializable objects using \c iser::CMemoryWriteArchive and \c iser::CMemoryReadArchive archives.
	*/
	static bool CloneObjectByArchive(const ISerializable& source, ISerializable& result);
	
protected:
	// reimplemented (iser::CArchiveBase)
	virtual int GetMaxStringLength() const;

	const quint8* m_bufferPtr;
	int m_bufferSize;
	int m_readPosition;
	bool m_isValid;
};


} // namespace iser


#endif // !iser_CMemoryReadArchive_included


