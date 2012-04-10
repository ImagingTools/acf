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


CRectangle CAnnulusSegment::GetBoundingBox() const
{
	return BaseClass::GetBoundingBox();	// TODO: implement more exact bounding box of annulus segment
}


bool CAnnulusSegment::Contains(const i2d::CVector2d& point) const
{
	double r = point.GetLength();
	double phi = atan2(point.GetY(), point.GetX());

	if (r < GetInnerRadius() || r > GetOuterRadius()){
		return false;
	}

	if(m_angleRange.IsEmpty()){
		return true;
	}

	if ( qAbs(phi) > I_2PI)
		phi = fmod(phi, I_2PI);

	if (phi < 0)
		phi += I_2PI;

	if (GetBeginAngle() < GetEndAngle()){
		if(phi > GetBeginAngle() && phi < GetEndAngle()){
			return true;
		}
	}
	else if (GetBeginAngle() > GetEndAngle()){
		if(phi > GetBeginAngle() && phi > GetEndAngle()){
			return true;
		}
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CAnnulusSegment::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass::Serialize(archive);

	static iser::CArchiveTag beginAngleTag("BeginAngle", "Start angle of the segment");
	static iser::CArchiveTag endAngleTag("EndAngle", "End angle of the segment");

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


