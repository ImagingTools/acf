#include "i2d/CAnnulusSegment.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "i2d/CRectangle.h"
#include "istd/TDelPtr.h"

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


bool CAnnulusSegment::Contains(const i2d::CVector2d& point) const
{
	double r = point.GetLength();
	double phi = qAtan2(point.GetY(), point.GetX());

	if (r < GetInnerRadius() || r > GetOuterRadius()){
		return false;
	}

	if (m_angleRange.IsEmpty()){
		return true;
	}

	if ( qAbs(phi) > I_2PI)
		phi = fmod(phi, I_2PI);

	if (phi < 0)
		phi += I_2PI;

	if (GetBeginAngle() < GetEndAngle()){
		if (phi > GetBeginAngle() && phi < GetEndAngle()){
			return true;
		}
	}
	else if (GetBeginAngle() > GetEndAngle()){
		if (phi > GetBeginAngle() && phi > GetEndAngle()){
			return true;
		}
	}

	return false;
}


// reimplemented (i2d::IObject2d)

CRectangle CAnnulusSegment::GetBoundingBox() const
{
	return BaseClass::GetBoundingBox();	// TODO: implement more exact bounding box of annulus segment
}


// reimplemented (istd::IChangeable)

int CAnnulusSegment::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CAnnulusSegment::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CAnnulusSegment* annulusSegmentPtr = dynamic_cast<const CAnnulusSegment*>(&object);

	if (annulusSegmentPtr != NULL){
		istd::CChangeNotifier notifier(this);

		SetPosition(annulusSegmentPtr->GetPosition());
		SetInnerRadius(annulusSegmentPtr->GetInnerRadius());
		SetOuterRadius(annulusSegmentPtr->GetOuterRadius());
		SetBeginAngle(annulusSegmentPtr->GetBeginAngle());
		SetEndAngle(annulusSegmentPtr->GetEndAngle());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}	

	return false;
}


istd::IChangeable* CAnnulusSegment::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CAnnulusSegment> clonePtr(new CAnnulusSegment);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CAnnulusSegment::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag beginAngleTag("BeginAngle", "Start angle of the segment");
	static iser::CArchiveTag endAngleTag("EndAngle", "End angle of the segment");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | CF_MODEL);

	bool retVal = BaseClass::Serialize(archive);

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


