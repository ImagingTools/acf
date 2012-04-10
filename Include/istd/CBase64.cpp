#include "istd/CBase64.h"


// ACF includes
#include "istd/TDelPtr.h"


namespace istd
{


// public static methods

QByteArray CBase64::ConvertToBase64(const void* dataPtr, int size)
{
	if (size == 0){
		return QByteArray();
	}

	int padding = size % 3;
	if (padding > 0){
		padding = 3 - padding;
	}

	int blocks = (size - 1) / 3 + 1;

	QByteArray outputString;
	
	outputString.resize(blocks * 4);

	for (int i = 0; i < blocks; i++){
		bool finalBlock = (i == blocks - 1);
		bool pad2 = false;
		bool pad1 = false;
		if (finalBlock){
			pad2 = padding == 2;
			pad1 = padding > 0;
		}

		int index = i * 3;
		quint8 b1 = ((quint8*)dataPtr)[index];
		quint8 b2 = pad2 ? (quint8)0 : ((quint8*)dataPtr)[index + 1];
		quint8 b3 = pad1 ? (quint8)0 : ((quint8*)dataPtr)[index + 2];

		quint8 temp1 = (quint8)((b1 & 0xFC) >> 2);

		quint8 temp = (quint8)((b1 & 0x03) << 4);
		quint8 temp2 = (quint8)((b2 & 0xF0) >> 4);
		temp2 = quint8(temp2 + temp);

		temp = (quint8)((b2 & 0x0F) << 2);
		quint8 temp3 = (quint8)((b3 & 0xC0) >> 6);
		temp3 = quint8(temp3 + temp);

		quint8 temp4 = (quint8)(b3 & 0x3F);

		index = i * 4;
		outputString[index] = SixBitToChar(temp1);
		outputString[index+1] = SixBitToChar(temp2);
		outputString[index+2] = pad2 ? '=' : SixBitToChar(temp3);
		outputString[index+3] = pad1 ? '=' : SixBitToChar(temp4);
	}

	return outputString;
}


QVector<quint8> CBase64::ConvertFromBase64(const QByteArray& base64String)
{
	int size = base64String.size();
	if (size == 0){
		return QVector<quint8>();
	}

	int padding = 0;
	if (size > 2 && base64String[size - 2] == '='){
		padding = 2;
	}
	else if (size > 1 && base64String[size - 1] == '='){
		padding = 1;
	}

	int blocks = (size - 1) / 4 + 1;
	int bytes = blocks * 3;

	QVector<quint8> outputData;
	outputData.resize(bytes - padding);

	for (int i = 0; i < blocks; i++){
		bool finalBlock = i == blocks - 1;
		bool pad2 = false;
		bool pad1 = false;
		if (finalBlock){
			pad2 = padding == 2;
			pad1 = padding > 0;
		}

		int index = i * 4;
		quint8 temp1 = CharToSixBit(base64String[index]);
		quint8 temp2 = CharToSixBit(base64String[index + 1]);
		quint8 temp3 = CharToSixBit(base64String[index + 2]);
		quint8 temp4 = CharToSixBit(base64String[index + 3]);

		quint8 b = (quint8)(temp1 << 2);
		quint8 b1 = (quint8)((temp2 & 0x30) >> 4);
		b1 = quint8(b1 + b);

		b = (quint8)((temp2 & 0x0F) << 4);
		quint8 b2 = (quint8)((temp3 & 0x3C) >> 2);
		b2 = quint8(b2 + b);

		b = (quint8)((temp3 & 0x03) << 6);
		quint8 b3 = temp4;
		b3 = quint8(b3 + b);

		index = i * 3;
		outputData[index] = b1;
		if (!pad2){
			outputData[index + 1] = b2;
		}

		if (!pad1){
			outputData[index + 2] = b3;
		}
	}

	return outputData;
}


// private static methods

char CBase64::SixBitToChar(quint8 b)
{
	char c;
	if (b < 26){
		c = (char)((int)b + (int)'A');
	}
	else if (b < 52){
		c = (char)((int)b - 26 + (int)'a');
	}
	else if (b < 62){
		c = (char)((int)b - 52 + (int)'0');
	}
	else if (b == 62){
		c = '+';
	}
	else{
		c = '/';
	}

	return c;
}


quint8 CBase64::CharToSixBit(char c)
{
	quint8 b;
	
	if (c >= 'A' && c <= 'Z'){
		b = (quint8)((int)c - (int)'A');
	}
	else if (c >= 'a' && c <= 'z'){
		b = (quint8)((int)c - (int)'a' + 26);
	}
	else if (c >= '0' && c <= '9'){
		b = (quint8)((int)c - (int)'0' + 52);
	}
	else if (c == '+'){
		b = (quint8)62;
	}
	else{
		b = (quint8)63;
	}

	return b;
}


} // namespace istd


