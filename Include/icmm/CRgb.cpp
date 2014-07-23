#include "icmm/CRgb.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CRgb::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag cyanTag("Red", "Red component", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag magentaTag("Green", "Green component", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag yellowTag("Blue", "Blue component", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(cyanTag);
	retVal = retVal && archive.Process(GetElementRef(CI_RED));
	retVal = retVal && archive.EndTag(cyanTag);

	retVal = retVal && archive.BeginTag(magentaTag);
	retVal = retVal && archive.Process(GetElementRef(CI_GREEN));
	retVal = retVal && archive.EndTag(magentaTag);

	retVal = retVal && archive.BeginTag(yellowTag);
	retVal = retVal && archive.Process(GetElementRef(CI_BLUE));
	retVal = retVal && archive.EndTag(yellowTag);

	return retVal;
}


} // namespace icmm


