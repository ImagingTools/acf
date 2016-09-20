#include <iser/CXmlReadArchiveBase.h>


namespace iser
{


// reimplemented (iser::IArchive)

bool CXmlReadArchiveBase::IsTagSkippingSupported() const
{
	return true;
}


bool CXmlReadArchiveBase::BeginTag(const CArchiveTag& tag)
{
	bool retVal = true;

	QByteArray tagText;

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(">",  tagText);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Begin of tag cannot be found, shoud be '%1'").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText.isEmpty()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Found tag is empty, shoud be '%1'").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText.endsWith('/')){	// if the last char is '/'
		tagText = tagText.left(tagText.size() - 1);

		m_isTagEmpty = true;
	}
	else{
		m_isTagEmpty = false;
	}

	if (tagText != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Bad tag begin, is '%1', should be '%2'").arg(QString(tagText)).arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlReadArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int& count)
{
	bool retVal = true;

	QByteArray tagText;
	char foundDelimeter = ' ';

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(" \t>", tagText, true, &foundDelimeter);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Found tag is empty, shoud be '%1'").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText.isEmpty()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Found tag is empty, shoud be '%1'").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Bad tag begin, is '%1', should be '%2'").arg(QString(tagText)).arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (foundDelimeter == '>'){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("No count attribute in multitag '%1").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	retVal = retVal && ReadToDelimeter("\"", tagText);
	retVal = retVal && ReadToDelimeter("\"", tagText);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("The quotes for tag '%1' could not be found").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	count = tagText.toInt(&retVal);
	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Attribute 'count' of tag %1 is not a number").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}


	retVal = retVal && ReadToDelimeter(">", tagText);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("End of tag '%1' could not be found").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText.endsWith('/')){	// if the last char is '/'
		tagText = tagText.left(tagText.size() - 1);

		m_isTagEmpty = true;
	}
	else{
		m_isTagEmpty = false;
	}

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlReadArchiveBase::EndTag(const CArchiveTag& tag)
{
	if (m_isTagEmpty){
		m_isTagEmpty = false;

		return true;
	}

	bool isSkippedFlag = false;

	bool retVal = InternEndTag(tag, isSkippedFlag);

	if (retVal && isSkippedFlag){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_INFO,
						MI_TAG_SKIPPED,
						QString("Some elements in '%1' was skipped").arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CXmlReadArchiveBase::Process(QByteArray& value)
{
	QByteArray xmlText;
	if (ReadTextNode(xmlText)){
		DecodeXml(xmlText, value);

		return true;
	}

	return false;
}


bool CXmlReadArchiveBase::Process(QString& value)
{
	QByteArray xmlText;
	if (ReadTextNode(xmlText)){
		DecodeXml(xmlText, value);

		return true;
	}

	return false;
}


// protected methods

CXmlReadArchiveBase::CXmlReadArchiveBase(const CArchiveTag& rootTag)
:	m_rootTag(rootTag),
	m_isSeparatorNeeded(false),
	m_isTagEmpty(false)
{
}


bool CXmlReadArchiveBase::InternEndTag(const CArchiveTag& tag, bool& wasTagSkipped)
{
	bool retVal = true;

	QByteArray tagText;

	int nestedTagsCount = 0;
	for (;;){
		retVal = retVal && ReadToDelimeter("<", tagText);
		retVal = retVal && ReadToDelimeter(">", tagText);

		if (!retVal || tagText.isEmpty()){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("End of tag cannot be found, shoud be '%1").arg(QString(tag.GetId())),
							"XmlReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}

		if (tagText[0] == '/'){
			if (nestedTagsCount-- <= 0){
				break;
			}
		}
		else if (tag.IsTagSkippingUsed()){
			++nestedTagsCount;
			wasTagSkipped = true;
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("Tag end expected but found '%'").arg(QString(tagText)),
							"XmlReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}
	}

	if (tagText.mid(1) != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Bad tag end, is '%1', should be '%2'").arg(QString(tagText.mid(1))).arg(QString(tag.GetId())),
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CXmlReadArchiveBase::ReadXmlHeader()
{
	bool retVal = true;

	QByteArray tagText;

	do{
		retVal = retVal && ReadToDelimeter("<", tagText);
		retVal = retVal && ReadToDelimeter(">", tagText);
	} while (retVal && (tagText != m_rootTag.GetId()));

	if (!retVal && IsLogConsumed()){
		SendLogMessage(
					istd::IInformationProvider::IC_ERROR,
					MI_TAG_ERROR,
					QObject::tr("Cannot find root tag '%1'").arg(QString(m_rootTag.GetId())),
					"XmlReader",
					istd::IInformationProvider::ITF_SYSTEM);
	}

	return retVal;
}


bool CXmlReadArchiveBase::ReadXmlFooter()
{
	return EndTag(m_rootTag);
}


// reimplemented (iser::CTextReadArchiveBase)

bool CXmlReadArchiveBase::ReadTextNode(QByteArray& text)
{
	if (m_isTagEmpty){
		if (m_isSeparatorNeeded){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_SKIPPED,
							"Could not read second string from empty tag",
							"XmlReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}

		text = "";

		m_isSeparatorNeeded = true;

		return true;
	}

	QByteArray xmlText;

	if (m_isSeparatorNeeded){
		if (!ReadToDelimeter("<", xmlText) || !ReadToDelimeter("/>", xmlText, false)){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_SKIPPED,
							"Cannot find separator between multiple elements in the same tag",
							"XmlReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}

		if (xmlText != GetElementSeparator().toLocal8Bit()){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							"Bad separator tag",
							"XmlReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}

			return false;
		}

		ReadToDelimeter(">", xmlText);
	}
	else{
		m_isSeparatorNeeded = true;
	}

	if (ReadToDelimeter("<", text, false)){
		return true;
	}
	else{
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						"Cannot find begin of enclosing tag",
						"XmlReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}

		return false;
	}
}


} // namespace iser


