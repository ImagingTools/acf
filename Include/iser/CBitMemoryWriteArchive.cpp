#include <iser/CBitMemoryWriteArchive.h>


namespace iser
{


// public methods

CBitMemoryWriteArchive::CBitMemoryWriteArchive(const IVersionInfo* versionInfoPtr, bool serializeHeader, int initialBufferSize)
:	BaseClass(versionInfoPtr, serializeHeader)
{
	m_bitPosition = 0;

	int oldSize = m_dataBuffer.size();

	if (initialBufferSize > oldSize){
		m_dataBuffer.resize(initialBufferSize);
		memset(m_dataBuffer.data() + oldSize, 0, initialBufferSize - oldSize);
	}

}


void CBitMemoryWriteArchive::WriteValue(quint32 inputValue, int bitsCount)
{
	Q_ASSERT(bitsCount >= 1);
	Q_ASSERT(bitsCount <= 32);

	int newBitPosition = m_bitPosition + bitsCount;
	int newSize = newBitPosition / 8 + ((newBitPosition % 8) != 0);
	int oldSize = m_dataBuffer.size();

	if(newSize > oldSize){
		m_dataBuffer.resize(newSize);
		memset(m_dataBuffer.data() + oldSize, 0, newSize - oldSize);
	}

	if (m_bitPosition % 8 == 0 && bitsCount % 8 == 0){
		int byteCount = bitsCount / 8;
		int bytePosition = m_bitPosition / 8;

		for (int i = 0; i < byteCount; i++){
			memset(m_dataBuffer.data() + bytePosition + i, ((unsigned char *)(&inputValue))[byteCount - (i + 1)], 1);
		}
	}
	else {
		for (int bitIndex = 0; bitIndex < bitsCount; bitIndex++){
			int localByteIndex = (m_bitPosition + bitIndex) / 8;
			int contextBitIndex = bitIndex % bitsCount;
			int globalBitIndex = (m_bitPosition + bitIndex) % 8;
			int contextMask = (1 << (bitsCount - contextBitIndex - 1));

			bool bitOn = (inputValue & contextMask) != 0;

			unsigned char& bufferValue = (unsigned char&)(*(m_dataBuffer.data() + localByteIndex));

			if (bitOn){
				bufferValue |= 1 << (8 - globalBitIndex - 1);
			}
			else{
				bufferValue &= ~(1 << (8 - globalBitIndex - 1));
			}
		}
	}

	m_bitPosition += bitsCount;
}


// reimplemented (iser::IArchive)

bool CBitMemoryWriteArchive::ProcessData(void* data, int size)
{
	bool retVal = BaseClass::ProcessData(data, size);
	if (retVal){
		m_bitPosition += size * 8;
	}

	return retVal;
}


bool CBitMemoryWriteArchive::ProcessBits(void* data, int bitsCount, int bytesCount)
{	
	Q_ASSERT(bitsCount <= bytesCount * 8); 

	if (bytesCount == int(sizeof(quint32))){
		WriteValue(*(quint32*)data, bitsCount);
	}

	return true;
}


} // namespace iser

