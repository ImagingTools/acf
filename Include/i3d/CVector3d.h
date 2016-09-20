#ifndef i3d_CVector3d_included
#define i3d_CVector3d_included


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <imath/TVector.h>
#include <i2d/CVector2d.h>
#include <i3d/i3d.h>


namespace i3d
{


/**
	Definition of position or mathematical vector in 3D space.
*/
class CVector3d: public imath::TVector<3>
{
public:
	typedef imath::TVector<3> BaseClass;

	/**
		Default constructor.
		Please note, elements will be not initilized.
	 */
	CVector3d();
	/**
		Construct initializing elements to specified values.
	 */
	CVector3d(double x, double y, double z);
	/**
		Copy constructor.
	 */
	CVector3d(const imath::TVector<3>& vector);

	/**
		Contructs 3D vector from 2D.
	*/
	CVector3d(const i2d::CVector2d& vector, double z = 0);

	/**
		Get X position of this vector.
	*/
	double GetX() const;
	/**
		Set X position of this vector.
	*/
	void SetX(double value);
	/**
		Get Y position of this vector.
	*/
	double GetY() const;
	/**
		Set Y position of this vector.
	*/
	void SetY(double value);
	/**
		Get Y position of this vector.
	*/
	double GetZ() const;
	/**
		Set Y position of this vector.
	*/
	void SetZ(double value);

	using BaseClass::GetTranslated;
	/**
		Get translated point.
	 */
	CVector3d GetTranslated(const imath::TVector<3>& vector);

	/**
		Return Z coordinate of two vectors cross product.
	 */
	CVector3d GetCrossProduct(const imath::TVector<3>& vector) const;

	using BaseClass::GetNormalized;
	/**
		Return normalized vector with the same direction and specified length.
	 */
	CVector3d GetNormalized(double length = 1.0) const;

	/**
		Get XY part of this 3D vector.
	*/
	i2d::CVector2d GetPlaneCasted() const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	CVector3d operator-() const;

	CVector3d operator+(const imath::TVector<3>& vector) const;
	CVector3d operator-(const imath::TVector<3>& vector) const;
	CVector3d operator*(double scalar) const;
	CVector3d operator/(double scalar) const;

	CVector3d& operator+=(const imath::TVector<3>& vector);
	CVector3d& operator-=(const imath::TVector<3>& vector);
	CVector3d& operator*=(double scalar);
	CVector3d& operator/=(double scalar);
};


// inline methods

inline CVector3d::CVector3d()
{
}


inline CVector3d::CVector3d(double x, double y, double z)
{
	operator[](0) = x;
	operator[](1) = y;
	operator[](2) = z;
}


inline CVector3d::CVector3d(const imath::TVector<3>& vector)
:	BaseClass(vector)
{
}


inline CVector3d::CVector3d(const i2d::CVector2d& vector, double z)
{
	operator[](0) = vector.GetX();
	operator[](1) = vector.GetY();
	operator[](2) = z;
}


inline double CVector3d::GetX() const
{
	return operator[](0);
}


inline void CVector3d::SetX(double value)
{
	operator[](0) = value;
}


inline double CVector3d::GetY() const
{
	return operator[](1);
}


inline void CVector3d::SetY(double value)
{
	operator[](1) = value;
}


inline double CVector3d::GetZ() const
{
	return operator[](2);
}


inline void CVector3d::SetZ(double value)
{
	operator[](2) = value;
}


inline CVector3d CVector3d::GetTranslated(const imath::TVector<3>& vector)
{
	return *this + vector;
}


inline i2d::CVector2d CVector3d::GetPlaneCasted() const
{
	return i2d::CVector2d(GetX(), GetY());
}


inline CVector3d CVector3d::operator-() const
{
	return CVector3d(-GetX(), -GetY(), -GetZ());
}



inline CVector3d CVector3d::operator+(const imath::TVector<3>& vector) const
{
	return CVector3d(GetX() + vector[0], GetY() + vector[1], GetZ() + vector[2]);
}



inline CVector3d CVector3d::operator-(const imath::TVector<3>& vector) const
{
	return CVector3d(GetX() - vector[0], GetY() - vector[1], GetZ() - vector[2]);
}



inline CVector3d CVector3d::operator*(double scalar) const
{
	return CVector3d(GetX() * scalar, GetY() * scalar, GetZ() * scalar);
}



inline CVector3d CVector3d::operator/(double scalar) const
{
	if (qAbs(scalar) < I_BIG_EPSILON){
		scalar = (scalar > 0)? I_BIG_EPSILON: -I_BIG_EPSILON;
	}

	return CVector3d(GetX() / scalar, GetY() / scalar, GetZ() / scalar);
}



inline CVector3d& CVector3d::operator+=(const imath::TVector<3>& vector)
{
	BaseClass::operator+=(vector);

	return *this;
}



inline CVector3d& CVector3d::operator-=(const imath::TVector<3>& vector)
{
	BaseClass::operator-=(vector);

	return *this;
}


inline CVector3d& CVector3d::operator*=(double scalar)
{
	BaseClass::operator*=(scalar);

	return *this;
}


inline CVector3d& CVector3d::operator/=(double scalar)
{
	BaseClass::operator/=(scalar);

	return *this;
}


} // namespace i3d


#endif // !i3d_CVector3d_included

