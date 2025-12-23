#include <istd/CBitManip.h>


namespace istd
{


CBitManip::CBitManip()
{
	// Build table: index of least significant set bit in byte, -1 for 0
	for (int v = 0; v < 256; ++v){
		int idx = -1;
		if (v != 0){
			for (int i = 0; i < 8; ++i){
				if (v & (1 << i)){
					idx = i;
					break;
				}
			}
		}
		m_firstBitInByte[v] = static_cast<qint8>(idx);
	}
}


CBitManip CBitManip::instance;


} // namespace istd


