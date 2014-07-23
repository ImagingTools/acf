#include "icmm/CCmyk.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CCmyk::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag cyanTag("Cyan", "Cyan component", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag magentaTag("Magenta", "Magenta component", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag yellowTag("Yellow", "Yellow component", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag blackTag("Black", "Black component", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(cyanTag);
	retVal = retVal && archive.Process(GetElementRef(CI_CYAN));
	retVal = retVal && archive.EndTag(cyanTag);

	retVal = retVal && archive.BeginTag(magentaTag);
	retVal = retVal && archive.Process(GetElementRef(CI_MAGENTA));
	retVal = retVal && archive.EndTag(magentaTag);

	retVal = retVal && archive.BeginTag(yellowTag);
	retVal = retVal && archive.Process(GetElementRef(CI_YELLOW));
	retVal = retVal && archive.EndTag(yellowTag);

	retVal = retVal && archive.BeginTag(blackTag);
	retVal = retVal && archive.Process(GetElementRef(CI_BLACK));
	retVal = retVal && archive.EndTag(blackTag);

	return retVal;
}


} // namespace icmm


