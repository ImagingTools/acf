#include <istd/CBitManip.h>


namespace istd
{


CBitManip::CBitManip()
{
	qint8 firstBitIndex = -1;
	for (int i = 0; i < 256; ++i){
		if (i >= int(2 << firstBitIndex)){
			++firstBitIndex;
		}

		m_firstBitInByte[i] = firstBitIndex;
	}
}


CBitManip CBitManip::instance;


} // namespace istd


