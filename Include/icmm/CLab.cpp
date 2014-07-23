#include "icmm/CLab.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CLab::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag lTag("L", "L component of Lab", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag aTag("A", "A component of Lab", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag bTag("B", "B component of Lab", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(lTag);
	retVal = retVal && archive.Process(GetElementRef(CI_L));
	retVal = retVal && archive.EndTag(lTag);

	retVal = retVal && archive.BeginTag(aTag);
	retVal = retVal && archive.Process(GetElementRef(CI_A));
	retVal = retVal && archive.EndTag(aTag);

	retVal = retVal && archive.BeginTag(bTag);
	retVal = retVal && archive.Process(GetElementRef(CI_B));
	retVal = retVal && archive.EndTag(bTag);

	return retVal;
}


double CLab::GetDeltaE(const CLab& lab) const
{
	return GetDistance(lab);
}


} // namespace icmm


