#include "icmm/CCmy.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CCmy::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag cyanTag("Cyan", "Cyan component");
	retVal = retVal && archive.BeginTag(cyanTag);
	retVal = retVal && archive.Process(GetElementRef(CI_CYAN));
	retVal = retVal && archive.EndTag(cyanTag);

	static iser::CArchiveTag magentaTag("Magenta", "Magenta component");
	retVal = retVal && archive.BeginTag(magentaTag);
	retVal = retVal && archive.Process(GetElementRef(CI_MAGENTA));
	retVal = retVal && archive.EndTag(magentaTag);

	static iser::CArchiveTag yellowTag("Yellow", "Yellow component");
	retVal = retVal && archive.BeginTag(yellowTag);
	retVal = retVal && archive.Process(GetElementRef(CI_YELLOW));
	retVal = retVal && archive.EndTag(yellowTag);

	return retVal;
}


} // namespace icmm


