#include "icmm/CRgb.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


bool CRgb::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag cyanTag("Red", "Red component");
	retVal = retVal && archive.BeginTag(cyanTag);
	retVal = retVal && archive.Process(GetComponentValueRef(Red));
	retVal = retVal && archive.EndTag(cyanTag);

	static iser::CArchiveTag magentaTag("Green", "Green component");
	retVal = retVal && archive.BeginTag(magentaTag);
	retVal = retVal && archive.Process(GetComponentValueRef(Green));
	retVal = retVal && archive.EndTag(magentaTag);

	static iser::CArchiveTag yellowTag("Blue", "Blue component");
	retVal = retVal && archive.BeginTag(yellowTag);
	retVal = retVal && archive.Process(GetComponentValueRef(Blue));
	retVal = retVal && archive.EndTag(yellowTag);

	return retVal;
}


} // namespace icmm


