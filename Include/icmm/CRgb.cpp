#include "icmm/CRgb.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CRgb::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag cyanTag("Red", "Red component");
	retVal = retVal && archive.BeginTag(cyanTag);
	retVal = retVal && archive.Process(GetElementRef(Red));
	retVal = retVal && archive.EndTag(cyanTag);

	static iser::CArchiveTag magentaTag("Green", "Green component");
	retVal = retVal && archive.BeginTag(magentaTag);
	retVal = retVal && archive.Process(GetElementRef(Green));
	retVal = retVal && archive.EndTag(magentaTag);

	static iser::CArchiveTag yellowTag("Blue", "Blue component");
	retVal = retVal && archive.BeginTag(yellowTag);
	retVal = retVal && archive.Process(GetElementRef(Blue));
	retVal = retVal && archive.EndTag(yellowTag);

	return retVal;
}


} // namespace icmm


