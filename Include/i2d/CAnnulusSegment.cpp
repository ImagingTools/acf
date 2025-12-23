#include <i2d/CAnnulusSegment.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <i2d/CRectangle.h>
#include <istd/TDelPtr.h>


namespace i2d
{


// static constants
static const iser::CArchiveTag s_beginAngleTag("BeginAngle", "Start angle of the segment", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_endAngleTag("EndAngle", "End angle of the segment", iser::CArchiveTag::TT_LEAF);


// public methods

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
	// normalize angle to the range of [0..360)
	while (angle >= I_2PI){
		angle -= I_2PI;
	}

	while (angle < 0){
		angle += I_2PI;
	}

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
	// normalize angle to the range of [0..360)
	while (angle >= I_2PI){
		angle -= I_2PI;
	}

	while (angle < 0){
		angle += I_2PI;
	}

	if (angle != m_angleRange.GetMaxValue()){
		istd::CChangeNotifier notifier(this);

		m_angleRange.SetMaxValue(angle);
	}
}


bool CAnnulusSegment::Contains(const i2d::CVector2d& point) const
{
	i2d::CVector2d refPoint = point - GetPosition();

	double r = refPoint.GetLength();
	double phi = qAtan2(refPoint.GetY(), refPoint.GetX());

	if (r < GetInnerRadius() || r > GetOuterRadius()){
		return false;
	}

	if (m_angleRange.IsEmpty()){
		return true;
	}

	if (qAbs(phi) > I_2PI)
		phi = fmod(phi, I_2PI);

	if (phi < 0)
		phi += I_2PI;

	if (GetBeginAngle() < GetEndAngle()){
		if (phi >= GetBeginAngle() && phi <= GetEndAngle()){
			return true;
		}
	}
	else if (GetBeginAngle() > GetEndAngle()){
		if (phi >= GetBeginAngle() && phi <= GetEndAngle()){
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


bool CAnnulusSegment::Transform(const ITransformation2d& transformation,
						 ITransformation2d::ExactnessMode mode,
						 double* errorFactorPtr)
{
	// check for angle translation
	i2d::CLine2d tempVector(0,0, 1,0);
	if (tempVector.Transform(transformation, mode, errorFactorPtr)){
		double angle = tempVector.GetDirectionAngle();
		SetBeginAngle(GetBeginAngle() + angle);
		SetEndAngle(GetEndAngle() + angle);
	}

	return BaseClass::Transform(transformation, mode, errorFactorPtr);
}


bool CAnnulusSegment::InvTransform(const ITransformation2d& transformation,
								ITransformation2d::ExactnessMode mode,
								double* errorFactorPtr)
{
	// check for angle translation
	i2d::CLine2d tempVector(0,0, 1,0);
	if (tempVector.InvTransform(transformation, mode, errorFactorPtr)){
		double angle = tempVector.GetDirectionAngle();
		SetBeginAngle(GetBeginAngle() + angle);
		SetEndAngle(GetEndAngle() + angle);
	}

	return BaseClass::InvTransform(transformation, mode, errorFactorPtr);
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

		m_position = annulusSegmentPtr->m_position;
		m_radiusRange = annulusSegmentPtr->m_radiusRange;
		m_angleRange = annulusSegmentPtr->m_angleRange;

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	const CAnnulus* annulusPtr = dynamic_cast<const CAnnulus*>(&object);
	if (annulusPtr != NULL){
		istd::CChangeNotifier notifier(this);

		m_position = annulusPtr->GetPosition();
		m_radiusRange = istd::CRange(annulusPtr->GetInnerRadius(), annulusPtr->GetOuterRadius());
		m_angleRange = istd::CRange(0, I_2PI-0.0001);

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CAnnulusSegment::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new CAnnulusSegment);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


// reimplemented (iser::ISerializable)

bool CAnnulusSegment::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	quint32 versionNumber = 0;
	if (!archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, versionNumber) || (versionNumber > 1177)){
		retVal = retVal && archive.BeginTag(s_beginAngleTag);
		retVal = retVal && archive.Process(m_angleRange.GetMinValueRef());
		retVal = retVal && archive.EndTag(s_beginAngleTag);

		retVal = retVal && archive.BeginTag(s_endAngleTag);
		retVal = retVal && archive.Process(m_angleRange.GetMaxValueRef());
		retVal = retVal && archive.EndTag(s_endAngleTag);
	}
	else{
		retVal = retVal && archive.Process(m_angleRange.GetMinValueRef());
		retVal = retVal && archive.Process(m_angleRange.GetMaxValueRef());
	}

	return retVal;
}


} // namespace i2d


