#include "iser/CFileWriteArchive.h"


#include "istd/CString.h"


namespace iser
{


CFileWriteArchive::CFileWriteArchive(
			const istd::CString& fileName,
			bool supportTagSkipping,
			const IVersionInfo* versionInfoPtr,
			bool serializeHeader)
:	BaseClass(versionInfoPtr), m_supportTagSkipping(supportTagSkipping)
{
	m_stream.open(fileName.ToString().c_str(), std::fstream::out | std::fstream::binary);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


void CFileWriteArchive::Flush()
{
	m_stream.flush();
}


// reimplemented (iser::IArchive)

bool CFileWriteArchive::IsTagSkippingSupported() const
{
	return true;
}


bool CFileWriteArchive::BeginTag(const CArchiveTag& tag)
{
	bool retVal = BaseClass::BeginTag(tag);

	if (!retVal){
		return false;
	}

	m_tagStack.push_back(TagStackElement());
	TagStackElement& element = m_tagStack.back();

	element.tagBinaryId = tag.GetBinaryId();
	element.endFieldPosition = (tag.IsTagSkippingUsed() && m_supportTagSkipping)? m_stream.tellp(): 0;

	I_DWORD dummyPos = 0;
	retVal = retVal && Process(dummyPos);

	return retVal;
}


bool CFileWriteArchive::EndTag(const CArchiveTag& tag)
{
	TagStackElement& element = m_tagStack.back();

	bool retVal = (element.tagBinaryId == tag.GetBinaryId());

	if (!retVal){
		I_CRITICAL();	// BeginTag and EndTag have to use the same tag

		return false;
	}

	if (element.endFieldPosition != 0){	// add position of the file tag end to the tag begin
		I_DWORD endPosition = m_stream.tellp();

		m_stream.seekp(element.endFieldPosition);

		retVal = retVal && Process(endPosition);

		m_stream.seekp(endPosition);
	}

	m_tagStack.pop_back();

	return retVal && BaseClass::EndTag(tag);
}


bool CFileWriteArchive::ProcessData(void* data, int size)
{
	if (data != NULL && size > 0){
		m_stream.write((char*)data, size);
	}

	return !m_stream.fail();
}


} // namespace iser

