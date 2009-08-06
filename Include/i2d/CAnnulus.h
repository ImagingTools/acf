#ifndef i2d_CAnnulus_included
#define i2d_CAnnulus_included


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
	CAnnulus(const i2d::CVector2d& center, double innerRadius, double outerRadius);

	bool IsAnnulusValid() const;

	double GetInnerRadius() const;
	void SetInnerRadius(double innerRadius);

	double GetOuterRadius() const;
	void SetOuterRadius(double outerRadius);

	CCircle GetInnerCircle() const;
	CCircle GetOuterCircle() const;

	virtual CRectangle GetBoundingBox() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	istd::CRange m_radiusRange;
};


} // namespace i2d


#endif // !i2d_CAnnulus_included


