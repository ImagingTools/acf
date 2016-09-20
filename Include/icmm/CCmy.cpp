#include <icmm/CCmy.h>


// ACF includes
#include <iser/CArchiveTag.h>


namespace icmm
{


// static constants
static const iser::CArchiveTag s_cyanTag("Cyan", "Cyan component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_magentaTag("Magenta", "Magenta component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_yellowTag("Yellow", "Yellow component", iser::CArchiveTag::TT_LEAF);


// public methods

bool CCmy::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_cyanTag);
	retVal = retVal && archive.Process(GetElementRef(CI_CYAN));
	retVal = retVal && archive.EndTag(s_cyanTag);

	retVal = retVal && archive.BeginTag(s_magentaTag);
	retVal = retVal && archive.Process(GetElementRef(CI_MAGENTA));
	retVal = retVal && archive.EndTag(s_magentaTag);

	retVal = retVal && archive.BeginTag(s_yellowTag);
	retVal = retVal && archive.Process(GetElementRef(CI_YELLOW));
	retVal = retVal && archive.EndTag(s_yellowTag);

	return retVal;
}


} // namespace icmm


