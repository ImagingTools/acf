#include "i2d/CTransform.h"


namespace i2d
{	


CTransform::CTransform()
	:m_m11(1.0),
	m_m12(0.0),
	m_m21(0.0),
	m_m22(1.0),
	m_dx(0.0),
	m_dy(0.0)
{
}


CTransform::CTransform(const CVector2d& translation, const CVector2d& scale, double angle)
{
	angle = imath::GetRadianFromDegree(angle);

	double sina = sin(angle);
    double cosa = cos(angle);

	m_m11 = scale.GetX() * cosa;
	m_m12 = scale.GetY() * sina;
	m_m21 = (-scale.GetY()) * sina;
	m_m21 = scale.GetX() * cosa;
	m_dx = translation.GetX();
	m_dy = translation.GetY();
}


CVector2d CTransform::ApplyTo(const CVector2d& point) const
{
	double x = m_m11 * point.GetX() + m_m21 * point.GetY() + m_dx;
	double y = m_m22 * point.GetY() + m_m12 * point.GetX() + m_dy;
	
	return CVector2d(x, y);
}


CTransform& CTransform::Rotate(double angle)
{
	angle = imath::GetRadianFromDegree(angle);

	double sina = sin(angle);
    double cosa = cos(angle);

	double m11 = cosa * m_m11 + sina * m_m21;
    double m12 = cosa * m_m12 + sina * m_m22;
    double m21 = -sina * m_m11 + cosa * m_m21;
    double m22 = -sina * m_m12 + cosa * m_m22;
    	
	m_m11 = m11;
	m_m12 = m12;
	m_m21 = m21;
	m_m22 = m22;

	return *this;
}


CTransform& CTransform::Scale(const CVector2d& scale)
{	
	return this->Scale(scale.GetX(), scale.GetY());
}


CTransform& CTransform::Scale(double sx, double sy)
{
	m_m11 *= sx;
	m_m12 *= sy;
	m_m21 *= sy;
	m_m22 *= sx;

	return *this;
}


CTransform& CTransform::Translate(const CVector2d& translation)
{
	return Translate(translation.GetX(), translation.GetY());
}


CTransform& CTransform::Translate(double dx, double dy)
{
	m_dx += dx;
	m_dy += dy;

	return *this;
}



CTransform CTransform::GetRotated(double angle) const
{
	CTransform result = *this;

	return result.Rotate(angle);
}


CTransform CTransform::GetScaled(const CVector2d& scale) const
{
	return GetScaled(scale.GetX(), scale.GetY());
}


CTransform CTransform::GetScaled(double sx, double sy) const
{
	CTransform result = *this;

	return result.Scale(sx, sy);
}


CTransform CTransform::GetTranslated(const CVector2d& translation) const
{
	return GetTranslated(translation.GetX(), translation.GetY());
}


CTransform CTransform::GetTranslated(double dx, double dy) const
{
	CTransform result = *this;

	return result.Translate(dx, dy);
}


} // namespace i2d


