#include "iser/CXmlDocumentInfoBase.h"


// ACF includes
#include "istd/istd.h"


namespace iser
{


void CXmlDocumentInfoBase::EncodeXml(const QByteArray& text, QByteArray& xmlText)
{
	xmlText = "";

	int textLength = int(text.size());
	for (int i = 0; i < textLength; ++i){
		char c = text[i];
		CharToEntityMap::ConstIterator iter = s_charToEntityMap.constFind(c);
		if (iter != s_charToEntityMap.constEnd()){
			xmlText += iter.value();
		}
		else if ((c >= ' ') && (c <= '}')){
			xmlText += c;
		}
		else{
			xmlText +=  QString("&#%1;").arg(quint16((unsigned char)c)).toLocal8Bit();
		}
	}
}


void CXmlDocumentInfoBase::DecodeXml(const QByteArray& xmlText, QByteArray& text)
{
	text = "";

	int actPos = 0;

	for (;;){
		int ampPos = xmlText.indexOf('&', actPos);
		if (ampPos >= 0){
			text += xmlText.mid(actPos, ampPos - actPos);

			int semicolonPos = xmlText.indexOf(';', actPos);
			if ((semicolonPos < 0) || (ampPos >= semicolonPos - 2)){
				return;
			}

			if (xmlText[ampPos + 1] == '#'){
				QString number(xmlText.mid(ampPos + 2, semicolonPos - ampPos - 2));

				text += char(number.toInt());
			}
			else{
				QByteArray entityId = xmlText.mid(ampPos, semicolonPos - ampPos + 1);
				EntityToChartMap::ConstIterator entityIter = s_entityToChartMap.constFind(entityId);
				if (entityIter != s_entityToChartMap.constEnd()){
					text += entityIter.value();
				}
			}

			actPos = semicolonPos + 1;
		}
		else{
			text += xmlText.mid(actPos);

			return;
		}
	}
}


void CXmlDocumentInfoBase::EncodeXml(const QString& text, QByteArray& xmlText)
{
	xmlText = "";

	std::wstring::size_type textLength = int(text.size());
	for (std::wstring::size_type i = 0; i < textLength; ++i){
		QChar c = text.at(i);
		WideCharToEntityMap::ConstIterator iter = s_wideCharToEntityMap.constFind(c);
		if (iter != s_wideCharToEntityMap.constEnd()){
			xmlText += iter.value();
		}
		else if ((c >= ' ') && (c <= '}')){
			xmlText += c.toLatin1();
		}
		else{
			quint16 unicodeChar = quint16(c.unicode());

			xmlText +=  QString("&#%1;").arg(unicodeChar).toLocal8Bit();
		}
	}
}


void CXmlDocumentInfoBase::DecodeXml(const QByteArray& xmlText, QString& text)
{
	text.clear();

	int actPos = 0;

	for (;;){
		int ampPos = xmlText.indexOf('&', actPos);
		if (ampPos >= 0){
			QString subString = xmlText.mid(actPos, ampPos - actPos);

			text += subString;

			int semicolonPos = xmlText.indexOf(';', actPos);
			if ((semicolonPos < 0) || (ampPos >= semicolonPos - 2)){
				return;
			}

			if (xmlText[ampPos + 1] == '#'){
				QByteArray numberString = xmlText.mid(ampPos + 2, semicolonPos - ampPos - 2);

				text += QChar(numberString.toInt());
			}
			else{
				QByteArray entityId = xmlText.mid(ampPos, semicolonPos - ampPos + 1);
				EntityToWideChartMap::ConstIterator entityIter = s_entityToWideChartMap.constFind(entityId);
				if (entityIter != s_entityToWideChartMap.constEnd()){
					text += entityIter.value();
				}
			}

			actPos = semicolonPos + 1;
		}
		else{
			QString subString = xmlText.mid(actPos);

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


