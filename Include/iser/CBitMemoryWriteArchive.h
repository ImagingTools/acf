#ifndef iser_CBitMemoryWriteArchive_included
#define iser_CBitMemoryWriteArchive_included


// ACF includes
#include "iser/CMemoryWriteArchive.h"


namespace iser
{


/**
	This is an archive implementation which provides bitwise write of data.

	\ingroup Persistence
*/
class CBitMemoryWriteArchive: public CMemoryWriteArchive
{
public:
	typedef CMemoryWriteArchive BaseClass;

	CBitMemoryWriteArchive(const IVersionInfo* versionInfoPtr = NULL, bool serializeHeader = true);

	/**
		Pushes a value of the \c word bounding by N \c bits into the stream.
	*/
	void WriteValue(I_DWORD inputValue, int bits);

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	virtual bool ProcessBits(void* data, int bitsCount, int bytesCount);

protected:
	int m_bitPosition;
};



} // namespace iser


#endif // !iser_CBitMemoryWriteArchive_included

