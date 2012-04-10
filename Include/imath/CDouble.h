#ifndef istd_CDouble_included
#define istd_CDouble_included


// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include "iser/IArchive.h"

#include "imath/TVector.h"


namespace imath
{


/**
	Simple wrapper of real value represented as double type.
*/
class CDouble: public TVector<1>
{
public:
	typedef TVector<1> BaseClass;

	CDouble(double value = 0.0);
	CDouble(const BaseClass& value);

	/**
		Get rounded value.
	*/
	CDouble GetRounded(int precision = 2) const;

	/**
		Check if two values are equal after rounding.
	*/
	bool IsRoundedEqual(const CDouble& value, int precision = 2) const;

	/**
		Get rounded down value with specified precision.
	*/
	CDouble GetRoundedDown(int precision = 2) const;

	/**
		Check if two values are equal after rounding down.
	*/
	bool IsRoundedDownEqual(const CDouble& value, int precision = 2) const;

	/**
		Get rounded down value with specified precision.
	*/
	CDouble GetRoundedUp(int precision = 2) const;

	/**
		Check if two values are equal after rounding down.
	*/
	bool IsRoundedUpEqual(const CDouble& value, int precision = 2) const;

	/**
		Check if two values are similiar with specified tolerance.
	*/
	bool IsSimiliar(const CDouble& value, double tolerance = I_BIG_EPSILON) const;

	// operators
	operator double() const;

	bool operator==(const CDouble& value) const;
	bool operator!=(const CDouble& value) const;
	bool operator<(const CDouble& value) const;
	bool operator>(const CDouble& value) const;
	bool operator<=(const CDouble& value) const;
	bool operator>=(const CDouble& value) const;

	CDouble operator+(const CDouble& value) const;
	CDouble operator-(const CDouble& value) const;
	CDouble operator*(const CDouble& value) const;
	CDouble operator/(const CDouble& value) const;

	const CDouble& operator=(const CDouble& value);

	const CDouble& operator+=(const CDouble& value);
	const CDouble& operator-=(const CDouble& value);
	const CDouble& operator*=(const CDouble& value);
	const CDouble& operator/=(const CDouble& value);

	// static methods
	static double GetRounded(double value, int precision = 2);
	static bool IsRoundedEqual(double value1, double value2, int precision = 2);
	static double GetRoundedDown(double value, int precision = 2);
	static bool IsRoundedDownEqual(double value1, double value2, int precision = 2);
	static double GetRoundedUp(double value, int precision = 2);
	static bool IsRoundedUpEqual(double value1, double value2, int precision = 2);
	static bool IsSimiliar(double value1, double value2, double tolerance = I_BIG_EPSILON);
};


// inline methods
	
inline CDouble::CDouble(double value)
{
	SetElement(0, value);
}


inline CDouble::CDouble(const TVector<1>& value)
:	BaseClass(value)
{
}


inline CDouble::operator double() const
{
	return GetElement(0);
}


inline CDouble CDouble::GetRounded(int precision) const
{
	return GetRounded(GetElement(0), precision);
}


inline bool CDouble::IsRoundedEqual(const CDouble& value, int precision) const
{
	return IsRoundedEqual(*this, value, precision);
}


inline CDouble CDouble::GetRoundedDown(int precision) const
{
	return GetRoundedDown(*this, precision);
}


inline bool CDouble::IsRoundedDownEqual(const CDouble& value, int precision) const
{
	return IsRoundedDownEqual(*this, value, precision);
}


inline CDouble CDouble::GetRoundedUp(int precision) const
{
	return GetRoundedUp(*this, precision);
}


inline bool CDouble::IsRoundedUpEqual(const CDouble& value, int precision) const
{
	return IsRoundedUpEqual(*this, value, precision);
}


inline bool CDouble::IsSimiliar(const CDouble& value, double tolerance) const
{
	return IsSimiliar(*this, value, tolerance);
}


inline bool CDouble::operator==(const CDouble& value) const
{
	return GetElement(0) == value.GetElement(0);
}


inline bool CDouble::operator!=(const CDouble& value) const
{
	return GetElement(0) != value.GetElement(0);
}


inline bool CDouble::operator<(const CDouble& value) const
{
	return GetElement(0) < value.GetElement(0);
}


inline bool CDouble::operator>(const CDouble& value) const
{
	return GetElement(0) > value.GetElement(0);
}


inline bool CDouble::operator<=(const CDouble& value) const
{
	return GetElement(0) <= value.GetElement(0);
}


inline bool CDouble::operator>=(const CDouble& value) const
{
	return GetElement(0) >= value.GetElement(0);
}


inline CDouble CDouble::operator+(const CDouble& value) const
{
	return GetElement(0) + value.GetElement(0);
}


inline CDouble CDouble::operator-(const CDouble& value) const
{
	return GetElement(0) - value.GetElement(0);
}


inline CDouble CDouble::operator*(const CDouble& value) const
{
	return GetElement(0) * value.GetElement(0);
}


inline CDouble CDouble::operator/(const CDouble& value) const
{
	return GetElement(0) / value.GetElement(0);
}


inline const CDouble& CDouble::operator=(const CDouble& value)
{
	operator[](0) = value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator+=(const CDouble& value)
{
	operator[](0) += value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator-=(const CDouble& value)
{
	operator[](0) -= value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator*=(const CDouble& value)
{
	operator[](0) *= value.GetElement(0);

	return *this;
}


inline const CDouble& CDouble::operator/=(const CDouble& value)
{
	operator[](0) /= value.GetElement(0);

	return *this;
}


// static methods

inline double CDouble::GetRounded(double value, int precision)
{
	double scale = qPow(10.0, precision);

	return std::floor(value * scale + 0.5) / scale;
}


inline bool CDouble::IsRoundedEqual(double value1, double value2, int precision)
{
	double scale = qPow(10.0, precision);

	return std::floor(value1 * scale + 0.5) == std::floor(value2 * scale + 0.5);
}


inline double CDouble::GetRoundedDown(double value, int precision)
{
	double scale = qPow(10.0, precision);

	return std::floor(value * scale) / scale;
}


inline bool CDouble::IsRoundedDownEqual(double value1, double value2, int precision)
{
	double scale = qPow(10.0, precision);

	return std::floor(value1 * scale) == std::floor(value2 * scale);
}


inline double CDouble::GetRoundedUp(double value, int precision)
{
	double scale = qPow(10.0, precision);

	return std::ceil(value * scale) / scale;
}


inline bool CDouble::IsRoundedUpEqual(double value1, double value2, int precision)
{
	double scale = qPow(10.0, precision);

	return std::ceil(value1 * scale) == std::ceil(value2 * scale);
}


inline bool CDouble::IsSimiliar(double value1, double value2, double tolerance)
{
	return qAbs(value1 - value2) <= tolerance;
}


} // namespace imath


#endif // !istd_CDouble_included


