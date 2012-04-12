#include "iser/CXmlWriteArchiveBase.h"


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
	bool retVal = MakeIndent() && WriteString("<" + tag.GetId() + " count=\"" + QByteArray::number(count) + "\">\n");

	++m_indent;

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlWriteArchiveBase::EndTag(const CArchiveTag& tag)
{
	--m_indent;

	return MakeIndent() && WriteString("</" + tag.GetId() + ">\n");
}


bool CXmlWriteArchiveBase::Process(QByteArray& value)
{
	bool retVal = true;

	if (m_isSeparatorNeeded){
		retVal = retVal && MakeIndent();
		retVal = retVal && WriteString("<" + GetElementSeparator().toLocal8Bit() + "/>\n");
	}
	else{
		m_isSeparatorNeeded = true;
	}

	retVal = retVal && MakeIndent();

	QByteArray xmlText;
	EncodeXml(value, xmlText);

	retVal = retVal && WriteString(xmlText) && WriteString("\n");

	return retVal;
}


bool CXmlWriteArchiveBase::Process(QString& value)
{
	bool retVal = true;

	if (m_isSeparatorNeeded){
		retVal = retVal && MakeIndent();
		retVal = retVal && WriteString("<" + GetElementSeparator().toLocal8Bit() + "/>\n");
	}
	else{
		m_isSeparatorNeeded = true;
	}

	retVal = retVal && MakeIndent();

	QByteArray xmlText;

	EncodeXml(value.toLocal8Bit(), xmlText);

	retVal = retVal && WriteString(xmlText) && WriteString("\n");

	return retVal;

}


// protected methods

CXmlWriteArchiveBase::CXmlWriteArchiveBase(const IVersionInfo* versionInfoPtr, const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr),
	m_rootTag(rootTag),
	m_indent(0),
	m_isSeparatorNeeded(false)
{
}


bool CXmlWriteArchiveBase::MakeIndent()
{
	return WriteString(QByteArray(m_indent * 2, ' '));
}


bool CXmlWriteArchiveBase::WriteXmlHeader()
{
	bool retVal = true;

	retVal = retVal && WriteString("<?xml version=\"1.0\"?>\n");
	retVal = retVal && BeginTag(m_rootTag);

	return retVal;
}


bool CXmlWriteArchiveBase::WriteXmlFooter()
{
	return EndTag(m_rootTag);
}


} // namespace iser


