#ifndef i2d_CVector2d_included
#define i2d_CVector2d_included


// Qt includes
#include <QtCore/qmath.h>
#include <QtCore/QtGlobal>
#include <QtCore/QPointF>

// ACF includes
#include "istd/CIndex2d.h"
#include "imath/TVector.h"

#include "i2d/i2d.h"


namespace i2d
{


/**
	Definition of position or mathematical vector on 2D plane.
*/
class CVector2d: public imath::TVector<2>
{
public:
	typedef imath::TVector<2> BaseClass;

	/**
		Default constructor.
		Please note, elements will be not initialized.
	 */
	CVector2d();
	
	/**
		Construct initializing elements to specified values.
	 */
	CVector2d(double x, double y);
	
	CVector2d(const istd::CIndex2d& index);

	CVector2d(const QPointF& point);

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
		Get horizontal translated point.
	*/
	CVector2d GetHorizontalTranslated(double offsetX) const;
	/**
		Get vertical translated point.
	*/
	CVector2d GetVerticalTranslated(double offsetY) const;

	/**
		Return Z coordinate of two vectors cross product.
	 */
	double GetCrossProductZ(const imath::TVector<2>& vector) const;

	double GetDotProduct(const CVector2d& vector) const;
	/**
		Get angle of this vector.
	 */
	double GetAngle() const;
	/**
		Get orthogonal vector.
	 */
	CVector2d GetOrthogonal() const;

	/**
		Get vector converted to 2D index.
	*/
	istd::CIndex2d ToIndex2d() const;

	using BaseClass::GetNormalized;
	/**
		Return normalized vector with the same direction and specified length.
	 */
	CVector2d GetNormalized(double length = 1.0) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	CVector2d operator-() const;

	CVector2d operator+(const imath::TVector<2>& vector) const;
	CVector2d operator-(const imath::TVector<2>& vector) const;
	CVector2d operator*(double scalar) const;
	CVector2d operator/(double scalar) const;

	CVector2d& operator+=(const imath::TVector<2>& vector);
	CVector2d& operator-=(const imath::TVector<2>& vector);
	CVector2d& operator*=(double scalar);
	CVector2d& operator/=(double scalar);

	operator QPointF() const;

	static const CVector2d& GetZero();
};


// inline methods

inline CVector2d::CVector2d(const QPointF& point)
{
	operator[](0) = point.x();
	operator[](1) = point.y();
}


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


inline CVector2d::CVector2d(const istd::CIndex2d& index)
{
	operator[](0) = index.GetX();
	operator[](1) = index.GetY();
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


inline CVector2d CVector2d::GetHorizontalTranslated(double offsetX) const
{
	return CVector2d(GetX() + offsetX, GetY());
}


inline CVector2d CVector2d::GetVerticalTranslated(double offsetY) const
{
	return CVector2d(GetX(), GetY() + offsetY);
}


inline double CVector2d::GetCrossProductZ(const imath::TVector<2>& vector) const
{
	return GetX() * vector[1] - GetY() * vector[0];
}


inline double CVector2d::GetDotProduct(const CVector2d& vector) const
{
	return GetX() * vector.GetX() + GetY() * vector.GetY();
}


inline double CVector2d::GetAngle() const
{
	return qAtan2(GetY(), GetX());
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
	if (qAbs(scalar) < I_BIG_EPSILON){
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


inline CVector2d::operator QPointF() const
{
	return QPointF(qreal(GetX()), qreal(GetY()));
}


// static methods

inline const CVector2d& CVector2d::GetZero()
{
	return reinterpret_cast<const CVector2d&>(BaseClass::GetZero());
}


// related global functions

inline uint qHash(const i2d::CVector2d& key, uint seed = 0)
{
	Q_UNUSED(seed);

	return int(key.GetX()) ^ int(key.GetY());
}


} // namespace i2d


#endif // !i2d_CVector2d_included


