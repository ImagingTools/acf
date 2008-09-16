#include "iser/CBitMemoryReadArchive.h"


namespace iser
{


// public methods

CBitMemoryReadArchive::CBitMemoryReadArchive(const std::vector<I_BYTE>& data, bool serializeHeader)
	:BaseClass(&data[0], data.size(), serializeHeader)	
{
	m_bitPosition = 0;
}


CBitMemoryReadArchive::CBitMemoryReadArchive(const void* bufferPtr, int bufferSize, bool serializeHeader)
	:BaseClass(bufferPtr, bufferSize, serializeHeader)	
{
	m_bitPosition = 0;
}


CBitMemoryReadArchive::CBitMemoryReadArchive(const CMemoryWriteArchive& writeArchive, bool serializeHeader)
	:BaseClass(writeArchive, serializeHeader)	
{
	m_bitPosition = 0;
}


I_DWORD CBitMemoryReadArchive::ReadValue(int bits)
{
	if (!IsValid()){
		return 0;
	}

	I_DWORD retVal = 0;

	for (int bitIndex = m_bitPosition, localBitIndex = 0; bitIndex < m_bitPosition + bits; bitIndex++, localBitIndex++){
		int localByteIndex = bitIndex / 8;
		int contextBitIndex = bitIndex % 8;
		int contextMask = 1 << ((8 - contextBitIndex) - 1);

		bool bitOn = (*(m_bufferPtr + localByteIndex) & contextMask) != 0;
		if (bitOn){
			retVal |= 1 << (bits - localBitIndex - 1);
		}
	}

	m_bitPosition += bits;

	return retVal;
}


// reimplemented (iser::IArchive)

bool CBitMemoryReadArchive::ProcessData(void* data, int size)
{
	bool retVal = BaseClass::ProcessData(data, size);
	if (retVal){
		m_bitPosition += size * 8;
	}

	return retVal;
}


bool CBitMemoryReadArchive::ProcessBits(void* dataPtr, int bitsCount, int bytesCount)
{
	I_ASSERT(bytesCount >= 1);

	if (bytesCount > 4){
		return false;
	}

	I_DWORD retVal = ReadValue(bitsCount);

	memcpy(dataPtr, &retVal, bytesCount);

	return true;
}


// reimplemented (iser::CMemoryReadArchive)

void CBitMemoryReadArchive::ResetPosition()
{
	m_bitPosition = 0;

	BaseClass::ResetPosition();
}


bool CBitMemoryReadArchive::IsValid() const
{
	int bytePosition = ((m_bitPosition % 8) != 0) + m_bitPosition / 8;

	return bytePosition < m_bufferSize;
}


} // namespace iser

