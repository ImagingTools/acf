#ifndef i2d_CLine2d_included
#define i2d_CLine2d_included


#include "i2d/CVector2d.h"

#include "i2d/IObject2d.h"


namespace i2d
{	


class CRectangle;


/**
	Definition of a line in 2D-space.
	Line is stored as the ordered set of two points.
*/
class CLine2d: virtual public IObject2d
{
public:
	CLine2d();
	CLine2d(const CVector2d& p1, const CVector2d& p2);
	CLine2d(double x1, double y1, double x2, double y2);
	CLine2d(const CLine2d& line);

	CLine2d operator = (const CLine2d& line);
	bool operator == (const CLine2d& line) const;
	bool operator != (const CLine2d& line) const;

	/**
		Returns \c true, if the line has a length equal 0.
	*/
	bool IsNull() const;

	/**
		Get value of first line point.
	*/
	const CVector2d& GetPoint1() const;

	/**
		Get reference to first line point.
		Please avoid to use this function, becouse of eventuel update problems.
	*/
	CVector2d& GetPoint1Ref();

	/**
		Set value of first line point.
	*/
	void SetPoint1(const CVector2d& point);

	/**
		Get value of second line point.
	*/
	const CVector2d& GetPoint2() const;

	/**
		Get reference to second line point.
		Please avoid to use this function, becouse of eventuel update problems.
	*/
	CVector2d& GetPoint2Ref();

	/**
		Set value of second line point.
	*/
	void SetPoint2(const CVector2d& point);

	/**
		Get slope of this line.
		Slope is derivative value along X axis (dx/dy) of line function.
	*/
	double GetSlope() const;

	/**
		Get intercept of this line.
		Intercept is derivative value along Y axis (dy/dx) of line function.
	*/
	double GetIntercept() const;

	/**
		Check if this line is parallel to another one.
	*/
	bool IsParalell(const CLine2d& line) const;

	/**
		Check if two lines intersect.
		\param	line	second line.
		\return	true, if this line has common point.
	*/
	bool IsIntersectedBy(const CLine2d& line) const;

	/**
		Get intersecition position of two lines.
		\param	line	second line.
		\param	result	object will be set to intersection point.
		\return	true if lines intersects and intersection could be calculated.
	*/
	bool GetIntersection(const CLine2d& line, CVector2d& result) const;

	/**
		Get intersecition position of extended lines.
		As extended line are mean mathematical lines going over specified line points.
		\param	line	second line.
		\param	result	object will be set to intersection point.
		\return	true if extended intersection could be calculated.
	*/
	bool GetExtendedIntersection(const CLine2d& line, CVector2d& result) const;

	/**
		Get distance from nearest point of this line to specified position.
	*/
	double GetDistance(const CVector2d& position) const;

	/**
		Get distance from nearest point of extended line to specified position.
		As extended line are mean mathematical lines going over specified line points.
	*/
	double GetExtendedDistance(const CVector2d& position) const;

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
		Get distance between line points.
	*/
	double GetLength() const;

	/**
		Get part of line intersecting specified rectangle.
	*/
	CLine2d GetClipped(const CRectangle& rect) const;

	/**
		Get the line with replaced points (p1 = p2 and p2 = p1)
	*/
	CLine2d GetSwapped() const;

	/**
		Get Y position of cutting this line by specified horizontal line.
	*/
	double GetCutXPos(double linePosY) const;
	/**
		Get X position of cutting this line by specified vertical line.
	*/
	double GetCutYPos(double linePosX) const;

	virtual CRectangle GetBoundingBox() const;

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	CVector2d m_point1;
	CVector2d m_point2;		
};
	

// inline methods

inline bool CLine2d::operator == (const CLine2d& line) const
{
	return ((m_point1 == line.m_point1) && (m_point2 == line.m_point2));
}


inline bool CLine2d::operator != (const CLine2d& line) const
{
	return !operator == (line);
}


inline bool CLine2d::IsNull() const
{
	return (m_point1 == m_point2);
}


inline CVector2d CLine2d::GetDiffVector() const
{
	return m_point2 - m_point1;
}


inline const CVector2d& CLine2d::GetPoint1() const
{
	return m_point1;
}


inline CVector2d& CLine2d::GetPoint1Ref()
{
	return m_point1;
}


inline const CVector2d& CLine2d::GetPoint2() const
{
	return m_point2;
}


inline CVector2d& CLine2d::GetPoint2Ref()
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
