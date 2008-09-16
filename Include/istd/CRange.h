#ifndef istd_CRange_included
#define istd_CRange_included


#include "istd/istd.h"


namespace istd
{


/**
	\ingroup istd

	Implementation of a abstract range defined by two values - minimum and maximum.
*/
class CRange
{
public:
	/**
		Constructs an empty range object. IsValid() will return \c false for this object.
	*/
	CRange();
	/**
		Copy constructor.
	*/
	CRange(const CRange& range);

	/**
		Constructs a range object from two numbers.
	*/
	CRange(double minValue, double topValue);

	/**
		Return \c true if the bottom value is smaller or equal then the top value.
	*/
	bool IsValid() const;

	/**
		Return \c true if the bottom value is equal to the top value.
	*/
	bool IsEmpty() const;

	/**
		Get the bottom value.	
	*/
	double GetMinValue() const;

	/**
		Set the bottom value.	
	*/
	void SetMinValue(double minValue);

	/**
		Get the top value.	
	*/
	double GetMaxValue() const;

	/**
		Set the top value.	
	*/
	void SetMaxValue(double topValue);

	/**
		Get length of this range.
		Length is difference between top and bottom values.
	*/
	double GetLength() const;

	/**
		Returns \c true, if value is in range between \c top and \c bottom.
		This range must be valid.
	*/
	bool Contains(double value) const;

	/**
		Returns \c true, if this range is inside of the \c otherRange.
	*/
	bool Contains(const CRange& range) const;

	/**
		Gets intersection with the \c otherRange.
	*/
	CRange GetIntersection(const CRange& otherRange) const;

	/**
		Gets union with the \c otherRange.
	*/
	CRange GetUnion(const CRange& otherRange) const;

	/**
		Get nearest value belonging to range.
	*/
	double GetNearestInRange(double value) const;

	/**
		Get value clipped to the range.
	*/
	double GetClipped(double value) const;

	/**
		Get value based on 'alpha' factor.
		\param	alpha	alpha factor. If it is 0, minimum range value will be returned.
						If it is 1, maximum range value will be returned.
						Rest values are linear interpolated.
	*/
	double GetValueFromAlpha(double alpha) const;

	/**
		Get value based on 'alpha' factor.
		\param	alpha	alpha factor. If it is 0, minimum range value will be returned.
						If it is 1, maximum range value will be returned.
						Rest values are linear interpolated.
	*/
	double GetAlphaFromValue(double value) const;

	/**
		Returns the value, that corresponds the input value \c inputValue in the other range \c otherRange.
							It is the same as \c otherRange.GetValueFromAlpha(GetAlphaFromValue(value)).
		\param	value		value definde in this range.
							If it is equal to minimum value in this range,
							it will be mapped to minimum value of \c otherRange.
							If it is equal to maximum value in this range,
							it will be mapped to maximum value of \c otherRange.
							Rest values are linear interpolated.
							\note This value must be in current range.
	*/
	double GetMappedTo(double value, const istd::CRange& otherRange) const;

	CRange& operator=(const CRange& range);
	bool operator==(const CRange& range) const;
	bool operator!=(const CRange& range) const;

private:
	double m_minValue;
	double m_maxValue;
};


// inline methods

inline bool CRange::IsValid() const
{
	return (m_minValue <= m_maxValue);
}


inline bool CRange::IsEmpty() const
{
	return (m_minValue == m_maxValue);
}


inline double CRange::GetMinValue() const
{
	return m_minValue;
}


inline double CRange::GetMaxValue() const
{
	return m_maxValue;
}


inline double CRange::GetLength() const
{
	return IsValid()? m_maxValue - m_minValue: 0;
}


inline bool CRange::Contains(double value) const
{
	I_ASSERT(IsValid());

	return (m_minValue <= value) && (m_maxValue >= value);
}


inline bool CRange::Contains(const CRange& range) const
{
	I_ASSERT(IsValid());
	I_ASSERT(range.IsValid());

	return (m_minValue <= range.m_minValue) && (m_maxValue >= range.m_maxValue);
}


inline double CRange::GetValueFromAlpha(double alpha) const
{
	return GetMinValue() + alpha * GetLength();
}


inline double CRange::GetAlphaFromValue(double value) const
{
	return (value - GetMinValue()) / GetLength();
}


inline bool CRange::operator==(const CRange& range) const
{
	return (m_maxValue == range.m_maxValue) && (m_minValue == range.m_minValue);
}


inline bool CRange::operator!=(const CRange& range) const
{
	return (m_maxValue != range.m_maxValue) || (m_minValue != range.m_minValue);
}


} // namespace istd


#endif // !istd_CRange_included

