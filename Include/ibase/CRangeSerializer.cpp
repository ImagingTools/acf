#include "ibase/CRangeSerializer.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{


// public static methods

bool CRangeSerializer::SerializeRange(iser::IArchive& archive, istd::CRange& range)
{
	bool retVal = true;

	static iser::CArchiveTag minValueTag("MinValue", "Minimal range value");
	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(range.GetMinValueRef());
	retVal = retVal && archive.EndTag(minValueTag);

	static iser::CArchiveTag maxValueTag("MaxValue", "Maximal range value");
	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(range.GetMaxValueRef());
	retVal = retVal && archive.EndTag(maxValueTag);

	return retVal;
}


} // namespace ibase


