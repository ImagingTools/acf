#ifndef istd_CBase64_included
#define istd_CBase64_included


// STL includes
#include <vector>
#include <string>


// ACF includes
#include <istd/istd.h>


namespace istd
{


/**
	Base64 converter.
*/
class CBase64
{
public:
	static std::string ConvertToBase64(const void* dataPtrPtr, int size);
	static std::vector<I_BYTE> ConvertFromBase64(const std::string& base64String);

private:
	static char SixBitToChar(I_BYTE b);
	static I_BYTE CharToSixBit(char c);
};


} // namespace istd


#endif // !istd_CBase64_included


