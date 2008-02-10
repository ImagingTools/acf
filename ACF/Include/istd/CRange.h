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
		Constructs a range object from two numbers.
	*/
	CRange(double bottomValue, double topValue);

	/**
		Returns \c true if the bottom value is smaller or equal then the top value.
	*/
	bool IsValid() const;

	/**
		Sets the bottom value.	
	*/
	void SetBottomValue(double bottomValue);

	/**
		Sets the top value.	
	*/
	void SetTopValue(double topValue);

	/**
		Sets the bottom value.	
	*/
	double GetBottomValue() const;

	/**
		Sets the top value.	
	*/
	double GetTopValue() const;

	/**
		Returns \c true, if value is in range between \c top and \c bottom.
	*/
	bool IsInside(double value) const;

	/**
		Returns \c true, if this range is inside of the \c otherRange.
	*/
	bool IsInside(const CRange& otherRange) const;

	/**
		Gets intersection with the \c otherRange.
	*/
	CRange GetIntersection(const CRange& otherRange) const;

	/**
		Gets union with the \c otherRange.
	*/
	CRange GetUnion(const CRange& otherRange) const;

private:
	double m_topValue;
	double m_bottomValue;
};


} // namespace istd


#endif // !istd_CRange_included
