#include <i3d/CQuaternion3d.h>


// ACF includes
#include <i3d/CMatrix3d.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace i3d
{


// static constants
static const iser::CArchiveTag s_wTag("W", "W component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_xTag("X", "X component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_yTag("Y", "Y component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_zTag("Z", "Z component", iser::CArchiveTag::TT_LEAF);


CQuaternion3d CQuaternion3d::FromAxisAngle(const CVector3d& axis, double angle)
{
	CVector3d normalizedAxis = axis.GetNormalized();
	double halfAngle = angle * 0.5;
	double sinHalf = qSin(halfAngle);
	
	return CQuaternion3d(
		qCos(halfAngle),
		normalizedAxis.GetX() * sinHalf,
		normalizedAxis.GetY() * sinHalf,
		normalizedAxis.GetZ() * sinHalf
	);
}


CQuaternion3d CQuaternion3d::FromMatrix(const CMatrix3d& matrix)
{
	double trace = matrix.GetAt(0, 0) + matrix.GetAt(1, 1) + matrix.GetAt(2, 2);
	
	CQuaternion3d result;
	
	if (trace > 0.0){
		double s = 0.5 / qSqrt(trace + 1.0);
		result.m_w = 0.25 / s;
		result.m_x = (matrix.GetAt(2, 1) - matrix.GetAt(1, 2)) * s;
		result.m_y = (matrix.GetAt(0, 2) - matrix.GetAt(2, 0)) * s;
		result.m_z = (matrix.GetAt(1, 0) - matrix.GetAt(0, 1)) * s;
	}
	else if (matrix.GetAt(0, 0) > matrix.GetAt(1, 1) && matrix.GetAt(0, 0) > matrix.GetAt(2, 2)){
		double s = 2.0 * qSqrt(1.0 + matrix.GetAt(0, 0) - matrix.GetAt(1, 1) - matrix.GetAt(2, 2));
		result.m_w = (matrix.GetAt(2, 1) - matrix.GetAt(1, 2)) / s;
		result.m_x = 0.25 * s;
		result.m_y = (matrix.GetAt(0, 1) + matrix.GetAt(1, 0)) / s;
		result.m_z = (matrix.GetAt(0, 2) + matrix.GetAt(2, 0)) / s;
	}
	else if (matrix.GetAt(1, 1) > matrix.GetAt(2, 2)){
		double s = 2.0 * qSqrt(1.0 + matrix.GetAt(1, 1) - matrix.GetAt(0, 0) - matrix.GetAt(2, 2));
		result.m_w = (matrix.GetAt(0, 2) - matrix.GetAt(2, 0)) / s;
		result.m_x = (matrix.GetAt(0, 1) + matrix.GetAt(1, 0)) / s;
		result.m_y = 0.25 * s;
		result.m_z = (matrix.GetAt(1, 2) + matrix.GetAt(2, 1)) / s;
	}
	else{
		double s = 2.0 * qSqrt(1.0 + matrix.GetAt(2, 2) - matrix.GetAt(0, 0) - matrix.GetAt(1, 1));
		result.m_w = (matrix.GetAt(1, 0) - matrix.GetAt(0, 1)) / s;
		result.m_x = (matrix.GetAt(0, 2) + matrix.GetAt(2, 0)) / s;
		result.m_y = (matrix.GetAt(1, 2) + matrix.GetAt(2, 1)) / s;
		result.m_z = 0.25 * s;
	}
	
	return result;
}


CQuaternion3d CQuaternion3d::FromEulerAngles(double roll, double pitch, double yaw)
{
	double cr = qCos(roll * 0.5);
	double sr = qSin(roll * 0.5);
	double cp = qCos(pitch * 0.5);
	double sp = qSin(pitch * 0.5);
	double cy = qCos(yaw * 0.5);
	double sy = qSin(yaw * 0.5);
	
	CQuaternion3d result;
	result.m_w = cr * cp * cy + sr * sp * sy;
	result.m_x = sr * cp * cy - cr * sp * sy;
	result.m_y = cr * sp * cy + sr * cp * sy;
	result.m_z = cr * cp * sy - sr * sp * cy;
	
	return result;
}


CQuaternion3d CQuaternion3d::GetNormalized() const
{
	double len = GetLength();
	if (len < I_BIG_EPSILON){
		return CQuaternion3d();
	}
	
	return CQuaternion3d(m_w / len, m_x / len, m_y / len, m_z / len);
}


void CQuaternion3d::Normalize()
{
	double len = GetLength();
	if (len > I_BIG_EPSILON){
		m_w /= len;
		m_x /= len;
		m_y /= len;
		m_z /= len;
	}
}


CQuaternion3d CQuaternion3d::GetInverse() const
{
	double lengthSq = GetLengthSq();
	if (lengthSq < I_BIG_EPSILON){
		return CQuaternion3d();
	}
	
	CQuaternion3d conj = GetConjugate();
	return CQuaternion3d(
		conj.m_w / lengthSq,
		conj.m_x / lengthSq,
		conj.m_y / lengthSq,
		conj.m_z / lengthSq
	);
}


CMatrix3d CQuaternion3d::ToMatrix() const
{
	double xx = m_x * m_x;
	double xy = m_x * m_y;
	double xz = m_x * m_z;
	double xw = m_x * m_w;
	
	double yy = m_y * m_y;
	double yz = m_y * m_z;
	double yw = m_y * m_w;
	
	double zz = m_z * m_z;
	double zw = m_z * m_w;
	
	return CMatrix3d(
		1.0 - 2.0 * (yy + zz),  2.0 * (xy - zw),        2.0 * (xz + yw),
		2.0 * (xy + zw),        1.0 - 2.0 * (xx + zz),  2.0 * (yz - xw),
		2.0 * (xz - yw),        2.0 * (yz + xw),        1.0 - 2.0 * (xx + yy)
	);
}


void CQuaternion3d::ToAxisAngle(CVector3d& axis, double& angle) const
{
	CQuaternion3d normalized = GetNormalized();
	
	angle = 2.0 * qAcos(normalized.m_w);
	
	double sinHalf = qSqrt(1.0 - normalized.m_w * normalized.m_w);
	if (sinHalf < I_BIG_EPSILON){
		// Angle is zero, axis is arbitrary
		axis = CVector3d(1.0, 0.0, 0.0);
	}
	else{
		axis = CVector3d(
			normalized.m_x / sinHalf,
			normalized.m_y / sinHalf,
			normalized.m_z / sinHalf
		);
	}
}


CVector3d CQuaternion3d::Rotate(const CVector3d& vector) const
{
	// Using formula: v' = q * v * q^-1
	// Optimized version without constructing intermediate quaternions
	CVector3d qv(m_x, m_y, m_z);
	CVector3d cross1 = qv.GetCrossProduct(vector);
	CVector3d cross2 = qv.GetCrossProduct(cross1);
	
	return vector + (cross1 * m_w + cross2) * 2.0;
}


CQuaternion3d CQuaternion3d::Slerp(const CQuaternion3d& q1, const CQuaternion3d& q2, double t)
{
	double dot = q1.GetDotProduct(q2);
	
	// If quaternions are very close, use linear interpolation
	if (qAbs(dot) > 0.9995){
		CQuaternion3d result(
			q1.m_w + t * (q2.m_w - q1.m_w),
			q1.m_x + t * (q2.m_x - q1.m_x),
			q1.m_y + t * (q2.m_y - q1.m_y),
			q1.m_z + t * (q2.m_z - q1.m_z)
		);
		return result.GetNormalized();
	}
	
	// Ensure shortest path
	CQuaternion3d q2Fixed = q2;
	if (dot < 0.0){
		q2Fixed = CQuaternion3d(-q2.m_w, -q2.m_x, -q2.m_y, -q2.m_z);
		dot = -dot;
	}
	
	// Clamp dot product
	dot = qMax(-1.0, qMin(1.0, dot));
	
	double theta = qAcos(dot);
	double sinTheta = qSin(theta);
	
	if (qAbs(sinTheta) < I_BIG_EPSILON){
		return q1;
	}
	
	double w1 = qSin((1.0 - t) * theta) / sinTheta;
	double w2 = qSin(t * theta) / sinTheta;
	
	return CQuaternion3d(
		q1.m_w * w1 + q2Fixed.m_w * w2,
		q1.m_x * w1 + q2Fixed.m_x * w2,
		q1.m_y * w1 + q2Fixed.m_y * w2,
		q1.m_z * w1 + q2Fixed.m_z * w2
	);
}


CQuaternion3d CQuaternion3d::operator*(const CQuaternion3d& other) const
{
	return CQuaternion3d(
		m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z,
		m_w * other.m_x + m_x * other.m_w + m_y * other.m_z - m_z * other.m_y,
		m_w * other.m_y - m_x * other.m_z + m_y * other.m_w + m_z * other.m_x,
		m_w * other.m_z + m_x * other.m_y - m_y * other.m_x + m_z * other.m_w
	);
}


CQuaternion3d CQuaternion3d::operator*(double scalar) const
{
	return CQuaternion3d(m_w * scalar, m_x * scalar, m_y * scalar, m_z * scalar);
}


CQuaternion3d CQuaternion3d::operator+(const CQuaternion3d& other) const
{
	return CQuaternion3d(m_w + other.m_w, m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}


CQuaternion3d CQuaternion3d::operator-(const CQuaternion3d& other) const
{
	return CQuaternion3d(m_w - other.m_w, m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}


CQuaternion3d& CQuaternion3d::operator*=(const CQuaternion3d& other)
{
	*this = *this * other;
	return *this;
}


CQuaternion3d& CQuaternion3d::operator*=(double scalar)
{
	m_w *= scalar;
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;
	return *this;
}


bool CQuaternion3d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;
	
	retVal = retVal && archive.BeginTag(s_wTag);
	retVal = retVal && archive.Process(m_w);
	retVal = retVal && archive.EndTag(s_wTag);
	
	retVal = retVal && archive.BeginTag(s_xTag);
	retVal = retVal && archive.Process(m_x);
	retVal = retVal && archive.EndTag(s_xTag);
	
	retVal = retVal && archive.BeginTag(s_yTag);
	retVal = retVal && archive.Process(m_y);
	retVal = retVal && archive.EndTag(s_yTag);
	
	retVal = retVal && archive.BeginTag(s_zTag);
	retVal = retVal && archive.Process(m_z);
	retVal = retVal && archive.EndTag(s_zTag);
	
	return retVal;
}


} // namespace i3d
