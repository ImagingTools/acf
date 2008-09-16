#include "icmm/CHsv.h"


#include "iser/CArchiveTag.h"


namespace icmm
{


// public methods

bool CHsv::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag hueTag("Hue", "Hue component");
	retVal = retVal && archive.BeginTag(hueTag);
	retVal = retVal && archive.Process(GetElementRef(CI_HUE));
	retVal = retVal && archive.EndTag(hueTag);

	static iser::CArchiveTag saturationTag("Saturation", "Saturation component");
	retVal = retVal && archive.BeginTag(saturationTag);
	retVal = retVal && archive.Process(GetElementRef(CI_SATURATION));
	retVal = retVal && archive.EndTag(saturationTag);

	static iser::CArchiveTag valueTag("Value", "Value component");
	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && archive.Process(GetElementRef(CI_VALUE));
	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


} // namespace icmm


