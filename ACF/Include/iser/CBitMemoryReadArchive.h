#ifndef iser_CBitMemoryReadArchive_included
#define iser_CBitMemoryReadArchive_included


#include "iser/iser.h"


#include "iser/CMemoryReadArchive.h"


namespace iser
{


/**
	This class provides bitwise access to the byte data.
*/
class CBitMemoryReadArchive: public iser::CMemoryReadArchive
{
public:
	typedef iser::CMemoryReadArchive BaseClass;

	/**
		Constructs a bit stream from a vector of bytes. 
	*/
	CBitMemoryReadArchive(const std::vector<I_BYTE>& data, 	bool serializeHeader = true);

	CBitMemoryReadArchive(
				const void* bufferPtr,
				int bufferSize,
				bool serializeHeader = true);

	CBitMemoryReadArchive(
				const CMemoryWriteArchive& writeArchive,
				bool serializeHeader = true);

	/**
		Gets the value of the next \c bits in the stream.
	*/
	virtual I_DWORD ReadValue(int bits);

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* data, int size);
	virtual bool ProcessBits(void* dataPtr, int bitsCount, int bytesCount);

	// reimplemented (iser::CMemoryReadArchive)
	virtual void ResetPosition();
	virtual bool IsValid() const;

protected:
	int m_bitPosition;
};



} // namespace iser


#endif // !iser_CBitMemoryReadArchive_included

