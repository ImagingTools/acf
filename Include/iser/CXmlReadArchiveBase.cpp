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

	if (retVal && (tagText != tag.GetId())){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::ILogger::MC_ERROR,
						MI_TAG_ERROR,
						"Bad tag begin, is '" + tagText + "', should be '" + tag.GetId() + "'",
						"iser::CXmlReadArchiveBase",
						MF_SYSTEM);
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
		return false;
	}

	if (tagText != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::ILogger::MC_ERROR,
						MI_TAG_ERROR,
						"Bad tag begin, is '" + tagText + "', should be '" + tag.GetId() + "'",
						"iser::CXmlReadArchiveBase",
						MF_SYSTEM);
		}

		return false;
	}

	if (foundDelimeter == '>'){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::ILogger::MC_ERROR,
						MI_TAG_ERROR,
						"No count attribute in multitag '" + tag.GetId() + "'",
						"iser::CXmlReadArchiveBase",
						MF_SYSTEM);
		}

		return false;
	}

	retVal = retVal && ReadToDelimeter("\"", tagText);
	retVal = retVal && ReadToDelimeter("\"", tagText);

	std::istringstream stream(tagText);

	stream >> count;

	m_isSeparatorNeeded = false;

	return retVal;
}


bool CXmlReadArchiveBase::EndTag(const CArchiveTag& tag)
{
	bool isSkippedFlag = false;

	bool retVal = InternEndTag(tag, isSkippedFlag);

	if (retVal && isSkippedFlag && IsLogConsumed()){
		SendLogMessage(
					istd::ILogger::MC_INFO,
					MI_TAG_SKIPPED,
					"Some elements in '" + tag.GetId() + "' was skipped",
					"iser::CXmlReadArchiveBase",
					MF_SYSTEM);
	}

	return retVal;
}


bool CXmlReadArchiveBase::Process(std::string& value)
{
	std::string xmlText;

	if (m_isSeparatorNeeded){
		if (!ReadToDelimeter(">", xmlText)){
			return false;
		}

		if (xmlText != GetElementSeparator().ToString()){
			SendLogMessage(
						istd::ILogger::MC_INFO,
						MI_TAG_ERROR,
						"Bad separator tag, should be ",
						"iser::CXmlReadArchiveBase",
						MF_SYSTEM);
		}
	}
	else{
		m_isSeparatorNeeded = true;
	}

	if (ReadToDelimeter("<", xmlText, false)){
		DecodeXml(xmlText, value);

		return true;
	}
	else{
		return false;
	}
}


bool CXmlReadArchiveBase::Process(istd::CString& value)
{
	std::string textValue;
	if (Process(textValue)){
		value = textValue;

		return true;
	}

	return false;
}


// protected methods

CXmlReadArchiveBase::CXmlReadArchiveBase(const CArchiveTag& rootTag)
:	m_rootTag(rootTag),
	m_isSeparatorNeeded(false)
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
							istd::ILogger::MC_ERROR,
							MI_TAG_ERROR,
							"Tag end expected but found '" + tagText + "'",
							"iser::CXmlReadArchiveBase",
							MF_SYSTEM);
			}

			return false;
		}
	}

	if (tagText.substr(1) != tag.GetId()){
		if (IsLogConsumed()){
			SendLogMessage(
						istd::ILogger::MC_ERROR,
						MI_TAG_ERROR,
						"Bad tag end, is '" + tagText.substr(1) + "', should be '" + tag.GetId() + "'",
						"iser::CXmlReadArchiveBase",
						MF_SYSTEM);
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

	return retVal;
}


} // namespace iser


