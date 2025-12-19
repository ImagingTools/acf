#ifndef i3d_CSphere_included
#define i3d_CSphere_included


// ACF includes
#include <i3d/CVector3d.h>


namespace i3d
{


class CLine3d;


/**
	Definition of a sphere in 3D space.
	Sphere is stored as center position and radius.
*/
class CSphere
{
public:
	/**
		Default constructor.
		Creates a unit sphere at origin.
	*/
	CSphere();
	
	/**
		Construct sphere from radius and center.
		\param radius	Radius of the sphere.
		\param center	Center position.
	*/
	CSphere(double radius, const CVector3d& center);
	
	/**
		Get radius of the sphere.
	*/
	double GetRadius() const;
	
	/**
		Set radius of the sphere.
	*/
	void SetRadius(double radius);
	
	/**
		Get center of the sphere.
	*/
	const CVector3d& GetCenter() const;
	
	/**
		Set center of the sphere.
	*/
	void SetCenter(const CVector3d& center);
	
	/**
		Calculate volume of the sphere.
	*/
	double GetVolume() const;
	
	/**
		Calculate surface area of the sphere.
	*/
	double GetSurfaceArea() const;
	
	/**
		Check if point is inside the sphere.
		\param point		Point to check.
		\param tolerance	Tolerance for boundary check.
	*/
	bool Contains(const CVector3d& point, double tolerance = 0.0) const;
	
	/**
		Check if this sphere intersects another sphere.
	*/
	bool Intersects(const CSphere& other) const;
	
	/**
		Check if this sphere intersects a line segment.
	*/
	bool Intersects(const CLine3d& line) const;
	
	/**
		Get distance from sphere surface to point.
		Positive if outside, negative if inside, zero if on surface.
	*/
	double GetSignedDistance(const CVector3d& point) const;
	
	/**
		Get closest point on sphere surface to given point.
	*/
	CVector3d GetClosestPoint(const CVector3d& point) const;
	
	/**
		Serialize this sphere to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);
	
	bool operator==(const CSphere& sphere) const;
	bool operator!=(const CSphere& sphere) const;

private:
	CVector3d m_center;		///< Center position
	double m_radius;		///< Radius
};


// inline methods

inline CSphere::CSphere()
:	m_center(0.0, 0.0, 0.0)
,	m_radius(1.0)
{
}


inline CSphere::CSphere(double radius, const CVector3d& center)
:	m_center(center)
,	m_radius(radius)
{
}


inline double CSphere::GetRadius() const
{
	return m_radius;
}


inline void CSphere::SetRadius(double radius)
{
	m_radius = radius;
}


inline const CVector3d& CSphere::GetCenter() const
{
	return m_center;
}


inline void CSphere::SetCenter(const CVector3d& center)
{
	m_center = center;
}


inline double CSphere::GetVolume() const
{
	return (4.0 / 3.0) * I_PI * m_radius * m_radius * m_radius;
}


inline double CSphere::GetSurfaceArea() const
{
	return 4.0 * I_PI * m_radius * m_radius;
}


inline bool CSphere::Contains(const CVector3d& point, double tolerance) const
{
	double distSq = (point - m_center).GetLengthSq();
	double radiusTol = m_radius + tolerance;
	return distSq <= radiusTol * radiusTol;
}


inline bool CSphere::Intersects(const CSphere& other) const
{
	double centerDist = (m_center - other.m_center).GetLength();
	return centerDist <= (m_radius + other.m_radius);
}


inline double CSphere::GetSignedDistance(const CVector3d& point) const
{
	return (point - m_center).GetLength() - m_radius;
}


inline CVector3d CSphere::GetClosestPoint(const CVector3d& point) const
{
	CVector3d direction = (point - m_center).GetNormalized();
	return m_center + direction * m_radius;
}


inline bool CSphere::operator==(const CSphere& sphere) const
{
	return m_center == sphere.m_center && qAbs(m_radius - sphere.m_radius) < I_BIG_EPSILON;
}


inline bool CSphere::operator!=(const CSphere& sphere) const
{
	return !(*this == sphere);
}


} // namespace i3d


#endif // !i3d_CSphere_included
