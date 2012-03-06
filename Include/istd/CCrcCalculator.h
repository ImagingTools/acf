#ifndef istd_CCrcCalculator_included
#define istd_CCrcCalculator_included


// STL includes
#include <vector>

// ACF includes
#include "istd/istd.h"


class QString;


namespace istd
{

class CCrcCalculator
{
public:
	typedef std::vector<I_BYTE> ByteStream;

	static I_DWORD GetCrcFromData(const I_BYTE* dataPtr, int dataSize);
	static I_DWORD GetCrcFromStream(const ByteStream& byteStream);
	static I_DWORD GetCrcFromFile(const QString& fileName);

	static void UpdateCrc(const I_BYTE& byte, I_DWORD& dwCrc32);

private:
	static I_DWORD s_crcTable[256];
};


// inline methods

inline void CCrcCalculator::UpdateCrc(const I_BYTE& byte, I_DWORD& crcValue)
{
	crcValue = (crcValue >> 8) ^ s_crcTable[byte ^ (crcValue & 0x000000FF)];
}


} // namespace istd


#endif // !istd_CCrcCalculator_included

