#ifndef i2d_CTransform_included
#define	i2d_CTransform_included


#include "istd/IPolymorphic.h"

#include "i2d/CVector2d.h"


namespace i2d
{	


class CTransform: virtual public istd::IPolymorphic
{
public:
	CTransform();
	CTransform(const CVector2d& translation, const CVector2d& scale, double angle);

	CVector2d ApplyTo(const CVector2d& point) const;

	CTransform& Rotate(double angle);
	CTransform& Scale(const CVector2d& scale);
	CTransform& Scale(double sx, double sy);
	CTransform& Translate(const CVector2d& translation);
	CTransform& Translate(double dx, double dy);

	CTransform GetRotated(double angle) const;
	CTransform GetScaled(const CVector2d& scale) const;
	CTransform GetScaled(double sx, double sy) const;
	CTransform GetTranslated(const CVector2d& translation) const;
	CTransform GetTranslated(double dx, double dy) const;

private:
	double m_m11;
	double m_m12;
	double m_m21;
	double m_m22;
	double m_dx;
	double m_dy;
};


} // namespace i2d


#endif // i2d_CTransform_included

