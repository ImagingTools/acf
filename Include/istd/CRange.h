#ifndef istd_CRange_included
#define istd_CRange_included


#include "istd/istd.h"


namespace istd
{


/**
	Implementation of a abstract range defined by two values - minimum and maximum.

	\ingroup Main
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
		Get reference to the bottom value.	
	*/
	double& GetMinValueRef();

	/**
		Set the bottom value.	
	*/
	void SetMinValue(double minValue);

	/**
		Get the top value.	
	*/
	double GetMaxValue() const;

	/**
		Get reference to the top value.	
	*/
	double& GetMaxValueRef();

	/**
		Set the top value.	
	*/
	void SetMaxValue(double topValue);

	/**
		Set this range to be empty.
	*/
	void Reset();

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
		Returns \c true, if this range is inside of the \c range.
	*/
	bool Contains(const CRange& range) const;

	/**
		Check if this range is intersected by other range.
		Some range is intersected by another, if intercestion of this two ranges is not empty.
	*/
	bool IsIntersectedBy(const CRange& range) const;
	/**
		Get intersection with the second \c range.
	*/
	CRange GetIntersection(const CRange& range) const;
	/**
		Set this range to be intersection of two ranges.
	*/
	void Intersection(const CRange& range);

	/**
		Get union with the second \c range.
	*/
	CRange GetUnion(const CRange& range) const;
	/**
		Set this range to be union of two ranges.
	*/
	void Union(const CRange& range);

	/**
		Get expanded range using the second \c range.
		Expanding operation calculate simple sum for each components separately.
	*/
	CRange GetExpanded(const CRange& range) const;
	/**
		Set this range to be expanded.
		Expanding operation calculate simple sum for each components separately.
	*/
	void Expand(const CRange& range);

	/**
		Get nearest value belonging to range.
	*/
	double GetNearestInside(double value) const;

	/**
		Get value clipped to the range.
	*/
	double GetClipped(double value) const;

	/**
		Get range clipped to the current range.
	*/
	istd::CRange GetClipped(const istd::CRange& range) const;

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
		Returns the value, that corresponds the input value \c inputValue in the other range \c range.
							It is the same as \c range.GetValueFromAlpha(GetAlphaFromValue(value)).
		\param	value		value definde in this range.
							If it is equal to minimum value in this range,
							it will be mapped to minimum value of \c range.
							If it is equal to maximum value in this range,
							it will be mapped to maximum value of \c range.
							Rest values are linear interpolated.
							\note This value must be in current range.
	*/
	double GetMappedTo(double value, const istd::CRange& range) const;

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


inline double& CRange::GetMinValueRef()
{
	return m_minValue;
}


inline double CRange::GetMaxValue() const
{
	return m_maxValue;
}


inline double& CRange::GetMaxValueRef()
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

