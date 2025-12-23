#include <i2d/CArc.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <i2d/CAffine2d.h>
#include <i2d/CRectangle.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <istd/TDelPtr.h>


namespace i2d
{


// static constants
static iser::CArchiveTag s_startAngleTag("StartAngle", "Start angle", iser::CArchiveTag::TT_LEAF);
static iser::CArchiveTag s_endAngleTag("EndAngle", "End angle", iser::CArchiveTag::TT_LEAF);

// public methods

CArc::CArc()
:	m_startAngle(0),
	m_endAngle(0)
{
}


CArc::CArc(	double startAngle,
			double endAngle,
			const double radius,
			const CVector2d& center)
:	BaseClass(radius, center),
	m_startAngle(startAngle),
	m_endAngle(endAngle)
{
}


void CArc::SetStartAngle(double angle)
{
	if (angle != m_startAngle){
		istd::CChangeNotifier notifier(this);

		m_startAngle = angle;
	}
}


void CArc::SetEndAngle(double angle)
{
	if (angle != m_endAngle){
		istd::CChangeNotifier notifier(this);

		m_endAngle = angle;
	}
}


bool CArc::operator==(const CArc& ref) const
{
	return (ref.GetStartAngle() == GetStartAngle() &&
				ref.GetEndAngle() == GetEndAngle() &&
				ref.GetRadius() == GetRadius() &&
				ref.GetPosition() == GetPosition());
}


bool CArc::operator!=(const CArc& ref) const
{
	return !operator==(ref);
}


CVector2d CArc::GetPositionFromAlpha(double alpha) const
{
	double angle = m_startAngle + ((m_endAngle - m_startAngle) * alpha);
	i2d::CVector2d line;
	line.Init(angle, m_radius);
	line = line.GetTranslated(GetCenter());

	return line;
}


// reimplemented (i2d::IObject2d)

CRectangle CArc::GetBoundingBox() const
{
	return CRectangle(
		m_position.GetX() - m_radius,
		m_position.GetY() - m_radius,
		2 * m_radius, 
		2 * m_radius); 
}


bool CArc::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos;
	if (!transformation.GetPositionAt(m_position, transPos, mode)){
		return false;
	}

	CAffine2d affine;
	if (!transformation.GetLocalTransform(m_position, affine, mode)){
		return false;
	}

	istd::CChangeNotifier changePtr(this);

	double scale = affine.GetDeformMatrix().GetApproxScale();

	SetPosition(transPos);
	SetRadius(m_radius * scale);

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	return true;
}


bool CArc::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos;
	if (!transformation.GetInvPositionAt(m_position, transPos, mode)){
		return false;
	}

	CAffine2d affine;
	if (!transformation.GetLocalInvTransform(m_position, affine, mode)){
		return false;
	}

	istd::CChangeNotifier changePtr(this);

	double scale = affine.GetDeformMatrix().GetApproxScale();

	SetPosition(transPos);
	SetRadius(m_radius * scale);

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	return true;
}


bool CArc::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CArc* arcPtr = dynamic_cast<CArc*>(&result);
	if (arcPtr == NULL){
		return false;
	}

	istd::CChangeNotifier changePtr(arcPtr);

	arcPtr->SetPosition(m_position);
	arcPtr->SetRadius(m_radius);
	arcPtr->SetStartAngle(m_startAngle);
	arcPtr->SetEndAngle(m_endAngle);

	return arcPtr->Transform(transformation, mode, errorFactorPtr);
}


bool CArc::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CArc* arcPtr = dynamic_cast<CArc*>(&result);
	if (arcPtr == NULL){
		return false;
	}

	istd::CChangeNotifier changePtr(arcPtr);

	arcPtr->SetPosition(m_position);
	arcPtr->SetRadius(m_radius);
	arcPtr->SetStartAngle(m_startAngle);
	arcPtr->SetEndAngle(m_endAngle);

	return arcPtr->InvTransform(transformation, mode, errorFactorPtr);
}


// reimplemented (istd::IChangeable)

int CArc::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CArc::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CArc* arcPtr = dynamic_cast<const CArc*>(&object);

	if (arcPtr != NULL){
		istd::CChangeNotifier notifier(this);

		SetStartAngle(arcPtr->GetStartAngle());
		SetEndAngle(arcPtr->GetEndAngle());

		CCircle::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CArc::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new CArc);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


// reimplemented (iser::ISerializable)

bool CArc::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(s_startAngleTag);
	retVal = retVal && archive.Process(m_startAngle);
	retVal = retVal && archive.EndTag(s_startAngleTag);

	retVal = retVal && archive.BeginTag(s_endAngleTag);
	retVal = retVal && archive.Process(m_endAngle);
	retVal = retVal && archive.EndTag(s_endAngleTag);

	return retVal;
}

} // namespace i2d


