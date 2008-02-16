#ifndef istd_CDouble_included
#define istd_CDouble_included


#include "math.h"

#include "iser/IArchive.h"


namespace imath
{


/**
	Simple wrapper of real value represented as double type.
*/
class CDouble
{
public:
	CDouble(double value = 0.0);
	CDouble(const CDouble& value);

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

	bool Serialize(iser::IArchive& archive);

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

private:
	double m_value;
};


// inline methods
	
inline CDouble::CDouble(double value)
:	m_value(value)
{
}


inline CDouble::operator double() const
{
	return m_value;
}


inline CDouble CDouble::GetRounded(int precision) const
{
	return GetRounded(m_value, precision);
}


inline bool CDouble::IsRoundedEqual(const CDouble& value, int precision) const
{
	return IsRoundedEqual(m_value, value.m_value, precision);
}


inline CDouble CDouble::GetRoundedDown(int precision) const
{
	return GetRoundedDown(m_value, precision);
}


inline bool CDouble::IsRoundedDownEqual(const CDouble& value, int precision) const
{
	return IsRoundedDownEqual(m_value, value.m_value, precision);
}


inline CDouble CDouble::GetRoundedUp(int precision) const
{
	return GetRoundedUp(m_value, precision);
}


inline bool CDouble::IsRoundedUpEqual(const CDouble& value, int precision) const
{
	return IsRoundedUpEqual(m_value, value.m_value, precision);
}


inline bool CDouble::IsSimiliar(const CDouble& value, double tolerance) const
{
	return IsSimiliar(m_value, value, tolerance);
}


inline bool CDouble::operator==(const CDouble& value) const
{
	return m_value == value.m_value;
}


inline bool CDouble::operator!=(const CDouble& value) const
{
	return m_value != value.m_value;
}


inline bool CDouble::operator<(const CDouble& value) const
{
	return m_value < value.m_value;
}


inline bool CDouble::operator>(const CDouble& value) const
{
	return m_value > value.m_value;
}


inline bool CDouble::operator<=(const CDouble& value) const
{
	return m_value <= value.m_value;
}


inline bool CDouble::operator>=(const CDouble& value) const
{
	return m_value >= value.m_value;
}


inline CDouble CDouble::operator+(const CDouble& value) const
{
	return m_value + value.m_value;
}


inline CDouble CDouble::operator-(const CDouble& value) const
{
	return m_value - value.m_value;
}


inline CDouble CDouble::operator*(const CDouble& value) const
{
	return m_value * value.m_value;
}


inline CDouble CDouble::operator/(const CDouble& value) const
{
	return m_value / value.m_value;
}


inline const CDouble& CDouble::operator=(const CDouble& value)
{
	m_value = value.m_value;

	return *this;
}


inline const CDouble& CDouble::operator+=(const CDouble& value)
{
	m_value += value.m_value;

	return *this;
}


inline const CDouble& CDouble::operator-=(const CDouble& value)
{
	m_value -= value.m_value;

	return *this;
}


inline const CDouble& CDouble::operator*=(const CDouble& value)
{
	m_value *= value.m_value;

	return *this;
}


inline const CDouble& CDouble::operator/=(const CDouble& value)
{
	m_value /= value.m_value;

	return *this;
}


// static methods

inline double CDouble::GetRounded(double value, int precision)
{
	double scale = ::pow(10.0, precision);

	return ::floor(value * scale + 0.5) / scale;
}


inline bool CDouble::IsRoundedEqual(double value1, double value2, int precision)
{
	double scale = ::pow(10.0, precision);

	return ::floor(value1 * scale + 0.5) == ::floor(value2 * scale + 0.5);
}


inline double CDouble::GetRoundedDown(double value, int precision)
{
	double scale = ::pow(10.0, precision);

	return ::floor(value * scale) / scale;
}


inline bool CDouble::IsRoundedDownEqual(double value1, double value2, int precision)
{
	double scale = ::pow(10.0, precision);

	return ::floor(value1 * scale) == ::floor(value2 * scale);
}


inline double CDouble::GetRoundedUp(double value, int precision)
{
	double scale = ::pow(10.0, precision);

	return ::floor(value * scale + 0.5) / scale;	// TODO: correct it
}


inline bool CDouble::IsRoundedUpEqual(double value1, double value2, int precision)
{
	double scale = ::pow(10.0, precision);

	return ::floor(value1 * scale + 0.5) == ::floor(value2 * scale);	// TODO: correct it
}


inline bool CDouble::IsSimiliar(double value1, double value2, double tolerance)
{
	return ::fabs(value1 - value2) <= tolerance;
}


} // namespace imath


#endif // !istd_CDouble_included


