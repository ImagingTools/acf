#include "istd/CBitStream.h"


namespace istd
{


// public methods

CBitStream::CBitStream(const std::vector<I_BYTE>& data)
{
	m_stream = data;
	m_bitPosition = 0;
}


CBitStream::CBitStream()
{
	ResetPosition();
}


void CBitStream::Reset()
{
	m_stream.clear();
}


void CBitStream::ResetPosition()
{
	m_bitPosition = 0;
}


I_DWORD CBitStream::GetValue(int bits)
{
	if (IsEos()){
		return 0;
	}

	I_DWORD retVal = 0;

	for (int bitIndex = m_bitPosition, localBitIndex = 0; bitIndex < m_bitPosition + bits; bitIndex++, localBitIndex++){
		int localByteIndex = bitIndex / 8;
		int contextBitIndex = bitIndex % 8;
		int contextMask = 1 << ((8 - contextBitIndex) - 1);

		unsigned char value = m_stream.at(localByteIndex);
		value = value & contextMask ? 1 : 0;

		if (value){
			retVal |= 1 << (bits - localBitIndex - 1);
		}
	}

	m_bitPosition += bits;

	return retVal;
}
	

void CBitStream::PushBack(I_DWORD word, int bits)
{

}


bool CBitStream::IsEos() const
{
	int bytePosition = (m_bitPosition % 8) + m_bitPosition / 8;

	return bytePosition >= int(m_stream.size());
}


} // namespace istd

