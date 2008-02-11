#include "icmm/CCmyk.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


bool CCmyk::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag cyanTag("Cyan", "Cyan component");
	retVal = retVal && archive.BeginTag(cyanTag);
	retVal = retVal && archive.Process(GetComponentValueRef(CI_CYAN));
	retVal = retVal && archive.EndTag(cyanTag);

	static iser::CArchiveTag magentaTag("Magenta", "Magenta component");
	retVal = retVal && archive.BeginTag(magentaTag);
	retVal = retVal && archive.Process(GetComponentValueRef(CI_MAGENTA));
	retVal = retVal && archive.EndTag(magentaTag);

	static iser::CArchiveTag yellowTag("Yellow", "Yellow component");
	retVal = retVal && archive.BeginTag(yellowTag);
	retVal = retVal && archive.Process(GetComponentValueRef(CI_YELLOW));
	retVal = retVal && archive.EndTag(yellowTag);

	static iser::CArchiveTag blackTag("Black", "Black component");
	retVal = retVal && archive.BeginTag(blackTag);
	retVal = retVal && archive.Process(GetComponentValueRef(CI_BLACK));
	retVal = retVal && archive.EndTag(blackTag);

	return retVal;
}


} // namespace icmm


