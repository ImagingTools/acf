#ifndef i3d_CQuaternion3d_included
#define i3d_CQuaternion3d_included


// ACF includes
#include <i3d/CVector3d.h>


namespace i3d
{


class CMatrix3d;


/**
	Quaternion for representing rotations in 3D space.
	Stored as (w, x, y, z) where w is the scalar part.
*/
class CQuaternion3d
{
public:
	/**
		Default constructor.
		Creates identity quaternion (no rotation).
	*/
	CQuaternion3d();
	
	/**
		Construct quaternion from components.
		\param w	Scalar part.
		\param x	X component of vector part.
		\param y	Y component of vector part.
		\param z	Z component of vector part.
	*/
	CQuaternion3d(double w, double x, double y, double z);
	
	/**
		Construct quaternion from axis and angle.
		\param axis		Rotation axis (will be normalized).
		\param angle	Rotation angle in radians.
	*/
	static CQuaternion3d FromAxisAngle(const CVector3d& axis, double angle);
	
	/**
		Construct quaternion from rotation matrix.
	*/
	static CQuaternion3d FromMatrix(const CMatrix3d& matrix);
	
	/**
		Construct quaternion from Euler angles (XYZ order).
		\param roll		Rotation around X axis in radians.
		\param pitch	Rotation around Y axis in radians.
		\param yaw		Rotation around Z axis in radians.
	*/
	static CQuaternion3d FromEulerAngles(double roll, double pitch, double yaw);
	
	/**
		Get W component (scalar part).
	*/
	double GetW() const;
	
	/**
		Set W component.
	*/
	void SetW(double w);
	
	/**
		Get X component.
	*/
	double GetX() const;
	
	/**
		Set X component.
	*/
	void SetX(double x);
	
	/**
		Get Y component.
	*/
	double GetY() const;
	
	/**
		Set Y component.
	*/
	void SetY(double y);
	
	/**
		Get Z component.
	*/
	double GetZ() const;
	
	/**
		Set Z component.
	*/
	void SetZ(double z);
	
	/**
		Get vector part of quaternion.
	*/
	CVector3d GetVectorPart() const;
	
	/**
		Calculate length (norm) of quaternion.
	*/
	double GetLength() const;
	
	/**
		Calculate squared length of quaternion.
	*/
	double GetLengthSq() const;
	
	/**
		Get normalized quaternion (unit quaternion).
	*/
	CQuaternion3d GetNormalized() const;
	
	/**
		Normalize this quaternion in place.
	*/
	void Normalize();
	
	/**
		Get conjugate of quaternion.
	*/
	CQuaternion3d GetConjugate() const;
	
	/**
		Get inverse of quaternion.
	*/
	CQuaternion3d GetInverse() const;
	
	/**
		Convert quaternion to rotation matrix.
	*/
	CMatrix3d ToMatrix() const;
	
	/**
		Convert quaternion to axis and angle.
		\param axis		Output rotation axis.
		\param angle	Output rotation angle in radians.
	*/
	void ToAxisAngle(CVector3d& axis, double& angle) const;
	
	/**
		Rotate vector by this quaternion.
	*/
	CVector3d Rotate(const CVector3d& vector) const;
	
	/**
		Calculate dot product with another quaternion.
	*/
	double GetDotProduct(const CQuaternion3d& other) const;
	
	/**
		Spherical linear interpolation (SLERP) between two quaternions.
		\param q1	Start quaternion.
		\param q2	End quaternion.
		\param t	Interpolation parameter (0 to 1).
	*/
	static CQuaternion3d Slerp(const CQuaternion3d& q1, const CQuaternion3d& q2, double t);
	
	/**
		Serialize this quaternion to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);
	
	CQuaternion3d operator*(const CQuaternion3d& other) const;
	CQuaternion3d operator*(double scalar) const;
	CQuaternion3d operator+(const CQuaternion3d& other) const;
	CQuaternion3d operator-(const CQuaternion3d& other) const;
	
	CQuaternion3d& operator*=(const CQuaternion3d& other);
	CQuaternion3d& operator*=(double scalar);
	
	bool operator==(const CQuaternion3d& other) const;
	bool operator!=(const CQuaternion3d& other) const;

private:
	double m_w;		///< Scalar part
	double m_x;		///< X component
	double m_y;		///< Y component
	double m_z;		///< Z component
};


// inline methods

inline CQuaternion3d::CQuaternion3d()
:	m_w(1.0)
,	m_x(0.0)
,	m_y(0.0)
,	m_z(0.0)
{
}


inline CQuaternion3d::CQuaternion3d(double w, double x, double y, double z)
:	m_w(w)
,	m_x(x)
,	m_y(y)
,	m_z(z)
{
}


inline double CQuaternion3d::GetW() const
{
	return m_w;
}


inline void CQuaternion3d::SetW(double w)
{
	m_w = w;
}


inline double CQuaternion3d::GetX() const
{
	return m_x;
}


inline void CQuaternion3d::SetX(double x)
{
	m_x = x;
}


inline double CQuaternion3d::GetY() const
{
	return m_y;
}


inline void CQuaternion3d::SetY(double y)
{
	m_y = y;
}


inline double CQuaternion3d::GetZ() const
{
	return m_z;
}


inline void CQuaternion3d::SetZ(double z)
{
	m_z = z;
}


inline CVector3d CQuaternion3d::GetVectorPart() const
{
	return CVector3d(m_x, m_y, m_z);
}


inline double CQuaternion3d::GetLengthSq() const
{
	return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
}


inline double CQuaternion3d::GetLength() const
{
	return qSqrt(GetLengthSq());
}


inline CQuaternion3d CQuaternion3d::GetConjugate() const
{
	return CQuaternion3d(m_w, -m_x, -m_y, -m_z);
}


inline double CQuaternion3d::GetDotProduct(const CQuaternion3d& other) const
{
	return m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}


inline bool CQuaternion3d::operator==(const CQuaternion3d& other) const
{
	return qAbs(m_w - other.m_w) < I_BIG_EPSILON &&
	       qAbs(m_x - other.m_x) < I_BIG_EPSILON &&
	       qAbs(m_y - other.m_y) < I_BIG_EPSILON &&
	       qAbs(m_z - other.m_z) < I_BIG_EPSILON;
}


inline bool CQuaternion3d::operator!=(const CQuaternion3d& other) const
{
	return !(*this == other);
}


} // namespace i3d


#endif // !i3d_CQuaternion3d_included
