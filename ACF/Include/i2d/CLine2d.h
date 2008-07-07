#ifndef i2d_CLine2d_included
#define	i2d_CLine2d_included


#include "i2d/CVector2d.h"

#include "i2d/IObject2d.h"


namespace i2d
{	


class CLine2d: virtual public IObject2d
{
public:
	CLine2d();
	CLine2d(const CVector2d& p1, const CVector2d& p2);
	CLine2d(double x1, double y1, double x2, double y2);
	CLine2d(const CLine2d& line);
	CLine2d operator=(const CLine2d& inl);

	const CVector2d& GetPoint1() const;
	const CVector2d& GetPoint2() const;
	double GetSlope() const;
	double GetIntercept() const;
	bool IsParalell(const CLine2d& inLine) const;
	bool IsIntersectedBy(const CLine2d& inLine, CVector2d* intersectionPtr = NULL) const;
	CVector2d GetIntersection(const CLine2d& inLine) const;

	// reimplemented (IObject2d)
	virtual CVector2d GetCenter() const;
	virtual CRectangle GetBoundingBox() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	CVector2d m_point1;
	CVector2d m_point2;		
};
	

} // namespace i2d


#endif // i2d_CLine2d_included
