#ifndef i2d_CLine2d_included
#define i2d_CLine2d_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "i2d/CObject2dBase.h"
#include "i2d/CVector2d.h"


namespace i2d
{


class CRectangle;


/**
	Definition of a line in 2D-space.
	Line is stored as the ordered set of two points.
*/
class CLine2d: public CObject2dBase
{
public:
	CLine2d();
	CLine2d(const CVector2d& p1, const CVector2d& p2);
	CLine2d(double x1, double y1, double x2, double y2);

	bool operator==(const CLine2d& line) const;
	bool operator!=(const CLine2d& line) const;

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
		Please avoid to use this function because of possible update problems.
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
		Please avoid to use this function because of possible update problems.
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
	bool IsParallel(const CLine2d& line) const;

	/**
		Check if two lines intersect.
		\param	line	second line.
		\return	true, if this line has common point.
	*/
	bool IsIntersectedBy(const CLine2d& line) const;

	/**
		Get intersection position of two lines.
		\param	line	second line.
		\param	result	object will be set to intersection point.
		\return	true if lines intersects and intersection could be calculated.
	*/
	bool GetIntersection(const CLine2d& line, CVector2d& result) const;

	/**
		Get intersection position of extended lines.
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
		Value 0 will be mapped to point 1 and value 1 will be mapped to point 2.
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
		Get quadratic distance between line points.
	*/
	double GetLength2() const;

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

	/**	
		Copy begin point to end point and set new begin point.
	*/
	void PushBeginPoint(const i2d::CVector2d& newBeginPoint);

	/**	
		Copy end point to begin point and set new end point.
	*/
	void PushEndPoint(const i2d::CVector2d& newEndPoint);

	/**
		Get a proportion of lines cut point to line length (called also 'alpha value').
	*/
	double GetCutAlpha(const CLine2d& line) const;

	double GetCastAlpha(const i2d::CVector2d& point) const;

	bool CutDisk(const i2d::CVector2d& center, double radius) const;

	/**
		Get projection position 'alpha value' and orthogonal distance to line.
		\return first value equals result of \c GetCastAlpha, the second is \c GetExtendedDistance,
				but it can be negative for positions on the left size.
	*/
	QPair<double, double> GetAlphaAndCastDistance(const i2d::CVector2d& point) const;

	/**
		Get position where this line intersects the second one.
		\return	true, if this lines intersect and intersection position can be calculated.
	*/
	bool GetCutPoint(const CLine2d& otherLine, i2d::CVector2d& cutPoint) const;

	/**
		Return a line point, which is the nearest to the specified point.
	*/
	i2d::CVector2d GetNearestPoint(const i2d::CVector2d& point) const;

	/**
		Return a point on the straight line in a mathematical sense,
		which is the nearest to the specified point.
	*/
	i2d::CVector2d GetExtendedNearestPoint(const i2d::CVector2d& point) const;

	/**
		Return a line, which connects the nearest end points of two lines.
	*/
	CLine2d GetShortestEndConnection(const CLine2d& line) const;
	
	/**	
		Return a line, which connects the nearest points of two lines.
		Only connection between end point of first line and second line
		or connection between first line and begin point of second line will be checked.
	*/
	CLine2d GetShortestConnectionToNext(const CLine2d& line) const;
	
	/**
		Return a line, which connects the nearest point of line and specified point.
	*/
	CLine2d GetShortestConnection(const i2d::CVector2d& point) const;
	
	/**
		Return a line, which connects the nearest points of two lines.
	*/
	CLine2d GetShortestConnection(const CLine2d& line) const;

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
	virtual CRectangle GetBoundingBox() const;
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

	//reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object);
	virtual istd::IChangeable* CloneMe() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	CVector2d m_point1;
	CVector2d m_point2;		
};


// public inline methods

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


inline double CLine2d::GetCutAlpha(const CLine2d& line) const
{
	const i2d::CVector2d& delta = GetDiffVector();
	const i2d::CVector2d& lineDelta = line.GetDiffVector();
	const i2d::CVector2d& beginPoint = GetPoint1();
	const i2d::CVector2d& lineBeginPoint = line.GetPoint1();

	double crossProductZ = lineDelta.GetCrossProductZ(delta);

	return (beginPoint.GetCrossProductZ(delta) + delta.GetCrossProductZ(lineBeginPoint)) / crossProductZ;
}


inline double CLine2d::GetCastAlpha(const i2d::CVector2d& point) const
{
	i2d::CVector2d delta = GetDiffVector();
	i2d::CVector2d deltaToPoint = point - m_point1;

	double dotProduct = delta.GetDotProduct(deltaToPoint);
	return dotProduct / delta.GetLength2();
}


inline bool CLine2d::CutDisk(const i2d::CVector2d& center, double radius) const
{
	const i2d::CVector2d& delta = GetDiffVector();
	const i2d::CVector2d& deltaToCenter = m_point1 - center;

	double a = delta.GetLength2();
	if (a < I_BIG_EPSILON * I_BIG_EPSILON){
		return (m_point1.GetDistance2(center) <= radius * radius);
	}
	double b = delta.GetDotProduct(deltaToCenter);
	double c = deltaToCenter.GetLength2() - radius * radius;

	double D = b * b - a * c;
	if (D >= 0){
		double sqrtD = ::sqrt(D);

		double alpha1 = (-b - sqrtD) / a;
		double alpha2 = (-b + sqrtD) / a;
		if ((alpha1 < 1) && (alpha2 > 0)){
			return true;
		}
	}
	return false;
}


inline QPair<double, double> CLine2d::GetAlphaAndCastDistance(const i2d::CVector2d& point) const
{
	i2d::CVector2d delta = m_point2 - m_point1;
	i2d::CVector2d deltaToPoint = point - m_point1;

	double dotProduct = delta.GetDotProduct(deltaToPoint);
	double field = delta.GetCrossProductZ(deltaToPoint);
	double deltaLength2 = delta.GetLength2();

	return QPair<double, double>(dotProduct / deltaLength2, field / ::sqrt(deltaLength2));
}



} // namespace i2d


#endif // !i2d_CLine2d_included


