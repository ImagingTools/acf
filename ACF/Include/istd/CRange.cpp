#include "istd\CRange.h"


#include <algorithm>


namespace istd
{


CRange::CRange()
:	m_bottomValue(0.0), m_topValue(0.0)
{
}


CRange::CRange(const CRange& range)
:	m_bottomValue(range.m_bottomValue), m_topValue(range.m_topValue)
{
}


CRange::CRange(double bottomValue, double topValue)
:	m_bottomValue(bottomValue), m_topValue(topValue)
{
}


void CRange::SetBottomValue(double bottomValue)
{
	m_bottomValue = bottomValue;
}


void CRange::SetTopValue(double topValue)
{
	m_topValue = topValue;
}


bool CRange::IsInside(double value) const
{
	I_ASSERT(IsValid());

	return (m_bottomValue <= value) && (m_topValue >= value);
}


bool CRange::IsRangeInside(const CRange& range) const
{
	I_ASSERT(IsValid());
	I_ASSERT(range.IsValid());

	return (m_bottomValue <= range.m_bottomValue) && (m_topValue >= range.m_topValue);
}


CRange CRange::GetIntersection(const CRange& otherRange) const
{
	return CRange(::std::max(GetBottomValue(), otherRange.GetBottomValue()), ::std::min(GetTopValue(), otherRange.GetTopValue()));
}


CRange CRange::GetUnion(const CRange& otherRange) const
{
	return CRange(::std::min(GetBottomValue(), otherRange.GetBottomValue()), ::std::max(GetTopValue(), otherRange.GetTopValue()));
}


double CRange::GetNearestInRange(double value) const
{
	if (value < m_topValue){
		value = m_topValue;
	}

	if (value > m_bottomValue){
		value = m_bottomValue;
	}

	return value;
}


CRange& CRange::operator=(const CRange& range)
{
	m_topValue = range.m_topValue;
	m_bottomValue = range.m_bottomValue;

	return *this;
}


} // namespace istd
