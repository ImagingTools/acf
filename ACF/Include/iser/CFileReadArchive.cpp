#include "iser/CFileReadArchive.h"


#include <assert.h>

#include "istd/CString.h"

#include "iser/CArchiveTag.h"


namespace iser
{


CFileReadArchive::CFileReadArchive(const istd::CString& filePath, bool supportTagSkipping, bool serializeHeader)
:	m_supportTagSkipping(supportTagSkipping)
{
	m_stream.open(filePath.ToString().c_str(), std::fstream::in | std::fstream::binary);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


bool CFileReadArchive::IsTagSkippingSupported() const
{
	return m_supportTagSkipping;
}


bool CFileReadArchive::BeginTag(const CArchiveTag& tag)
{
	bool retVal = BaseClass::BeginTag(tag);

	if (!retVal){
		return false;
	}

	m_tagStack.push_back(TagStackElement());
	TagStackElement& element = m_tagStack.back();

	element.tagBinaryId = tag.GetBinaryId();

	retVal = retVal && Process(element.endPosition);
	element.useTagSkipping = tag.IsTagSkippingUsed() && m_supportTagSkipping;

	return retVal;
}


bool CFileReadArchive::EndTag(const CArchiveTag& tag)
{
	TagStackElement& element = m_tagStack.back();

	bool retVal = (element.tagBinaryId == tag.GetBinaryId());

	if (!retVal){
		I_CRITICAL();	// BeginTag and EndTag have to use the same tag

		return false;
	}

	if (element.useTagSkipping && (element.endPosition != 0)){
		m_stream.seekg(element.endPosition);
	}

	m_tagStack.pop_back();

	return BaseClass::EndTag(tag);
}


bool CFileReadArchive::ProcessData(void* data, int size)
{
	if (data != NULL && size > 0){
		m_stream.read((char*)data, size);
	}

	return !m_stream.fail();
}


} // namespace iser


