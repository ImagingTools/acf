#ifndef i3d_CLine3d_included
#define i3d_CLine3d_included


// ACF includes
#include <i3d/CVector3d.h>


namespace i3d
{


/**
	Definition of a line in 3D space.
	Line is stored as two points or equivalently as point and direction.
*/
class CLine3d
{
public:
	/**
		Default constructor.
		Creates a line along X axis through origin.
	*/
	CLine3d();
	
	/**
		Construct line from two points.
		\param p1	First point on line.
		\param p2	Second point on line.
	*/
	CLine3d(const CVector3d& p1, const CVector3d& p2);
	
	/**
		Returns true if the line has zero length.
	*/
	bool IsNull() const;
	
	/**
		Get first point of the line.
	*/
	const CVector3d& GetPoint1() const;
	
	/**
		Set first point of the line.
	*/
	void SetPoint1(const CVector3d& point);
	
	/**
		Get second point of the line.
	*/
	const CVector3d& GetPoint2() const;
	
	/**
		Set second point of the line.
	*/
	void SetPoint2(const CVector3d& point);
	
	/**
		Get direction vector of the line (normalized).
	*/
	CVector3d GetDirection() const;
	
	/**
		Get length of the line segment.
	*/
	double GetLength() const;
	
	/**
		Get squared length of the line segment (faster than GetLength).
	*/
	double GetLengthSq() const;
	
	/**
		Get point at parameter t.
		t=0 gives point1, t=1 gives point2.
	*/
	CVector3d GetPointAt(double t) const;
	
	/**
		Get closest point on line to given point.
		\param point	Point to find closest point to.
		\return			Closest point on the line segment.
	*/
	CVector3d GetClosestPoint(const CVector3d& point) const;
	
	/**
		Get parameter value t for closest point on line to given point.
		\param point	Point to find closest point to.
		\return			Parameter t (0 to 1 for segment, can be outside for infinite line).
	*/
	double GetClosestParameter(const CVector3d& point) const;
	
	/**
		Calculate distance from point to line segment.
	*/
	double GetDistance(const CVector3d& point) const;
	
	/**
		Calculate squared distance from point to line segment (faster).
	*/
	double GetDistanceSq(const CVector3d& point) const;
	
	/**
		Serialize this line to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);
	
	bool operator==(const CLine3d& line) const;
	bool operator!=(const CLine3d& line) const;

private:
	CVector3d m_point1;		///< First point
	CVector3d m_point2;		///< Second point
};


// inline methods

inline CLine3d::CLine3d()
:	m_point1(0.0, 0.0, 0.0)
,	m_point2(1.0, 0.0, 0.0)
{
}


inline CLine3d::CLine3d(const CVector3d& p1, const CVector3d& p2)
:	m_point1(p1)
,	m_point2(p2)
{
}


inline bool CLine3d::IsNull() const
{
	return m_point1 == m_point2;
}


inline const CVector3d& CLine3d::GetPoint1() const
{
	return m_point1;
}


inline void CLine3d::SetPoint1(const CVector3d& point)
{
	m_point1 = point;
}


inline const CVector3d& CLine3d::GetPoint2() const
{
	return m_point2;
}


inline void CLine3d::SetPoint2(const CVector3d& point)
{
	m_point2 = point;
}


inline CVector3d CLine3d::GetDirection() const
{
	return (m_point2 - m_point1).GetNormalized();
}


inline double CLine3d::GetLength() const
{
	return (m_point2 - m_point1).GetLength();
}


inline double CLine3d::GetLengthSq() const
{
	return (m_point2 - m_point1).GetLength2();
}


inline CVector3d CLine3d::GetPointAt(double t) const
{
	return m_point1 + (m_point2 - m_point1) * t;
}


inline bool CLine3d::operator==(const CLine3d& line) const
{
	return m_point1 == line.m_point1 && m_point2 == line.m_point2;
}


inline bool CLine3d::operator!=(const CLine3d& line) const
{
	return !(*this == line);
}


} // namespace i3d


#endif // !i3d_CLine3d_included
