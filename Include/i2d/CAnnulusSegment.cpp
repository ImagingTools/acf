#include "i2d/CAnnulusSegment.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "i2d/CRectangle.h"


namespace i2d
{	


CAnnulusSegment::CAnnulusSegment()
{
}


CAnnulusSegment::CAnnulusSegment(
			const i2d::CVector2d& center,
			double innerRadius,
			double outerRadius,
			double beginAngle,
			double endAngle)
:	BaseClass(center, innerRadius, outerRadius),
	m_angleRange(beginAngle, endAngle)
{
}


double CAnnulusSegment::GetBeginAngle() const
{
	return m_angleRange.GetMinValue();
}


void CAnnulusSegment::SetBeginAngle(double angle)
{
	if (angle != m_angleRange.GetMinValue()){
		istd::CChangeNotifier notifier(this);

		m_angleRange.SetMinValue(angle);
	}
}


double CAnnulusSegment::GetEndAngle() const
{
	return m_angleRange.GetMaxValue();
}


void CAnnulusSegment::SetEndAngle(double angle)
{
	if (angle != m_angleRange.GetMaxValue()){
		istd::CChangeNotifier notifier(this);

		m_angleRange.SetMaxValue(angle);
	}
}


// reimplemented (i2d::IObject2d)

CRectangle CAnnulusSegment::GetBoundingBox() const
{
	return BaseClass::GetBoundingBox();	// TODO: implement more exact bounding box of annulus segment
}


// reimplemented (iser::ISerializable)

bool CAnnulusSegment::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass::Serialize(archive);

	static iser::CArchiveTag beginAngleTag("BeginAngle", "First angle of segment");
	static iser::CArchiveTag endAngleTag("BeginAngle", "First angle of segment");


	if (archive.IsStoring()){
		double beginAngle = GetBeginAngle();
		double endAngle = GetEndAngle();

		retVal = retVal && archive.Process(beginAngle);
		retVal = retVal && archive.Process(endAngle);
	}
	else{
		double beginAngle = 0;
		double endAngle = 0;

		retVal = retVal && archive.Process(beginAngle);
		retVal = retVal && archive.Process(endAngle);

		if (!retVal){
			return false;
		}

		SetBeginAngle(beginAngle);
		SetEndAngle(endAngle);
	}

	return retVal;
}


} // namespace i2d


