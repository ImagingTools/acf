#ifndef i2d_CCircle_included
#define i2d_CCircle_included


#include "i2d/CPosition2d.h"


namespace i2d
{	


class CCircle: public CPosition2d
{
public:
	typedef CPosition2d BaseClass;

	CCircle();
	CCircle(double radius, const CVector2d& center);

	double GetRadius() const;
	void SetRadius(double radius);

	virtual CRectangle GetBoundingBox() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	double m_radius;
};


// inline methods

inline double CCircle::GetRadius() const
{
	return m_radius;
}


} // namespace i2d


#endif // !i2d_CCircle_included


