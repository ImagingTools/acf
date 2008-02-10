#include "istd\CRange.h"


#include <algorithm>


namespace istd
{


CRange::CRange()
	:m_bottomValue(0.0),
	m_topValue(0.0)
{
}


CRange::CRange(double bottomValue, double topValue)
	:m_bottomValue(bottomValue),
	m_topValue(topValue)
{
}


bool CRange::IsValid() const
{
	return m_bottomValue <= m_topValue;
}


void CRange::SetBottomValue(double bottomValue)
{
	m_bottomValue = bottomValue;
}


void CRange::SetTopValue(double topValue)
{
	m_topValue = topValue;
}



double CRange::GetBottomValue() const
{
	return m_bottomValue;
}


double CRange::GetTopValue() const
{
	return m_topValue;
}


bool CRange::IsInside(double value) const
{
	I_ASSERT(IsValid());

	return ((value - m_bottomValue) >= I_EPSYLON) && ((m_topValue - value) >= I_EPSYLON);
}


bool CRange::IsInside(const CRange& otherRange) const
{
	return (IsInside(otherRange.GetBottomValue()) && IsInside(otherRange.GetTopValue()));
}


CRange CRange::GetIntersection(const CRange& otherRange) const
{
	return CRange(std::max(GetBottomValue(), otherRange.GetBottomValue()), std::min(GetTopValue(), otherRange.GetTopValue()));
}


CRange CRange::GetUnion(const CRange& otherRange) const
{
	return CRange(std::min(GetBottomValue(), otherRange.GetBottomValue()), std::max(GetTopValue(), otherRange.GetTopValue()));
}


} // namespace istd
