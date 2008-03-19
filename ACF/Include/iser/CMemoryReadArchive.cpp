#include "iser/CMemoryReadArchive.h"


#include "istd/CString.h"

#include "iser/CMemoryWriteArchive.h"


namespace iser
{


CMemoryReadArchive::CMemoryReadArchive(
			const void* bufferPtr,
			int bufferSize,
			bool serializeHeader)
:	m_bufferPtr((const I_BYTE*)bufferPtr),
	m_bufferSize(bufferSize),
	m_readPosition(0),
	m_isValid(true)
{
	if (serializeHeader){
		m_isValid = SerializeAcfHeader();
	}
}


CMemoryReadArchive::CMemoryReadArchive(
			const CMemoryWriteArchive& writeArchive,
			bool serializeHeader)
:	m_bufferPtr((const I_BYTE*)writeArchive.GetBuffer()),
	m_bufferSize(writeArchive.GetBufferSize()),
	m_readPosition(0),
	m_isValid(true)
{
	if (serializeHeader){
		m_isValid = SerializeAcfHeader();
	}
}


void CMemoryReadArchive::ResetPosition()
{
	m_readPosition = 0;

	if (m_bufferSize > 0){
		m_isValid = true;
	}
}


bool CMemoryReadArchive::IsValid() const
{
	return m_isValid;
}


// reimplemented (iser::IArchive)

bool CMemoryReadArchive::ProcessData(void* data, int size)
{
	if (m_readPosition + size > m_bufferSize){
		m_isValid = false;
	}

	if (!m_isValid){
		return false;
	}

	::memcpy(data, m_bufferPtr + m_readPosition, size);
	m_readPosition += size;

	return true;
}


} // namespace iser


