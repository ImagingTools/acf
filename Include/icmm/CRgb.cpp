#include <icmm/CRgb.h>


#include <iser/CArchiveTag.h>


namespace icmm
{


// static constants
static const iser::CArchiveTag s_cyanTag("Red", "Red component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_magentaTag("Green", "Green component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_yellowTag("Blue", "Blue component", iser::CArchiveTag::TT_LEAF);


// public methods

bool CRgb::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_cyanTag);
	retVal = retVal && archive.Process(GetElementRef(CI_RED));
	retVal = retVal && archive.EndTag(s_cyanTag);

	retVal = retVal && archive.BeginTag(s_magentaTag);
	retVal = retVal && archive.Process(GetElementRef(CI_GREEN));
	retVal = retVal && archive.EndTag(s_magentaTag);

	retVal = retVal && archive.BeginTag(s_yellowTag);
	retVal = retVal && archive.Process(GetElementRef(CI_BLUE));
	retVal = retVal && archive.EndTag(s_yellowTag);

	return retVal;
}


} // namespace icmm


