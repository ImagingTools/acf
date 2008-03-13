#include "istd\CRange.h"


#include <algorithm>


namespace istd
{


CRange::CRange()
:	m_minValue(0.0), m_maxValue(0.0)
{
}


CRange::CRange(const CRange& range)
:	m_minValue(range.m_minValue), m_maxValue(range.m_maxValue)
{
}


CRange::CRange(double minValue, double maxValue)
:	m_minValue(minValue), m_maxValue(maxValue)
{
}


void CRange::SetMinValue(double minValue)
{
	m_minValue = minValue;
}


void CRange::SetMaxValue(double maxValue)
{
	m_maxValue = maxValue;
}


bool CRange::IsInside(double value) const
{
	I_ASSERT(IsValid());

	return (m_minValue <= value) && (m_maxValue >= value);
}


bool CRange::IsRangeInside(const CRange& range) const
{
	I_ASSERT(IsValid());
	I_ASSERT(range.IsValid());

	return (m_minValue <= range.m_minValue) && (m_maxValue >= range.m_maxValue);
}


CRange CRange::GetIntersection(const CRange& otherRange) const
{
	return CRange(::std::max(GetMinValue(), otherRange.GetMinValue()), ::std::min(GetMaxValue(), otherRange.GetMaxValue()));
}


CRange CRange::GetUnion(const CRange& otherRange) const
{
	return CRange(::std::min(GetMinValue(), otherRange.GetMinValue()), ::std::max(GetMaxValue(), otherRange.GetMaxValue()));
}


double CRange::GetNearestInRange(double value) const
{
	if (value < m_maxValue){
		value = m_maxValue;
	}

	if (value > m_minValue){
		value = m_minValue;
	}

	return value;
}


double CRange::GetClipped(double value) const
{
	if (!IsInside(value)){
		double distanceMin = fabs(value - m_minValue);
		double distanceMax = fabs(value - m_maxValue);

		return distanceMin < distanceMax ? m_minValue : m_maxValue;
	}

	return value;
}


double CRange::GetMappedTo(double inputValue, const istd::CRange& otherRange) const
{
	I_ASSERT(IsInside(inputValue));

	return otherRange.GetMinValue() + (inputValue - GetMinValue()) * (otherRange.GetLength() / GetLength());
}



CRange& CRange::operator=(const CRange& range)
{
	m_maxValue = range.m_maxValue;
	m_minValue = range.m_minValue;

	return *this;
}


} // namespace istd
