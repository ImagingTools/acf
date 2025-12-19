#ifndef i3d_CPlane3d_included
#define i3d_CPlane3d_included


// ACF includes
#include <i3d/CVector3d.h>


namespace i3d
{


/**
	Definition of a plane in 3D space.
	Plane is stored as a point on the plane and a normal vector.
	The plane equation is: dot(normal, (p - point)) = 0
*/
class CPlane3d
{
public:
	/**
		Default constructor.
		Creates a plane at origin with normal pointing along Z axis.
	*/
	CPlane3d();
	
	/**
		Construct plane from point and normal vector.
		\param point	A point on the plane.
		\param normal	Normal vector (will be normalized internally).
	*/
	CPlane3d(const CVector3d& point, const CVector3d& normal);
	
	/**
		Construct plane from three points.
		Points should not be collinear.
		\param p1	First point on plane.
		\param p2	Second point on plane.
		\param p3	Third point on plane.
	*/
	CPlane3d(const CVector3d& p1, const CVector3d& p2, const CVector3d& p3);
	
	/**
		Get point on the plane.
	*/
	const CVector3d& GetPoint() const;
	
	/**
		Set point on the plane.
	*/
	void SetPoint(const CVector3d& point);
	
	/**
		Get normal vector of the plane.
	*/
	const CVector3d& GetNormal() const;
	
	/**
		Set normal vector of the plane.
		The normal will be normalized internally.
	*/
	void SetNormal(const CVector3d& normal);
	
	/**
		Calculate signed distance from point to plane.
		Positive if point is on the side of normal, negative otherwise.
	*/
	double GetSignedDistance(const CVector3d& point) const;
	
	/**
		Calculate absolute distance from point to plane.
	*/
	double GetDistance(const CVector3d& point) const;
	
	/**
		Project point onto the plane.
	*/
	CVector3d GetProjection(const CVector3d& point) const;
	
	/**
		Check if point lies on the plane (within tolerance).
	*/
	bool Contains(const CVector3d& point, double tolerance = I_BIG_EPSILON) const;
	
	/**
		Serialize this plane to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);
	
	bool operator==(const CPlane3d& plane) const;
	bool operator!=(const CPlane3d& plane) const;

private:
	CVector3d m_point;		///< Point on the plane
	CVector3d m_normal;		///< Normal vector (unit length)
};


// inline methods

inline CPlane3d::CPlane3d()
:	m_point(0.0, 0.0, 0.0)
,	m_normal(0.0, 0.0, 1.0)
{
}


inline CPlane3d::CPlane3d(const CVector3d& point, const CVector3d& normal)
:	m_point(point)
,	m_normal(normal.GetNormalized())
{
}


inline const CVector3d& CPlane3d::GetPoint() const
{
	return m_point;
}


inline void CPlane3d::SetPoint(const CVector3d& point)
{
	m_point = point;
}


inline const CVector3d& CPlane3d::GetNormal() const
{
	return m_normal;
}


inline void CPlane3d::SetNormal(const CVector3d& normal)
{
	m_normal = normal.GetNormalized();
}


inline double CPlane3d::GetSignedDistance(const CVector3d& point) const
{
	return m_normal.GetDotProduct(point - m_point);
}


inline double CPlane3d::GetDistance(const CVector3d& point) const
{
	return qAbs(GetSignedDistance(point));
}


inline CVector3d CPlane3d::GetProjection(const CVector3d& point) const
{
	double distance = GetSignedDistance(point);
	return point - m_normal * distance;
}


inline bool CPlane3d::Contains(const CVector3d& point, double tolerance) const
{
	return GetDistance(point) < tolerance;
}


inline bool CPlane3d::operator==(const CPlane3d& plane) const
{
	return m_point == plane.m_point && m_normal == plane.m_normal;
}


inline bool CPlane3d::operator!=(const CPlane3d& plane) const
{
	return !(*this == plane);
}


} // namespace i3d


#endif // !i3d_CPlane3d_included
