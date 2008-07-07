#include "i2d/CCircle.h"
#include "i2d/CRectangle.h"


namespace i2d
{	


CCircle::CCircle()
{
	m_radius = 0.0;
}


CCircle::CCircle(double radius, const CVector2d& center)
{
	m_radius = radius;
	m_center = center;
}


double CCircle::GetRadius() const
{
	return m_radius;
}


// reimplemented (IObject2d)

CVector2d CCircle::GetCenter() const
{
	return m_center;
}


CRectangle CCircle::GetBoundingBox() const
{
	return CRectangle(
				m_center.GetY() - m_radius, 
				m_center.GetX() - m_radius, 
				m_center.GetY() + m_radius, 
				m_center.GetX() + m_radius); 
}


} // namespace i2d

