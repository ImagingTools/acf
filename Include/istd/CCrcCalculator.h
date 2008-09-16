#ifndef istd_CCrcCalculator_included
#define istd_CCrcCalculator_included


#include <vector>

#include "istd/CString.h"


namespace istd
{


class CCrcCalculator
{
public:
	typedef std::vector<I_BYTE> ByteStream;

	static I_DWORD GetCrcFromStream(const ByteStream& byteStream);
	static I_DWORD GetCrcFromFile(const istd::CString& fileName);

protected:
	static void CalcCrc32(const I_BYTE& byte, I_DWORD &dwCrc32);

private:
	static I_DWORD s_crcTable[256];
};


} // namespace istd


#endif // !istd_CCrcCalculator_included

