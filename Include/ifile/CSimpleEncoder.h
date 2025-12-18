#pragma once


// ACF includes
#include <ifile/ifile.h>


namespace ifile
{


class CSimpleEncoder
{
public:
	enum EncodeValues
	{
		EV_INIT_OFFSET = 23,
		EV_OFFSET = 97
	};

	static bool Encode(const quint8* dataPtr, quint8* resultPtr, int size);
	static bool Decode(const quint8* dataPtr, quint8* resultPtr, int size);
};


} // namespace ifile


