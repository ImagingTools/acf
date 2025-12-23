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
	if (bits == 0) {
		return -1;
	}

	quint8 b0 = static_cast<quint8>(bits & 0xFFu);
	if (b0 != 0){
		return int(m_firstBitInByte[b0]);
	}

	quint8 b1 = static_cast<quint8>((bits >> 8) & 0xFFu);
	if (b1 != 0) {
		return int(m_firstBitInByte[b1]) + 8;
	}

	quint8 b2 = static_cast<quint8>((bits >> 16) & 0xFFu);
	if (b2 != 0){
		return int(m_firstBitInByte[b2]) + 16;
	}

	quint8 b3 = static_cast<quint8>((bits >> 24) & 0xFFu);
	if (b3 != 0) {
		return int(m_firstBitInByte[b3]) + 24;
	}

	return -1;
}


} // namespace istd


#endif // !istd_CBitManip_included


