#ifndef istd_CBitManip_included
#define istd_CBitManip_included


// Qt includes
#include <QtCore/QtGlobal>


namespace istd
{


/**
	Helper class for bits manipulations.
*/
class CBitManip
{
public:
	CBitManip();

	/**
		Return index of first bit in byte.
	*/
	int GetFirstBitIndex(quint32 bits) const;

	static CBitManip instance;

private:
	qint8 m_firstBitInByte[256];
};


// inline methods

inline int CBitManip::GetFirstBitIndex(quint32 bits) const
{
	quint16* words = (quint16*)&bits;
	quint8* bytes = (quint8*)&bits;
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


