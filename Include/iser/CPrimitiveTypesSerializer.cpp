#include <iser/CPrimitiveTypesSerializer.h>


// Qt includes
#include <QtCore/QString>
#include <QtCore/QStringList>

// ACF includes
#include <iser/CArchiveTag.h>


namespace iser
{


// static constants
static const QString s_timeFormat("yyyy-MM-dd hh-mm-ss zzz");
static const QString s_dateFormat("yyyy-MM-dd");


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
		for (		istd::CRanges::SwitchPoints::const_iterator iter = switchPoints.begin();
					iter != switchPoints.end();
					++iter){
			retVal = retVal && archive.BeginTag(positionTag);

			double position = *iter;
			retVal = retVal && archive.Process(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}
	else{
		switchPoints.clear();

		for (int i = 0; i < pointsCount; ++i){
			double position = 0;
			retVal = retVal && archive.BeginTag(positionTag);
			retVal = retVal && archive.Process(position);
			retVal = retVal && archive.EndTag(positionTag);

			if (!retVal){
				return false;
			}

			switchPoints.insert(position);
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
		for (		istd::CIntRanges::SwitchPoints::const_iterator iter = switchPoints.begin();
					iter != switchPoints.end();
					++iter){
			retVal = retVal && archive.BeginTag(positionTag);

			int position = *iter;
			retVal = retVal && archive.Process(position);

			retVal = retVal && archive.EndTag(positionTag);
		}
	}
	else{
		switchPoints.clear();

		for (int i = 0; i < pointsCount; ++i){
			int position = 0;
			retVal = retVal && archive.BeginTag(positionTag);
			retVal = retVal && archive.Process(position);
			retVal = retVal && archive.EndTag(positionTag);

			if (!retVal){
				return false;
			}

			switchPoints.insert(position);
		}
	}

	retVal = retVal && archive.EndTag(switchPointsTag);

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime)
{
	if (archive.IsStoring()){
		QString dateTimeString = dateTime.toString(s_timeFormat);

		return archive.Process(dateTimeString);
	}
	else{
		QString dateTimeString;

		if (archive.Process(dateTimeString)){
			dateTime = QDateTime::fromString(dateTimeString, s_timeFormat);

			return true;
		}
	}

	return false;
}


bool CPrimitiveTypesSerializer::SerializeDate(iser::IArchive& archive, QDate& date)
{
	if (archive.IsStoring()){
		QString dateString = date.toString(s_dateFormat);

		return archive.Process(dateString);
	}
	else{
		QString dateString;

		if (archive.Process(dateString)){
			date = QDate::fromString(dateString, s_dateFormat);

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
		point = QPointF(x, y);
	}

	return retVal;
}


bool CPrimitiveTypesSerializer::SerializeQStringList(
			iser::IArchive& archive,
			QStringList& stringList,
			const QByteArray& containerTagName,
			const QByteArray& elementTagName)
{
	return SerializeContainer<QStringList>(archive, stringList, containerTagName, elementTagName);
}


} // namespace iser


