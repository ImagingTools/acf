#include <icmm/CLab.h>


#include <iser/CArchiveTag.h>


namespace icmm
{


// static constants
static const iser::CArchiveTag s_lTag("L", "L component of Lab", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_aTag("A", "A component of Lab", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_bTag("B", "B component of Lab", iser::CArchiveTag::TT_LEAF);


// public methods

bool CLab::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_lTag);
	retVal = retVal && archive.Process(GetElementRef(CI_L));
	retVal = retVal && archive.EndTag(s_lTag);

	retVal = retVal && archive.BeginTag(s_aTag);
	retVal = retVal && archive.Process(GetElementRef(CI_A));
	retVal = retVal && archive.EndTag(s_aTag);

	retVal = retVal && archive.BeginTag(s_bTag);
	retVal = retVal && archive.Process(GetElementRef(CI_B));
	retVal = retVal && archive.EndTag(s_bTag);

	return retVal;
}


double CLab::GetDeltaE(const CLab& lab) const
{
	return GetDistance(lab);
}


} // namespace icmm


