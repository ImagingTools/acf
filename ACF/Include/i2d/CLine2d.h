#ifndef i2d_CLine2d_included
#define	i2d_CLine2d_included


#include "i2d/CVector2d.h"

#include "i2d/IObject2d.h"


namespace i2d
{	


class CRectangle;


class CLine2d: virtual public IObject2d
{
public:
	CLine2d();
	CLine2d(const CVector2d& p1, const CVector2d& p2);
	CLine2d(double x1, double y1, double x2, double y2);
	CLine2d(const CLine2d& line);
	CLine2d operator=(const CLine2d& inl);

	const CVector2d& GetPoint1() const;
	void SetPoint1(const CVector2d& point);
	const CVector2d& GetPoint2() const;
	void SetPoint2(const CVector2d& point);
	double GetSlope() const;
	double GetIntercept() const;

	/**
		Check if this line is parallel to another one.
	*/
	bool IsParalell(const CLine2d& inLine) const;

	/**
		Check if two lines has intersection.
		\param	line			second line.
		\param	intersectionPtr	optional object will be set to intersection pointer.
	*/
	bool IsIntersectedBy(const CLine2d& line, CVector2d* intersectionPtr = NULL) const;

	/**
		Get intersecition position of two lines.
	*/
	CVector2d GetIntersection(const CLine2d& inLine) const;

	/**
		Get position of point using normed 'alpha' value.
		Value 0 will be mapped to point 1 and value 1 wil be mapped to point 2.
	*/
	CVector2d GetPositionFromAlpha(double alpha) const;

	/**
		Get difference vector from point 1 to point 2.
	*/
	CVector2d GetDiffVector() const;

	/**
		Get part of line intersecting specified rectangle.
	*/
	CLine2d GetClipped(const CRectangle& rect) const;

	/**
		Get Y position of cutting this line by specified horizontal line.
	*/
	double GetCutXPos(double linePosY) const;
	/**
		Get X position of cutting this line by specified vertical line.
	*/
	double GetCutYPos(double linePosX) const;

	// reimplemented (IObject2d)
	virtual CVector2d GetCenter() const;
	virtual CRectangle GetBoundingBox() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	CVector2d m_point1;
	CVector2d m_point2;		
};
	

// inline methods

inline CVector2d CLine2d::GetDiffVector() const
{
	return m_point2 - m_point1;
}


inline const CVector2d& CLine2d::GetPoint1() const
{
	return m_point1;
}


inline const CVector2d& CLine2d::GetPoint2() const
{
	return m_point2;
}


inline double CLine2d::GetCutXPos(double linePosY) const
{
	CVector2d diff = GetDiffVector();

	return (linePosY - m_point1.GetY()) * diff.GetX() / diff.GetY() + m_point1.GetX();
}



inline double CLine2d::GetCutYPos(double linePosX) const
{
	CVector2d diff = GetDiffVector();

	return (linePosX - m_point1.GetX()) * diff.GetY() / diff.GetX() + m_point1.GetY();
}



} // namespace i2d


#endif // !i2d_CLine2d_included
