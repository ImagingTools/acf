#include "iser/CMemoryWriteArchive.h"


// STD includes
#include <cstring>


// ACF includes
#include "istd/CString.h"


namespace iser
{


CMemoryWriteArchive::CMemoryWriteArchive(
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader)
:	BaseClass(versionInfoPtr), m_serializeHeader(serializeHeader)
{
	Reset();
}


const void* CMemoryWriteArchive::GetBuffer() const
{
	return &m_dataBuffer[0];
}


int CMemoryWriteArchive::GetBufferSize() const
{
	return int(m_dataBuffer.size());
}


void CMemoryWriteArchive::Reset()
{
	m_dataBuffer.clear();

	if (m_serializeHeader){
		SerializeAcfHeader();
	}
}


// reimplemented (iser::IArchive)

bool CMemoryWriteArchive::ProcessData(void* data, int size)
{
	if (size <= 0){
		return true;
	}

	if (data == NULL){
		return false;
	}

	DataBuffer::size_type previousSize = m_dataBuffer.size();

	m_dataBuffer.resize(previousSize + size);

	::memcpy(&m_dataBuffer[previousSize], data, size);

	return true;
}


} // namespace iser


