#include "iser/CXmlReadArchiveBase.h"


// STL includes
#include <sstream>


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

	std::string tagText;

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(">",  tagText);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Begin of tag cannot be found, shoud be '%1'").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText.empty()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Found tag is empty, shoud be '%1'").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	if (*tagText.rbegin() == '/'){	// if the last char is '/'
		tagText = tagText.substr(0, tagText.size() - 1);

		m_isTagEmpty = true;
	}
	else{
		m_isTagEmpty = false;
	}

	if (tagText != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Bad tag begin, is '%1', should be '%2'").arg(tagText.c_str()).arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlReadArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int& count)
{
	bool retVal = true;

	std::string tagText;
	char foundDelimeter = ' ';

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(" \t>", tagText, true, &foundDelimeter);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Found tag is empty, shoud be '%'").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText.empty()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Found tag is empty, shoud be '%'").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	if (tagText != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Bad tag begin, is '%1', should be '%2'").arg(tagText.c_str()).arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	if (foundDelimeter == '>'){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("No count attribute in multitag '%1").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	retVal = retVal && ReadToDelimeter("\"", tagText);
	retVal = retVal && ReadToDelimeter("\"", tagText);

	if (!retVal){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("The quotes for tag '%1' could not be found").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	std::istringstream stream(tagText);

	stream >> count;

	retVal = retVal && ReadToDelimeter(">", tagText);

	if (!tagText.empty() && *tagText.rbegin() == '/'){	// if the last char is '/'
		tagText = tagText.substr(0, tagText.size() - 1);

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
						istd::IInformation::IC_INFO,
						MI_TAG_SKIPPED,
						QString("Some elements in '%1' was skipped").arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CXmlReadArchiveBase::Process(std::string& value)
{
	if (m_isTagEmpty){
		if (m_isSeparatorNeeded){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformation::IC_ERROR,
							MI_TAG_SKIPPED,
							"Could not read second string from empty tag",
							"iser::CXmlReadArchiveBase",
							istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}

		value = "";

		m_isSeparatorNeeded = true;

		return true;
	}

	std::string xmlText;

	if (m_isSeparatorNeeded){
		if (!ReadToDelimeter("<", xmlText) || !ReadToDelimeter("/>", xmlText, false)){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformation::IC_ERROR,
							MI_TAG_SKIPPED,
							"Cannot find separator between multiple elements in the same tag",
							"iser::CXmlReadArchiveBase",
							istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}

		if (xmlText != GetElementSeparator().toStdString()){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformation::IC_ERROR,
							MI_TAG_ERROR,
							"Bad separator tag",
							"iser::CXmlReadArchiveBase",
							istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}

		ReadToDelimeter(">", xmlText);
	}
	else{
		m_isSeparatorNeeded = true;
	}

	if (ReadToDelimeter("<", xmlText, false)){
		DecodeXml(xmlText, value);

		return true;
	}
	else{
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						"Cannot find begin of enclosing tag",
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}
}


bool CXmlReadArchiveBase::Process(QString& value)
{
	if (m_isTagEmpty){
		if (m_isSeparatorNeeded){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformation::IC_ERROR,
							MI_TAG_SKIPPED,
							"Could not read second string from empty tag",
							"iser::CXmlReadArchiveBase",
							istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}

		value.clear();

		m_isSeparatorNeeded = true;

		return true;
	}

	std::string xmlText;

	if (m_isSeparatorNeeded){
		if (!ReadToDelimeter("<", xmlText) || !ReadToDelimeter("/>", xmlText, false)){
			if (IsLogConsumed()){
				SendLogMessage(
					istd::IInformation::IC_ERROR,
					MI_TAG_SKIPPED,
					"Cannot find separator between multiple elements in the same tag",
					"iser::CXmlReadArchiveBase",
					istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}

		if (xmlText != GetElementSeparator().toStdString()){
			if (IsLogConsumed()){
				SendLogMessage(
					istd::IInformation::IC_ERROR,
					MI_TAG_ERROR,
					"Bad separator tag",
					"iser::CXmlReadArchiveBase",
					istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}

		ReadToDelimeter(">", xmlText);
	}
	else{
		m_isSeparatorNeeded = true;
	}

	if (ReadToDelimeter("<", xmlText, false)){
		DecodeXml(xmlText, value);

		return true;
	}
	else{
		if (IsLogConsumed()){
			SendLogMessage(
				istd::IInformation::IC_ERROR,
				MI_TAG_ERROR,
				"Cannot find begin of enclosing tag",
				"iser::CXmlReadArchiveBase",
				istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}
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

	std::string tagText;

	int nestedTagsCount = 0;
	for (;;){
		retVal = retVal && ReadToDelimeter("<", tagText);
		retVal = retVal && ReadToDelimeter(">", tagText);

		if (!retVal || tagText.empty()){
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformation::IC_ERROR,
							MI_TAG_ERROR,
							QString("End of tag cannot be found, shoud be '%1").arg(tag.GetId().c_str()),
							"iser::CXmlReadArchiveBase",
							istd::IInformation::ITF_SYSTEM);
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
							istd::IInformation::IC_ERROR,
							MI_TAG_ERROR,
							QString("Tag end expected but found '%'").arg(tagText.c_str()),
							"iser::CXmlReadArchiveBase",
							istd::IInformation::ITF_SYSTEM);
			}

			return false;
		}
	}

	if (tagText.substr(1) != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformation::IC_ERROR,
						MI_TAG_ERROR,
						QString("Bad tag end, is '%1', should be '%2'").arg(tagText.substr(1).c_str()).arg(tag.GetId().c_str()),
						"iser::CXmlReadArchiveBase",
						istd::IInformation::ITF_SYSTEM);
		}

		return false;
	}

	return true;
}


bool CXmlReadArchiveBase::SerializeXmlHeader()
{
	bool retVal = true;

	std::string tagText;

	do{
		retVal = retVal && ReadToDelimeter("<", tagText);
		retVal = retVal && ReadToDelimeter(">", tagText);
	} while (retVal && (tagText != m_rootTag.GetId()));

	if (!retVal && IsLogConsumed()){
		SendLogMessage(
					istd::IInformation::IC_ERROR,
					MI_TAG_ERROR,
					QString("Cannot find root tag '%1'").arg(m_rootTag.GetId().c_str()),
					"iser::CXmlReadArchiveBase",
					istd::IInformation::ITF_SYSTEM);
	}

	return retVal;
}


} // namespace iser


