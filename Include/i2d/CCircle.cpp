#include "i2d/CCircle.h"


#include "istd/TChangeNotifier.h"

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


// reimplemented (i2d::IObject2d)

bool CCircle::Transform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	// TODO: implement geometrical transformations for circle.
	return false;
}


bool CCircle::InvTransform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	// TODO: implement geometrical transformations for circle.
	return false;
}


bool CCircle::GetTransformed(
			const ITransformation2d& /*transformation*/,
			IObject2d& /*result*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	// TODO: implement geometrical transformations for circle.
	return false;
}


bool CCircle::GetInvTransformed(
			const ITransformation2d& /*transformation*/,
			IObject2d& /*result*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	// TODO: implement geometrical transformations for circle.
	return false;
}


// reimplemented (iser::ISerializable)

bool CCircle::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	static iser::CArchiveTag radiusTag("Radius", "Circle radius");
	retVal = retVal && archive.BeginTag(radiusTag);
	retVal = retVal && archive.Process(m_radius);
	retVal = retVal && archive.EndTag(radiusTag);

	return retVal;
}


} // namespace i2d

