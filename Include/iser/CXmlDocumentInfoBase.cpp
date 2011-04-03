#include "iser/CXmlDocumentInfoBase.h"


// ACF includes
#include "istd/istd.h"


namespace iser
{


void CXmlDocumentInfoBase::EncodeXml(const std::string& text, std::string& xmlText)
{
	xmlText = "";

	std::string::size_type textLength = int(text.size());
	for (std::string::size_type i = 0; i < textLength; ++i){
		char c = text[i];
		CharToEntityMap::const_iterator iter = s_charToEntityMap.find(c);
		if (iter != s_charToEntityMap.end()){
			xmlText += iter->second;
		}
		else if ((c >= ' ') && (c <= '}')){
			xmlText += c;
		}
		else{
			xmlText += "&#" + istd::CString::FromNumber(I_WORD((unsigned char)c)).ToString() + ";";
		}
	}
}


void CXmlDocumentInfoBase::DecodeXml(const std::string& xmlText, std::string& text)
{
	text = "";

	std::string::size_type actPos = 0;

	for (;;){
		std::string::size_type ampPos = xmlText.find('&', actPos);
		if (ampPos != std::string::npos){
			text += xmlText.substr(actPos, ampPos - actPos);

			std::string::size_type semicolonPos = xmlText.find(';', actPos);
			if ((semicolonPos == std::string::npos) || (ampPos >= semicolonPos - 2)){
				return;
			}

			if (xmlText[ampPos + 1] == '#'){
				text += char(istd::CString(xmlText.substr(ampPos + 2, semicolonPos - ampPos - 2)).ToNumber<int>());
			}
			else{
				EntityToChartMap::const_iterator entityIter = s_entityToChartMap.find(xmlText.substr(ampPos, semicolonPos - ampPos + 1));
				if (entityIter != s_entityToChartMap.end()){
					text += entityIter->second;
				}
			}

			actPos = semicolonPos + 1;
		}
		else{
			text += xmlText.substr(actPos);

			return;
		}
	}
}


void CXmlDocumentInfoBase::EncodeXml(const std::wstring& text, std::string& xmlText)
{
	xmlText = "";

	std::wstring::size_type textLength = int(text.size());
	for (std::wstring::size_type i = 0; i < textLength; ++i){
		wchar_t c = text[i];
		WideCharToEntityMap::const_iterator iter = s_wideCharToEntityMap.find(c);
		if (iter != s_wideCharToEntityMap.end()){
			xmlText += iter->second;
		}
		else if ((c >= ' ') && (c <= '}')){
			xmlText += char(c);
		}
		else{
			xmlText += "&#" + istd::CString::FromNumber(I_WORD((unsigned short)c)).ToString() + ";";
		}
	}
}


void CXmlDocumentInfoBase::DecodeXml(const std::string& xmlText, std::wstring& text)
{
	text = L"";

	std::wstring::size_type actPos = 0;

	for (;;){
		std::string::size_type ampPos = xmlText.find('&', actPos);
		if (ampPos != std::string::npos){
			istd::CString subString = xmlText.substr(actPos, ampPos - actPos);

			text += subString;

			std::string::size_type semicolonPos = xmlText.find(';', actPos);
			if ((semicolonPos == std::string::npos) || (ampPos >= semicolonPos - 2)){
				return;
			}

			if (xmlText[ampPos + 1] == '#'){
				text += wchar_t(istd::CString(xmlText.substr(ampPos + 2, semicolonPos - ampPos - 2)).ToNumber<int>());
			}
			else{
				EntityToWideChartMap::const_iterator entityIter = s_entityToWideChartMap.find(xmlText.substr(ampPos, semicolonPos - ampPos + 1));
				if (entityIter != s_entityToWideChartMap.end()){
					text += entityIter->second;
				}
			}

			actPos = semicolonPos + 1;
		}
		else{
			istd::CString subString = xmlText.substr(actPos);

			text += subString;

			return;
		}
	}
}


const istd::CString& CXmlDocumentInfoBase::GetElementSeparator()
{
	return s_elementSeparator;
}


// public methods of embedded class CharToEntityMap

CXmlDocumentInfoBase::CharToEntityMap::CharToEntityMap()
{
	operator[]('<') = "&lt;";
	operator[]('>') = "&gt;";
	operator[]('&') = "&amp;";
}


// public methods of embedded class EntityToChartMap

CXmlDocumentInfoBase::EntityToChartMap::EntityToChartMap()
{
	operator[]("&lt;") = '<';
	operator[]("&gt;") = '>';
	operator[]("&amp;") = '&';
}


// public methods of embedded class WideCharToEntityMap

CXmlDocumentInfoBase::WideCharToEntityMap::WideCharToEntityMap()
{
	operator[](L'<') = "&lt;";
	operator[](L'>') = "&gt;";
	operator[](L'&') = "&amp;";
}


// public methods of embedded class EntityToWideChartMap

CXmlDocumentInfoBase::EntityToWideChartMap::EntityToWideChartMap()
{
	operator[]("&lt;") = L'<';
	operator[]("&gt;") = L'>';
	operator[]("&amp;") = L'&';
}


// static attributes

CXmlDocumentInfoBase::CharToEntityMap CXmlDocumentInfoBase::s_charToEntityMap;
CXmlDocumentInfoBase::EntityToChartMap CXmlDocumentInfoBase::s_entityToChartMap;
CXmlDocumentInfoBase::WideCharToEntityMap CXmlDocumentInfoBase::s_wideCharToEntityMap;
CXmlDocumentInfoBase::EntityToWideChartMap CXmlDocumentInfoBase::s_entityToWideChartMap;

istd::CString CXmlDocumentInfoBase::s_elementSeparator("br");


} // namespace iser


