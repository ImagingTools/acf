#include <icmm/CHsv.h>


#include <iser/CArchiveTag.h>


namespace icmm
{


// static constants
static const iser::CArchiveTag s_hueTag("Hue", "Hue component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_saturationTag("Saturation", "Saturation component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_valueTag("Value", "Value component", iser::CArchiveTag::TT_LEAF);


// public methods

bool CHsv::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_hueTag);
	retVal = retVal && archive.Process(GetElementRef(CI_HUE));
	retVal = retVal && archive.EndTag(s_hueTag);

	retVal = retVal && archive.BeginTag(s_saturationTag);
	retVal = retVal && archive.Process(GetElementRef(CI_SATURATION));
	retVal = retVal && archive.EndTag(s_saturationTag);

	retVal = retVal && archive.BeginTag(s_valueTag);
	retVal = retVal && archive.Process(GetElementRef(CI_VALUE));
	retVal = retVal && archive.EndTag(s_valueTag);

	return retVal;
}


} // namespace icmm


