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
			xmlText += iter.value();
		}
		else if ((c >= ' ') && (c <= '}')){
			xmlText += c;
		}
		else{
			xmlText +=  QString("&#%1;").arg(quint16((unsigned char)c)).toStdString();
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
				QString number(xmlText.substr(ampPos + 2, semicolonPos - ampPos - 2).c_str());

				text += char(number.toInt());
			}
			else{
				EntityToChartMap::const_iterator entityIter = s_entityToChartMap.find(xmlText.substr(ampPos, semicolonPos - ampPos + 1));
				if (entityIter != s_entityToChartMap.end()){
					text += entityIter.value();
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


void CXmlDocumentInfoBase::EncodeXml(const QString& text, std::string& xmlText)
{
	xmlText = "";

	std::wstring::size_type textLength = int(text.size());
	for (std::wstring::size_type i = 0; i < textLength; ++i){
		QChar c = text.at(i);
		WideCharToEntityMap::const_iterator iter = s_wideCharToEntityMap.find(c);
		if (iter != s_wideCharToEntityMap.end()){
			xmlText += iter.value();
		}
		else if ((c >= ' ') && (c <= '}')){
			xmlText += c.toAscii();
		}
		else{
			quint16 unicodeChar = quint16(c.unicode());

			xmlText +=  QString("&#%1;").arg(unicodeChar).toStdString();
		}
	}
}


void CXmlDocumentInfoBase::DecodeXml(const std::string& xmlText, QString& text)
{
	text.clear();

	int actPos = 0;

	for (;;){
		std::string::size_type ampPos = xmlText.find('&', actPos);
		if (ampPos != std::string::npos){
			QString subString = xmlText.substr(actPos, ampPos - actPos).c_str();

			text += subString;

			std::string::size_type semicolonPos = xmlText.find(';', actPos);
			if ((semicolonPos == std::string::npos) || (ampPos >= semicolonPos - 2)){
				return;
			}

			if (xmlText[ampPos + 1] == '#'){
				QString numberString = xmlText.substr(ampPos + 2, semicolonPos - ampPos - 2).c_str();

				text += wchar_t(numberString.toInt());
			}
			else{
				EntityToWideChartMap::const_iterator entityIter = s_entityToWideChartMap.find(xmlText.substr(ampPos, semicolonPos - ampPos + 1));
				if (entityIter != s_entityToWideChartMap.end()){
					text += entityIter.value();
				}
			}

			actPos = semicolonPos + 1;
		}
		else{
			QString subString = xmlText.substr(actPos).c_str();

			text += subString;

			return;
		}
	}
}


const QString& CXmlDocumentInfoBase::GetElementSeparator()
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

QString CXmlDocumentInfoBase::s_elementSeparator("br");


} // namespace iser


