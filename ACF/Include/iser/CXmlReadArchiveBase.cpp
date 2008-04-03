#include "iser/CXmlReadArchiveBase.h"


#include <strstream>


namespace iser
{


// reimplemented (iser::IArchive)

bool CXmlReadArchiveBase::BeginTag(const CArchiveTag& tag)
{
	bool retVal = true;

	std::string tagText;

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(">",  tagText);

	retVal = retVal && (tagText == tag.GetId());

	return retVal;
}


bool CXmlReadArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int& count)
{
	bool retVal = true;

	std::string tagText;

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(" ", tagText);

	retVal = retVal && (tagText == tag.GetId());

	retVal = retVal && ReadToDelimeter("\"", tagText);
	retVal = retVal && ReadToDelimeter("\"", tagText);

	std::istrstream stream(tagText.c_str());

	stream >> count;

	return retVal;
}


bool CXmlReadArchiveBase::EndTag(const CArchiveTag& tag)
{
	bool retVal = true;

	std::string tagText;

	retVal = retVal && ReadToDelimeter("<", tagText);
	retVal = retVal && ReadToDelimeter(">", tagText);

	retVal = retVal && !tagText.empty() && (tagText[0] == '/');
	retVal = retVal && (tagText.substr(1) == tag.GetId());

	return retVal;
}


bool CXmlReadArchiveBase::Process(std::string& value)
{
	std::string xmlText;
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
	if (ProcessInternal(textValue)){
		value = textValue;

		return true;
	}

	return false;
}


// protected methods

CXmlReadArchiveBase::CXmlReadArchiveBase(const iser::CArchiveTag& rootTag)
:	m_rootTag(rootTag)
{
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


