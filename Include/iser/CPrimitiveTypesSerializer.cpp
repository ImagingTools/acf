#include "iser/CPrimitiveTypesSerializer.h"


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "iser/CArchiveTag.h"


namespace iser
{


// public static methods

bool CPrimitiveTypesSerializer::SerializeRange(iser::IArchive& archive, istd::CRange& range)
{
	static iser::CArchiveTag minValueTag("MinValue", "Minimal range value", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag maxValueTag("MaxValue", "Maximal range value", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(range.GetMinValueRef());
	retVal = retVal && archive.EndTag(minValueTag);

	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(range.GetMaxValueRef());
	retVal = retVal && archive.EndTag(maxValueTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeIntRange(iser::IArchive& archive, istd::CIntRange& range)
{
	static iser::CArchiveTag minValueTag("MinValue", "Minimal range value", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag maxValueTag("MaxValue", "Maximal range value", iser::CArchiveTag::TT_LEAF);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(range.GetMinValueRef());
	retVal = retVal && archive.EndTag(minValueTag);

	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(range.GetMaxValueRef());
	retVal = retVal && archive.EndTag(maxValueTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeRanges(iser::IArchive& archive, istd::CRanges& ranges)
{
	static iser::CArchiveTag beginStateTag("BeginState", "Begin state", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag switchPointsTag("SwitchPoints", "List of switch point positions", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag positionTag("Position", "Switch point position", iser::CArchiveTag::TT_LEAF, &switchPointsTag);

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	istd::CRanges::SwitchPoints& switchPoints = ranges.GetSwitchPointsRef();

	retVal = retVal && archive.BeginTag(beginStateTag);
	if (isStoring){
		bool beginState = ranges.GetBeginState();

		retVal = retVal && archive.Process(beginState);
	}
	else{
		bool beginState = false;

		retVal = retVal && archive.Process(beginState);

		ranges.SetBeginState(beginState);
	}

	retVal = retVal && archive.EndTag(beginStateTag);

	int pointsCount = int(switchPoints.size());

	retVal = retVal && archive.BeginMultiTag(switchPointsTag, positionTag, pointsCount);

	if (!retVal){
		return false;
	}

	if (isStoring){
		for (		istd::CRanges::SwitchPoints::iterator iter = switchPoints.begin();
					iter != switchPoints.end();
					++iter){
			retVal = retVal && archive.BeginTag(positionTag);

			double position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}
	else{
		switchPoints.clear();

		for (int i = 0; i < pointsCount; ++i){
			retVal = retVal && archive.BeginTag(positionTag);

			double position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}

	retVal = retVal && archive.EndTag(switchPointsTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeIntRanges(iser::IArchive& archive, istd::CIntRanges& ranges)
{
	static iser::CArchiveTag beginStateTag("BeginState", "Begin state", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag switchPointsTag("SwitchPoints", "List of switch point positions", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag positionTag("Position", "Switch point position", iser::CArchiveTag::TT_LEAF, &switchPointsTag);

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	istd::CIntRanges::SwitchPoints& switchPoints = ranges.GetSwitchPointsRef();

	retVal = retVal && archive.BeginTag(beginStateTag);
	if (isStoring){
		bool beginState = ranges.GetBeginState();

		retVal = retVal && archive.Process(beginState);
	}
	else{
		bool beginState = false;

		retVal = retVal && archive.Process(beginState);

		ranges.SetBeginState(beginState);
	}

	retVal = retVal && archive.EndTag(beginStateTag);

	int pointsCount = int(switchPoints.size());

	retVal = retVal && archive.BeginMultiTag(switchPointsTag, positionTag, pointsCount);

	if (!retVal){
		return false;
	}

	if (isStoring){
		for (		istd::CIntRanges::SwitchPoints::iterator iter = switchPoints.begin();
					iter != switchPoints.end();
					++iter){
			retVal = retVal && archive.BeginTag(positionTag);

			int position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}
	else{
		switchPoints.clear();

		for (int i = 0; i < pointsCount; ++i){
			retVal = retVal && archive.BeginTag(positionTag);

			int position = 0;
			retVal = retVal && archive.Process(position);
			switchPoints.insert(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}

	retVal = retVal && archive.EndTag(switchPointsTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime)
{
	const static QString timeFormat("yyyy-MM-dd hh-mm-ss zzz");

	if (archive.IsStoring()){
		QString dateTimeString = dateTime.toString(timeFormat);

		return archive.Process(dateTimeString);
	}
	else{
		QString dateTimeString;

		if (archive.Process(dateTimeString)){
			dateTime = QDateTime::fromString(dateTimeString, timeFormat);

			return true;
		}
	}

	return false;
}


bool CPrimitiveTypesSerializer::SerializeQPointF(iser::IArchive& archive, QPointF& point)
{
	static iser::CArchiveTag xTag("X", "Horizontal position", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag yTag("Y", "Vertical position", iser::CArchiveTag::TT_LEAF);

	float x = point.x();
	float y = point.y();

	bool retVal = archive.BeginTag(xTag);
	retVal = retVal && archive.Process(x);
	retVal = retVal && archive.EndTag(xTag);

	retVal = retVal && archive.BeginTag(yTag);
	retVal = retVal && archive.Process(y);
	retVal = retVal && archive.EndTag(yTag);

	if (!archive.IsStoring()){
		point = QPoint(x, y);
	}

	return retVal;
}


} // namespace iser


