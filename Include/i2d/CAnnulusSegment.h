#ifndef i2d_CAnnulusSegment_included
#define i2d_CAnnulusSegment_included


#include "i2d/CAnnulus.h"


namespace i2d
{	


class CAnnulusSegment: public CAnnulus
{
public:
	typedef CAnnulus BaseClass;

	CAnnulusSegment();
	CAnnulusSegment(
				const i2d::CVector2d& center,
				double innerRadius,
				double outerRadius,
				double beginAngle,
				double endAngle);

	double GetBeginAngle() const;
	void SetBeginAngle(double angle);
	double GetEndAngle() const;
	void SetEndAngle(double angle);

	/**
		Get bounding box of the annulus segment object.
	*/
	virtual CRectangle GetBoundingBox() const;

	/**
		Return \c true if a given point is inside of annulus segment area.
	*/
	bool Contains(const i2d::CVector2d& point) const; 

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CRange m_angleRange;
};


} // namespace i2d


#endif // !i2d_CAnnulusSegment_included


