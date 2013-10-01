#include "i2d/CCircle.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "i2d/CAffine2d.h"
#include "i2d/CRectangle.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "istd/TDelPtr.h"


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


bool CCircle::operator==(const CCircle& ref) const
{
	return (ref.GetRadius() == GetRadius() && ref.GetPosition() == GetPosition());
}


bool CCircle::operator!=(const CCircle& ref) const
{
	return !operator==(ref);
}


bool CCircle::IsIntersectedBy(const CCircle& circle, bool isFilled) const
{
	double distance2 = m_position.GetDistance2(circle.m_position);
	double radiusSum = m_radius + circle.m_radius;

	if (!isFilled){
		double radiusDiff = qFabs(m_radius - circle.m_radius);

		if (distance2 < (radiusDiff * radiusDiff)){
			return false;

		}
	}

	return distance2 < (radiusSum * radiusSum);
}


bool CCircle::IsIntersectedBy(const CLine2d& line, bool isFilled) const
{
	const i2d::CVector2d& lineDiff = line.GetDiffVector();
	const i2d::CVector2d& point1ToCenter = line.GetPoint1() - m_position;

	bool isPoint1Inside = point1ToCenter.GetLength2() <= m_radius * m_radius;
	if (isPoint1Inside && isFilled){
		return true;
	}

	bool isPoint2Inside = m_position.GetDistance2(line.GetPoint2()) <= m_radius * m_radius;
	if (isPoint2Inside && isFilled){
		return true;
	}

	if (isPoint1Inside != isPoint2Inside){	// one is inside, the other is outside -> intersects
		return true;
	}

	double a = lineDiff.GetLength2();
	double b = lineDiff.GetDotProduct(point1ToCenter);
	double c = point1ToCenter.GetLength2() - m_radius * m_radius;

	double D = b * b - a * c;
	if (D >= 0){
		double sqrtD = qSqrt(D);

		double alpha1a = (-b - sqrtD);
		double alpha2a = (-b + sqrtD);
		if (((alpha1a <= a) && (alpha1a >= 0)) || ((alpha2a <= a) && (alpha2a >= 0))){
			return true;
		}
	}

	return false;
}


int CCircle::GetIntersectionAlphas(const CLine2d& line, double result[2]) const
{
	int retVal = 0;

	const i2d::CVector2d& lineDiff = line.GetDiffVector();
	const i2d::CVector2d& point1ToCenter = line.GetPoint1() - m_position;

	double a = lineDiff.GetLength2();
	double b = lineDiff.GetDotProduct(point1ToCenter);
	double c = point1ToCenter.GetLength2() - m_radius * m_radius;

	double D = b * b - a * c;
	if (D >= 0){
		double sqrtD = qSqrt(D);

		if (D < I_BIG_EPSILON * I_BIG_EPSILON){
			double alphaa = -b;

			if ((alphaa >= 0) && (alphaa <= a)){
				if (a > I_BIG_EPSILON * I_BIG_EPSILON){
					result[retVal++] = alphaa / a;
				}
				else{
					result[retVal++] = 0.5;
				}
			}
		}
		else{
			double alpha1a = (-b - sqrtD);
			double alpha2a = (-b + sqrtD);
			if ((alpha1a <= a) && (alpha2a >= 0)){
				if (a > I_BIG_EPSILON * I_BIG_EPSILON){
					if (alpha1a >= 0){
						result[retVal++] = alpha1a / a;
					}

					if (alpha2a <= a){
						result[retVal++] = alpha2a / a;
					}
				}
				else{
					result[retVal++] = 0.5;
				}
			}
		}
	}

	return retVal;
}


int CCircle::GetIntersectionPoints(const CLine2d& line, i2d::CVector2d& result1, i2d::CVector2d& result2) const
{
	double alphas[2] = {0, 0};
	int retVal = GetIntersectionAlphas(line, alphas);
	if (retVal >= 1){
		result1 = line.GetPositionFromAlpha(alphas[0]);
		if (retVal >= 2){
			result2 = line.GetPositionFromAlpha(alphas[1]);
		}
	}
	return retVal;
}


// reimplemented (i2d::IObject2d)

CRectangle CCircle::GetBoundingBox() const
{
	return CRectangle(
				m_position.GetX() - m_radius, 
				m_position.GetY() - m_radius, 
				2 * m_radius, 
				2 * m_radius); 
}


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


// reimplemented (istd::IChangeable)

int CCircle::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CCircle::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CCircle* circlePtr = dynamic_cast<const CCircle*>(&object);

	if (circlePtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetPosition(circlePtr->GetPosition());
		SetRadius(circlePtr->GetRadius());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}	

	return false;
}


istd::IChangeable* CCircle::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CCircle> clonePtr(new CCircle);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CCircle::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag radiusTag("Radius", "Circle radius");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | CF_MODEL);

	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(radiusTag);
	retVal = retVal && archive.Process(m_radius);
	retVal = retVal && archive.EndTag(radiusTag);

	return retVal;
}


} // namespace i2d


