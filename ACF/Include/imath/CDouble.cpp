#include "imath/CDouble.h"


namespace imath
{


bool CDouble::Serialize(iser::IArchive& archive)
{
	return archive.Process(m_value);
}


} // namespace imath


