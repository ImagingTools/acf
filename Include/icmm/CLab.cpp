#include "icmm/CLab.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CLab::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag lTag("L", "L component of Lab");
	retVal = retVal && archive.BeginTag(lTag);
	retVal = retVal && archive.Process(GetElementRef(CI_L));
	retVal = retVal && archive.EndTag(lTag);

	static iser::CArchiveTag aTag("A", "A component of Lab");
	retVal = retVal && archive.BeginTag(aTag);
	retVal = retVal && archive.Process(GetElementRef(CI_A));
	retVal = retVal && archive.EndTag(aTag);

	static iser::CArchiveTag bTag("B", "B component of Lab");
	retVal = retVal && archive.BeginTag(bTag);
	retVal = retVal && archive.Process(GetElementRef(CI_B));
	retVal = retVal && archive.EndTag(bTag);

	return retVal;
}


} // namespace icmm


