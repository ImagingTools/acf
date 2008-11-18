#include "istd/CRange.h"

#include <math.h>


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


CRange CRange::GetIntersection(const CRange& otherRange) const
{
	return CRange(istd::Max(GetMinValue(), otherRange.GetMinValue()), istd::Min(GetMaxValue(), otherRange.GetMaxValue()));
}


CRange CRange::GetUnion(const CRange& otherRange) const
{
	return CRange(istd::Min(GetMinValue(), otherRange.GetMinValue()), istd::Max(GetMaxValue(), otherRange.GetMaxValue()));
}


double CRange::GetNearestInRange(double value) const
{
	if (value > m_maxValue){
		value = m_maxValue;
	}

	if (value < m_minValue){
		value = m_minValue;
	}

	return value;
}


double CRange::GetClipped(double value) const
{
	if (!Contains(value)){
		double distanceMin = fabs(value - m_minValue);
		double distanceMax = fabs(value - m_maxValue);

		return distanceMin < distanceMax ? m_minValue : m_maxValue;
	}

	return value;
}


double CRange::GetMappedTo(double value, const istd::CRange& otherRange) const
{
	return otherRange.GetMinValue() + (value - GetMinValue()) * (otherRange.GetLength() / GetLength());
}



CRange& CRange::operator=(const CRange& range)
{
	m_maxValue = range.m_maxValue;
	m_minValue = range.m_minValue;

	return *this;
}


} // namespace istd
