#include "i2d/CAnnulus.h"


namespace i2d
{


CAnnulus::CAnnulus()
	:m_innerRadius(0),
	m_outerRadius(0)
{
}


CAnnulus::CAnnulus(double innerRadius, double outerRadius, const i2d::CVector2d& center)
	:BaseClass(center),
	m_innerRadius(innerRadius),
	m_outerRadius(outerRadius)
{
	I_ASSERT(IsValid());
}


bool CAnnulus::IsValid() const
{
	return (m_innerRadius <= m_outerRadius);
}


double CAnnulus::GetInnerRadius() const
{
	return m_innerRadius;
}


double CAnnulus::GetOuterRadius() const
{
	return m_outerRadius;
}


CCircle CAnnulus::GetInnerCircle() const
{
	return CCircle(m_innerRadius, m_center);
}


CCircle CAnnulus::GetOuterCircle() const
{
	return CCircle(m_outerRadius, m_center);
}


// reimplemented (i2d::IObject2d)

CRectangle CAnnulus::GetBoundingBox() const
{
	return CRectangle(m_center.GetY() - m_outerRadius, 
		m_center.GetX() - m_outerRadius,
		m_center.GetY() + m_outerRadius,
		m_center.GetX() + m_outerRadius);
}


} // namespace i2d


