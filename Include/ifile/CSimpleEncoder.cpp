#include <ifile/CSimpleEncoder.h>


namespace ifile
{


// public static methods

bool CSimpleEncoder::Encode(const quint8* dataPtr, quint8* resultPtr, int size)
{
	quint8 carry = EV_INIT_OFFSET;
	for (int i = 0; i < size; i++){
		carry += dataPtr[i] + EV_OFFSET;
		resultPtr[i] = carry;
	}

	return true;
}


bool CSimpleEncoder::Decode(const quint8* dataPtr, quint8* resultPtr, int size)
{
	quint8 carry = EV_INIT_OFFSET;
	for (int i = 0; i < size; i++){
		carry += EV_OFFSET;

		quint8 value = dataPtr[i] - carry;
		resultPtr[i] = value;

		carry += value;
	}

	return true;
}


} // namespace ifile


