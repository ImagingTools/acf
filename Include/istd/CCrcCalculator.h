#ifndef istd_CCrcCalculator_included
#define istd_CCrcCalculator_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/istd.h"


class QString;


namespace istd
{

class CCrcCalculator
{
public:
	typedef QVector<quint8> ByteStream;

	static quint32 GetCrcFromData(const quint8* dataPtr, int dataSize);
	static quint32 GetCrcFromStream(const ByteStream& byteStream);
	static quint32 GetCrcFromFile(const QString& fileName);

	static void UpdateCrc(const quint8& byte, quint32& dwCrc32);

private:
	static quint32 s_crcTable[256];
};


// inline methods

inline void CCrcCalculator::UpdateCrc(const quint8& byte, quint32& crcValue)
{
	crcValue = (crcValue >> 8) ^ s_crcTable[byte ^ (crcValue & 0x000000FF)];
}


} // namespace istd


#endif // !istd_CCrcCalculator_included

