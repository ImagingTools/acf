#include "iser/CXmlWriteArchiveBase.h"


// STL includes
#include <sstream>


namespace iser
{


// reimplemented (iser::IArchive)

bool CXmlWriteArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CXmlWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	bool retVal = MakeIndent() && WriteString("<" + tag.GetId() + ">\n");

	++m_indent;

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlWriteArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int& count)
{
	std::ostringstream stream;
	stream << count;

	bool retVal = MakeIndent() && WriteString("<" + tag.GetId() + " count=\"" + stream.str() + "\">\n");

	++m_indent;

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlWriteArchiveBase::EndTag(const CArchiveTag& tag)
{
	--m_indent;

	return MakeIndent() && WriteString("</" + tag.GetId() + ">\n");
}


bool CXmlWriteArchiveBase::Process(std::string& value)
{
	bool retVal = true;

	if (m_isSeparatorNeeded){
		retVal = retVal && MakeIndent();
		retVal = retVal && WriteString("<" + GetElementSeparator().toStdString() + "/>\n");
	}
	else{
		m_isSeparatorNeeded = true;
	}

	retVal = retVal && MakeIndent();

	std::string xmlText;
	EncodeXml(value, xmlText);

	retVal = retVal && WriteString(xmlText) && WriteString("\n");

	return retVal;
}


bool CXmlWriteArchiveBase::Process(QString& value)
{
	bool retVal = true;

	if (m_isSeparatorNeeded){
		retVal = retVal && MakeIndent();
		retVal = retVal && WriteString("<" + GetElementSeparator().toStdString() + "/>\n");
	}
	else{
		m_isSeparatorNeeded = true;
	}

	retVal = retVal && MakeIndent();

	std::string xmlText;

	EncodeXml(value.toStdString(), xmlText);

	retVal = retVal && WriteString(xmlText) && WriteString("\n");

	return retVal;

}


// protected methods

CXmlWriteArchiveBase::CXmlWriteArchiveBase(const IVersionInfo* versionInfoPtr, const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	m_rootTag(rootTag),
	m_indent(0),
	m_isHeaderSerialized(false),
	m_isSeparatorNeeded(false)
{
}


bool CXmlWriteArchiveBase::Flush()
{
	if (m_isHeaderSerialized){
		m_isHeaderSerialized = false;

		return EndTag(m_rootTag);
	}

	return true;
}


bool CXmlWriteArchiveBase::MakeIndent()
{
	return WriteString(std::string(m_indent * 2, ' '));
}


bool CXmlWriteArchiveBase::SerializeXmlHeader()
{
	bool retVal = true;

	retVal = retVal && WriteString("<?xml version=\"1.0\"?>\n");
	retVal = retVal && BeginTag(m_rootTag);

	m_isHeaderSerialized = true;

	return retVal;
}


} // namespace iser


