#ifndef istd_CBitManip_included
#define istd_CBitManip_included


#include "istd/istd.h"


namespace istd
{


class CBitManip
{
public:
	CBitManip();

	/**
		Return index of first bit in byte.
	*/
	int GetFirstBitIndex(I_DWORD bits) const;

	static CBitManip instance;

private:
	I_SBYTE m_firstBitInByte[256];
};


// inline methods

inline int CBitManip::GetFirstBitIndex(I_DWORD bits) const
{
	I_WORD* words = (I_WORD*)&bits;
	I_BYTE* bytes = (I_BYTE*)&bits;
	if (words[0] != 0){
		if (bytes[0] != 0){
			return int(m_firstBitInByte[bytes[0]]);
		}

		return int(m_firstBitInByte[bytes[1]]);
	}
	else if (words[1] != 0){
		if (bytes[2] != 0){
			return int(m_firstBitInByte[bytes[2]]);
		}

		return int(m_firstBitInByte[bytes[3]]);
	}

	return -1;
}


} // namespace istd


#endif // !istd_CBitManip_included


