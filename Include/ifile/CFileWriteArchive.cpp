#include <ifile/CFileWriteArchive.h>


// Qt includes
#include <QtCore/QString>


namespace ifile
{


CFileWriteArchive::CFileWriteArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool supportTagSkipping,
			bool serializeHeader)
:	BaseClass(versionInfoPtr),
	BaseClass2(filePath),
	m_file(filePath),
	m_supportTagSkipping(supportTagSkipping),
	m_isValid(false)
{
	m_isValid = m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


void CFileWriteArchive::Flush()
{
	m_file.flush();
}


// reimplemented (iser::IArchive)

bool CFileWriteArchive::IsTagSkippingSupported() const
{
	return m_supportTagSkipping;
}


bool CFileWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	bool retVal = m_isValid && BaseClass::BeginTag(tag);

	if (!retVal){
		return false;
	}

	m_tagStack.push_back(TagStackElement());
	TagStackElement& element = m_tagStack.back();

	element.tagBinaryId = tag.GetBinaryId();
	element.endFieldPosition = (tag.IsTagSkippingUsed() && m_supportTagSkipping)?
				quint32(m_file.pos()):
				quint32(0);

	quint32 dummyPos = 0;
	retVal = retVal && Process(dummyPos);

	return retVal;
}


bool CFileWriteArchive::EndTag(const iser::CArchiveTag& tag)
{
	TagStackElement& element = m_tagStack.back();

	bool retVal = (element.tagBinaryId == tag.GetBinaryId());

	if (!retVal){
		qFatal("BeginTag and EndTag have to use the same tag");

		return false;
	}

	if (element.endFieldPosition != 0){	// add position of the file tag end to the tag begin
		quint32 endPosition = m_file.pos();

		retVal = retVal && m_file.seek(element.endFieldPosition);

		retVal = retVal && Process(endPosition);

		retVal = retVal && m_file.seek(endPosition);
	}

	m_tagStack.pop_back();

	return retVal && BaseClass::EndTag(tag);
}


bool CFileWriteArchive::ProcessData(void* data, int size)
{
	if (size <= 0){
		return true;
	}

	if (data == NULL){
		return false;
	}

	m_file.write((char*)data, size);

	return m_file.error() == QFile::NoError;
}


} // namespace ifile

