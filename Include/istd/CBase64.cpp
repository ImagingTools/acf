#include "istd/CBase64.h"


// ACF includes
#include "istd/TDelPtr.h"


namespace istd
{


// public static methods

std::string CBase64::ConvertToBase64(const void* dataPtr, int size)
{
	if (size == 0){
		return std::string();
	}

	int padding = size % 3;
	if (padding > 0){
		padding = 3 - padding;
	}

	int blocks = (size - 1) / 3 + 1;

	std::string outputString;
	
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
		I_BYTE b1 = ((I_BYTE*)dataPtr)[index];
		I_BYTE b2 = pad2 ? (I_BYTE)0 : ((I_BYTE*)dataPtr)[index + 1];
		I_BYTE b3 = pad1 ? (I_BYTE)0 : ((I_BYTE*)dataPtr)[index + 2];

		I_BYTE temp1 = (I_BYTE)((b1 & 0xFC) >> 2);

		I_BYTE temp = (I_BYTE)((b1 & 0x03) << 4);
		I_BYTE temp2 = (I_BYTE)((b2 & 0xF0) >> 4);
		temp2 = I_BYTE(temp2 + temp);

		temp = (I_BYTE)((b2 & 0x0F) << 2);
		I_BYTE temp3 = (I_BYTE)((b3 & 0xC0) >> 6);
		temp3 = I_BYTE(temp3 + temp);

		I_BYTE temp4 = (I_BYTE)(b3 & 0x3F);

		index = i * 4;
		outputString[index] = SixBitToChar(temp1);
		outputString[index+1] = SixBitToChar(temp2);
		outputString[index+2] = pad2 ? '=' : SixBitToChar(temp3);
		outputString[index+3] = pad1 ? '=' : SixBitToChar(temp4);
	}

	return outputString;
}


std::vector<I_BYTE> CBase64::ConvertFromBase64(const std::string& base64String)
{
	int size = base64String.size();
	if (size == 0){
		return std::vector<I_BYTE>();
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

	std::vector<I_BYTE> outputData;
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
		I_BYTE temp1 = CharToSixBit(base64String[index]);
		I_BYTE temp2 = CharToSixBit(base64String[index + 1]);
		I_BYTE temp3 = CharToSixBit(base64String[index + 2]);
		I_BYTE temp4 = CharToSixBit(base64String[index + 3]);

		I_BYTE b = (I_BYTE)(temp1 << 2);
		I_BYTE b1 = (I_BYTE)((temp2 & 0x30) >> 4);
		b1 = I_BYTE(b1 + b);

		b = (I_BYTE)((temp2 & 0x0F) << 4);
		I_BYTE b2 = (I_BYTE)((temp3 & 0x3C) >> 2);
		b2 = I_BYTE(b2 + b);

		b = (I_BYTE)((temp3 & 0x03) << 6);
		I_BYTE b3 = temp4;
		b3 = I_BYTE(b3 + b);

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

char CBase64::SixBitToChar(I_BYTE b)
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


I_BYTE CBase64::CharToSixBit(char c)
{
	I_BYTE b;
	
	if (c >= 'A' && c <= 'Z'){
		b = (I_BYTE)((int)c - (int)'A');
	}
	else if (c >= 'a' && c <= 'z'){
		b = (I_BYTE)((int)c - (int)'a' + 26);
	}
	else if (c >= '0' && c <= '9'){
		b = (I_BYTE)((int)c - (int)'0' + 52);
	}
	else if (c == '+'){
		b = (I_BYTE)62;
	}
	else{
		b = (I_BYTE)63;
	}

	return b;
}


} // namespace istd


