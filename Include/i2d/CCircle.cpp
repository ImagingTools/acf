#include "i2d/CCircle.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "i2d/CAffine2d.h"
#include "i2d/CRectangle.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


CCircle::CCircle()
:	m_radius(0)
{
}


CCircle::CCircle(double radius, const CVector2d& center)
:	BaseClass(center), m_radius(radius)
{
}


void CCircle::SetRadius(double radius)
{
	if (radius != m_radius){
		istd::CChangeNotifier notifier(this);

		m_radius = radius;
	}
}


CRectangle CCircle::GetBoundingBox() const
{
	return CRectangle(
				m_position.GetX() - m_radius, 
				m_position.GetY() - m_radius, 
				2 * m_radius, 
				2 * m_radius); 
}


bool CCircle::operator ==(const CCircle & ref) const
{
	return (ref.GetRadius() == GetRadius() && ref.GetPosition() == GetPosition()) ? true : false;
}


bool CCircle::operator !=(const CCircle & ref) const
{
	return !operator ==(ref);
}


// reimplemented (i2d::IObject2d)

bool CCircle::Transform(
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


bool CCircle::InvTransform(
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


bool CCircle::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CCircle* circlePtr = dynamic_cast<CCircle*>(&result);
	if (circlePtr == NULL){
		return false;
	}

	istd::CChangeNotifier changePtr(circlePtr);

	circlePtr->SetPosition(m_position);
	circlePtr->SetRadius(m_radius);

	return circlePtr->Transform(transformation, mode, errorFactorPtr);
}


bool CCircle::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CCircle* circlePtr = dynamic_cast<CCircle*>(&result);
	if (circlePtr == NULL){
		return false;
	}

	istd::CChangeNotifier changePtr(circlePtr);

	circlePtr->SetPosition(m_position);
	circlePtr->SetRadius(m_radius);

	return circlePtr->InvTransform(transformation, mode, errorFactorPtr);
}


// reimplemented (iser::ISerializable)

bool CCircle::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag radiusTag("Radius", "Circle radius");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(radiusTag);
	retVal = retVal && archive.Process(m_radius);
	retVal = retVal && archive.EndTag(radiusTag);

	return retVal;
}


} // namespace i2d

