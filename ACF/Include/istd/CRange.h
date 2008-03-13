#ifndef istd_CRange_included
#define istd_CRange_included


#include "istd/istd.h"


namespace istd
{


/**
	\ingroup istd

	Implementation of a abstract range of two value.
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
	bool IsInside(double value) const;

	/**
		Returns \c true, if this range is inside of the \c otherRange.
	*/
	bool IsRangeInside(const CRange& range) const;

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
		Returns the value, that corresponds the input value \c inputValue in the other range \c otherRange.
		\note \c inputValue must be in current range.
	*/
	double GetMappedTo(double inputValue, const istd::CRange& otherRange) const;

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

