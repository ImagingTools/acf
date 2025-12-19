#ifndef i3d_CBox3d_included
#define i3d_CBox3d_included


// ACF includes
#include <i3d/CVector3d.h>


namespace i3d
{


/**
	Definition of an axis-aligned bounding box (AABB) in 3D space.
	Box is stored as minimum and maximum corner points.
*/
class CBox3d
{
public:
	/**
		Default constructor.
		Creates an empty box (invalid state).
	*/
	CBox3d();
	
	/**
		Construct box from minimum and maximum corners.
		\param min	Minimum corner (smallest x, y, z).
		\param max	Maximum corner (largest x, y, z).
	*/
	CBox3d(const CVector3d& min, const CVector3d& max);
	
	/**
		Returns true if the box is in invalid/empty state.
	*/
	bool IsEmpty() const;
	
	/**
		Get minimum corner of the box.
	*/
	const CVector3d& GetMin() const;
	
	/**
		Set minimum corner of the box.
	*/
	void SetMin(const CVector3d& min);
	
	/**
		Get maximum corner of the box.
	*/
	const CVector3d& GetMax() const;
	
	/**
		Set maximum corner of the box.
	*/
	void SetMax(const CVector3d& max);
	
	/**
		Get center of the box.
	*/
	CVector3d GetCenter() const;
	
	/**
		Get size (dimensions) of the box.
	*/
	CVector3d GetSize() const;
	
	/**
		Get extent (half-size) of the box.
	*/
	CVector3d GetExtent() const;
	
	/**
		Calculate volume of the box.
	*/
	double GetVolume() const;
	
	/**
		Calculate surface area of the box.
	*/
	double GetSurfaceArea() const;
	
	/**
		Check if point is inside the box.
		\param point		Point to check.
		\param tolerance	Tolerance for boundary check.
	*/
	bool Contains(const CVector3d& point, double tolerance = 0.0) const;
	
	/**
		Check if this box intersects another box.
	*/
	bool Intersects(const CBox3d& other) const;
	
	/**
		Expand box to include point.
	*/
	void Include(const CVector3d& point);
	
	/**
		Expand box to include another box.
	*/
	void Include(const CBox3d& other);
	
	/**
		Get closest point on or inside box to given point.
	*/
	CVector3d GetClosestPoint(const CVector3d& point) const;
	
	/**
		Reset box to empty state.
	*/
	void Reset();
	
	/**
		Serialize this box to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);
	
	bool operator==(const CBox3d& box) const;
	bool operator!=(const CBox3d& box) const;

private:
	CVector3d m_min;		///< Minimum corner
	CVector3d m_max;		///< Maximum corner
};


// inline methods

inline CBox3d::CBox3d()
{
	Reset();
}


inline CBox3d::CBox3d(const CVector3d& min, const CVector3d& max)
:	m_min(min)
,	m_max(max)
{
}


inline bool CBox3d::IsEmpty() const
{
	return m_min.GetX() > m_max.GetX() || 
	       m_min.GetY() > m_max.GetY() || 
	       m_min.GetZ() > m_max.GetZ();
}


inline const CVector3d& CBox3d::GetMin() const
{
	return m_min;
}


inline void CBox3d::SetMin(const CVector3d& min)
{
	m_min = min;
}


inline const CVector3d& CBox3d::GetMax() const
{
	return m_max;
}


inline void CBox3d::SetMax(const CVector3d& max)
{
	m_max = max;
}


inline CVector3d CBox3d::GetCenter() const
{
	return (m_min + m_max) * 0.5;
}


inline CVector3d CBox3d::GetSize() const
{
	if (IsEmpty()){
		return CVector3d(0.0, 0.0, 0.0);
	}
	return m_max - m_min;
}


inline CVector3d CBox3d::GetExtent() const
{
	return GetSize() * 0.5;
}


inline double CBox3d::GetVolume() const
{
	if (IsEmpty()){
		return 0.0;
	}
	CVector3d size = GetSize();
	return size.GetX() * size.GetY() * size.GetZ();
}


inline double CBox3d::GetSurfaceArea() const
{
	if (IsEmpty()){
		return 0.0;
	}
	CVector3d size = GetSize();
	return 2.0 * (size.GetX() * size.GetY() + 
	              size.GetX() * size.GetZ() + 
	              size.GetY() * size.GetZ());
}


inline bool CBox3d::Contains(const CVector3d& point, double tolerance) const
{
	if (IsEmpty()){
		return false;
	}
	
	return point.GetX() >= m_min.GetX() - tolerance &&
	       point.GetX() <= m_max.GetX() + tolerance &&
	       point.GetY() >= m_min.GetY() - tolerance &&
	       point.GetY() <= m_max.GetY() + tolerance &&
	       point.GetZ() >= m_min.GetZ() - tolerance &&
	       point.GetZ() <= m_max.GetZ() + tolerance;
}


inline bool CBox3d::Intersects(const CBox3d& other) const
{
	if (IsEmpty() || other.IsEmpty()){
		return false;
	}
	
	return m_min.GetX() <= other.m_max.GetX() &&
	       m_max.GetX() >= other.m_min.GetX() &&
	       m_min.GetY() <= other.m_max.GetY() &&
	       m_max.GetY() >= other.m_min.GetY() &&
	       m_min.GetZ() <= other.m_max.GetZ() &&
	       m_max.GetZ() >= other.m_min.GetZ();
}


inline void CBox3d::Include(const CVector3d& point)
{
	if (IsEmpty()){
		m_min = point;
		m_max = point;
	}
	else{
		m_min.SetX(qMin(m_min.GetX(), point.GetX()));
		m_min.SetY(qMin(m_min.GetY(), point.GetY()));
		m_min.SetZ(qMin(m_min.GetZ(), point.GetZ()));
		
		m_max.SetX(qMax(m_max.GetX(), point.GetX()));
		m_max.SetY(qMax(m_max.GetY(), point.GetY()));
		m_max.SetZ(qMax(m_max.GetZ(), point.GetZ()));
	}
}


inline void CBox3d::Include(const CBox3d& other)
{
	if (!other.IsEmpty()){
		Include(other.m_min);
		Include(other.m_max);
	}
}


inline void CBox3d::Reset()
{
	// Set to invalid state (min > max)
	m_min = CVector3d(qInf(), qInf(), qInf());
	m_max = CVector3d(-qInf(), -qInf(), -qInf());
}


inline bool CBox3d::operator==(const CBox3d& box) const
{
	return m_min == box.m_min && m_max == box.m_max;
}


inline bool CBox3d::operator!=(const CBox3d& box) const
{
	return !(*this == box);
}


} // namespace i3d


#endif // !i3d_CBox3d_included
