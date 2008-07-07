#ifndef i2d_CCircle_included
#define	i2d_CCircle_included


#include "i2d/IObject2d.h"

#include "i2d/CVector2d.h"


namespace i2d
{	


class CCircle: virtual public IObject2d
{
public:
	CCircle();
	CCircle(double radius, const CVector2d& center);

	double GetRadius() const;

	// reimplemented (IObject2d)
	virtual CVector2d GetCenter() const;
	virtual CRectangle GetBoundingBox() const;

protected:
	double m_radius;
	CVector2d m_center;
};


} // namespace i2d


#endif // !i2d_CCircle_included

