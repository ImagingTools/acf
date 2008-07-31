#ifndef i2d_CAnnulus_included
#define	i2d_CAnnulus_included


#include "i2d/CPosition2d.h"
#include "i2d/CCircle.h"
#include "i2d/CRectangle.h"


namespace i2d
{	


class CAnnulus: public i2d::CPosition2d
{
public:
	typedef i2d::CPosition2d BaseClass;

	CAnnulus();
	CAnnulus(double innerRadius, double outerRadius, const i2d::CVector2d& center);

	bool IsValid() const;

	double GetInnerRadius() const;
	double GetOuterRadius() const;

	CCircle GetInnerCircle() const;
	CCircle GetOuterCircle() const;

	// reimplemented (i2d::IObject2d)
	virtual CRectangle GetBoundingBox() const;

protected:
	double m_innerRadius;
	double m_outerRadius;
};


} // namespace i2d


#endif // !i2d_CAnnulus_included


