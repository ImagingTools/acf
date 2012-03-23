#include "iser/CPrimitiveTypesSerializer.h"


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iser
{


// public static methods

bool CPrimitiveTypesSerializer::SerializeRange(iser::IArchive& archive, istd::CRange& range)
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


bool CPrimitiveTypesSerializer::SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime)
{
	if (archive.IsStoring()){
		QString dateTimeString = dateTime.toString(Qt::ISODate);

		return archive.Process(dateTimeString);
	}
	else{
		QString dateTimeString;

		if (archive.Process(dateTimeString)){
			dateTime = QDateTime::fromString(dateTimeString, Qt::ISODate);;
		}
	}

	return false;
}


} // namespace iser


