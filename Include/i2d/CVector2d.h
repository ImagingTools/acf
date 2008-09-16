#ifndef i2d_CVector2d_included
#define i2d_CVector2d_included


#include <math.h>

#include "imath/TVector.h"

#include "i2d/i2d.h"


namespace i2d
{


class CVector2d: public imath::TVector<2>
{
public:
	typedef imath::TVector<2> BaseClass;

	/**
		Default constructor.
		Please note, elements will be not initilized.
	 */
	CVector2d();
	/**
		Construct initializing elements to specified values.
	 */
	CVector2d(double x, double y);
	/**
		Copy constructor.
	 */
	CVector2d(const imath::TVector<2>& vector);

	/**
		Get X position of this vector.
	*/
	double GetX() const;
	/**
		Set X position of this vector.
	*/
	void SetX(double x);
	/**
		Get Y position of this vector.
	*/
	double GetY() const;
	/**
		Set Y position of this vector.
	*/
	void SetY(double y);

	/**
		Init this vector using angle and vector length.
	 */
	void Init(double angle, double length = 1.0);

	using BaseClass::GetTranslated;
	/**
		Get translated point.
	 */
	CVector2d GetTranslated(const imath::TVector<2>& vector);

	/**
		Return Z coordinate of two vectors cross product.
	 */
	double GetCrossProductZ(const imath::TVector<2>& vector) const;
	/**
		Get angle of this vector.
	 */
	double GetAngle() const;
	/**
		Get orthogonal vector.
	 */
	CVector2d GetOrthogonal() const;

	using BaseClass::GetNormalized;
	/**
		Return normalized vector with the same direction and specified length.
	 */
	CVector2d GetNormalized(double length) const;

	CVector2d operator-() const;

	CVector2d operator+(const imath::TVector<2>& vector) const;
	CVector2d operator-(const imath::TVector<2>& vector) const;
	CVector2d operator*(double scalar) const;
	CVector2d operator/(double scalar) const;

	CVector2d& operator+=(const imath::TVector<2>& vector);
	CVector2d& operator-=(const imath::TVector<2>& vector);
	CVector2d& operator*=(double scalar);
	CVector2d& operator/=(double scalar);
};


// inline methods

inline CVector2d::CVector2d()
{
}


inline CVector2d::CVector2d(double x, double y)
{
	operator[](0) = x;
	operator[](1) = y;
}


inline CVector2d::CVector2d(const imath::TVector<2>& vector)
:	BaseClass(vector)
{
}


inline double CVector2d::GetX() const
{
	return operator[](0);
}


inline void CVector2d::SetX(double x)
{
	operator[](0) = x;
}


inline double CVector2d::GetY() const
{
	return operator[](1);
}


inline void CVector2d::SetY(double y)
{
	operator[](1) = y;
}


inline CVector2d CVector2d::GetTranslated(const imath::TVector<2>& vector)
{
	return *this + vector;
}


inline double CVector2d::GetCrossProductZ(const imath::TVector<2>& vector) const
{
	return GetX() * vector[1] - GetY() * vector[0];
}


inline double CVector2d::GetAngle() const
{
	return ::atan2(GetY(), GetX());
}


inline CVector2d CVector2d::operator-() const
{
	return CVector2d(-GetX(), -GetY());
}



inline CVector2d CVector2d::operator+(const imath::TVector<2>& vector) const
{
	return CVector2d(GetX() + vector[0], GetY() + vector[1]);
}



inline CVector2d CVector2d::operator-(const imath::TVector<2>& vector) const
{
	return CVector2d(GetX() - vector[0], GetY() - vector[1]);
}



inline CVector2d CVector2d::operator*(double scalar) const
{
	return CVector2d(GetX() * scalar, GetY() * scalar);
}



inline CVector2d CVector2d::operator/(double scalar) const
{
	if (::fabs(scalar) < I_BIG_EPSILON){
		scalar = (scalar > 0)? I_BIG_EPSILON: -I_BIG_EPSILON;
	}

	return CVector2d(GetX() / scalar, GetY() / scalar);
}



inline CVector2d& CVector2d::operator+=(const imath::TVector<2>& vector)
{
	BaseClass::operator+=(vector);

	return *this;
}



inline CVector2d& CVector2d::operator-=(const imath::TVector<2>& vector)
{
	BaseClass::operator-=(vector);

	return *this;
}


inline CVector2d& CVector2d::operator*=(double scalar)
{
	BaseClass::operator*=(scalar);

	return *this;
}


inline CVector2d& CVector2d::operator/=(double scalar)
{
	BaseClass::operator/=(scalar);

	return *this;
}


} // namespace i2d


#endif // !i2d_CVector2d_included

