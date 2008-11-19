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


void CRange::Reset()
{
	m_minValue = 0.0;
	m_maxValue  = 0.0;
}


bool CRange::IsIntersectedBy(const CRange& range) const
{
	return (m_minValue < range.m_maxValue) || (m_maxValue > range.m_minValue);
}


CRange CRange::GetIntersection(const CRange& range) const
{
	if (IsEmpty()){
		return *this;
	}
	else{
		return CRange(Max(m_minValue, range.m_minValue), Min(m_maxValue, range.m_maxValue));
	}
}


void CRange::Intersection(const CRange& range)
{
	if (!IsEmpty()){
		if (range.m_minValue > m_minValue){
			m_minValue = range.m_minValue;
		}

		if (range.m_maxValue < m_maxValue){
			m_maxValue = range.m_maxValue;
		}
	}
}


CRange CRange::GetUnion(const CRange& range) const
{
	if (range.IsEmpty()){
		return *this;
	}
	else{
		if (IsEmpty()){
			return range;
		}
		else{
			return CRange(Min(m_minValue, range.m_minValue), Max(m_maxValue, range.m_maxValue));
		}
	}
}


void CRange::Union(const CRange& range)
{
	if (!range.IsEmpty()){
		if (IsEmpty()){
			*this = range;
		}
		else{
			if (range.m_minValue < m_minValue){
				m_minValue = range.m_minValue;
			}

			if (range.m_maxValue > m_maxValue){
				m_maxValue = range.m_maxValue;
			}
		}
	}
}


CRange CRange::GetExpanded(const CRange& range) const
{
	return CRange(m_minValue + range.m_minValue, m_maxValue + range.m_maxValue);
}


void CRange::Expand(const CRange& range)
{
	m_minValue += range.m_minValue;
	m_maxValue += range.m_maxValue;
}


double CRange::GetNearestInside(double value) const
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


double CRange::GetMappedTo(double value, const istd::CRange& range) const
{
	return range.GetMinValue() + (value - GetMinValue()) * (range.GetLength() / GetLength());
}



CRange& CRange::operator=(const CRange& range)
{
	m_maxValue = range.m_maxValue;
	m_minValue = range.m_minValue;

	return *this;
}


} // namespace istd
