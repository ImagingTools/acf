#include <iser/CMemoryReadArchive.h>


// STD includes
#include <cstring>


// ACF includes
#include <iser/ISerializable.h>
#include <iser/CMemoryWriteArchive.h>


namespace iser
{


CMemoryReadArchive::CMemoryReadArchive(
			const void* bufferPtr,
			int bufferSize,
			bool serializeHeader)
:	m_bufferPtr((const quint8*)bufferPtr),
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
:	m_bufferPtr((const quint8*)writeArchive.GetBuffer()),
	m_bufferSize(writeArchive.GetBufferSize()),
	m_readPosition(0),
	m_startPosition(0),
	m_isValid(true)
{
	if (serializeHeader){
		m_isValid = SerializeAcfHeader();

		m_startPosition = m_readPosition;
	}
}


void CMemoryReadArchive::ResetPosition()
{
	m_readPosition = m_startPosition;

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

	std::memcpy(data, m_bufferPtr + m_readPosition, size);
	m_readPosition += size;

	return true;
}


// static methods

bool CMemoryReadArchive::CloneObjectByArchive(const ISerializable& source, ISerializable& result)
{
	bool retVal = true;
	CMemoryWriteArchive stored;
	retVal = retVal && const_cast<ISerializable&>(source).Serialize(stored);

	CMemoryReadArchive restore(stored);
	retVal = retVal && result.Serialize(restore);

	return retVal;
}


// protected methods

// reimplemented (iser::CArchiveBase)

int CMemoryReadArchive::GetMaxStringLength() const
{
	return m_bufferSize - m_readPosition;
}


} // namespace iser


