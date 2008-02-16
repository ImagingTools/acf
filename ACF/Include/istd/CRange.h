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
	CRange(double bottomValue, double topValue);

	/**
		Return \c true if the bottom value is smaller or equal then the top value.
	*/
	bool IsValid() const;

	/**
		Get the bottom value.	
	*/
	double GetBottomValue() const;

	/**
		Set the bottom value.	
	*/
	void SetBottomValue(double bottomValue);

	/**
		Get the top value.	
	*/
	double GetTopValue() const;

	/**
		Set the top value.	
	*/
	void SetTopValue(double topValue);

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

	CRange& operator=(const CRange& range);
	bool operator==(const CRange& range) const;
	bool operator!=(const CRange& range) const;

private:
	double m_topValue;
	double m_bottomValue;
};


// inline methods

inline bool CRange::IsValid() const
{
	return m_bottomValue <= m_topValue;
}


inline double CRange::GetBottomValue() const
{
	return m_bottomValue;
}


inline double CRange::GetTopValue() const
{
	return m_topValue;
}


inline double CRange::GetLength() const
{
	return IsValid()? m_topValue - m_bottomValue: 0;
}


inline bool CRange::operator==(const CRange& range) const
{
	return (m_topValue == range.m_topValue) && (m_bottomValue == range.m_bottomValue);
}


inline bool CRange::operator!=(const CRange& range) const
{
	return (m_topValue != range.m_topValue) || (m_bottomValue != range.m_bottomValue);
}


} // namespace istd


#endif // !istd_CRange_included
