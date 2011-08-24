#include "istd/CRange.h"


// STL includes
#include <cmath>


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


CRange CRange::GetValidated() const
{
	return CRange(istd::Min(m_minValue, m_maxValue), istd::Max(m_minValue, m_maxValue));
}


void CRange::GetValidated(CRange& result) const
{
	result.SetMinValue(istd::Min(m_minValue, m_maxValue));
	result.SetMaxValue(istd::Max(m_minValue, m_maxValue));
}


void CRange::Validate()
{
	*this = CRange(istd::Min(m_minValue, m_maxValue), istd::Max(m_minValue, m_maxValue));
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
	if (range.IsValid()){
		if (IsValid()){
			return CRange(Min(m_minValue, range.m_minValue), Max(m_maxValue, range.m_maxValue));
		}
		else{
			return range;
		}
	}
	else{
		return *this;
	}
}


CRange CRange::GetUnion(double value) const
{
	if (IsValid()){
		return CRange(Min(m_minValue, value), Max(m_maxValue, value));
	}
	else{
		return CRange(value, value);
	}
}


void CRange::Unite(const CRange& range)
{
	if (range.IsValid()){
		if (IsValid()){
			if (range.m_minValue < m_minValue){
				m_minValue = range.m_minValue;
			}

			if (range.m_maxValue > m_maxValue){
				m_maxValue = range.m_maxValue;
			}
		}
		else{
			*this = range;
		}
	}
}


void CRange::Unite(double value)
{
	if (IsValid()){
		if (value < m_minValue){
			m_minValue = value;
		}

		if (value > m_maxValue){
			m_maxValue = value;
		}
	}
	else{
		m_minValue = value;
		m_maxValue = value;
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
		double distanceMin = std::fabs(value - m_minValue);
		double distanceMax = std::fabs(value - m_maxValue);

		return distanceMin < distanceMax ? m_minValue : m_maxValue;
	}

	return value;
}

	
istd::CRange CRange::GetClipped(const istd::CRange& range) const
{
	istd::CRange clippedRange = range;

	if (!Contains(range.GetMinValue())){
		clippedRange.SetMinValue(m_minValue);
	}
	
	if (!Contains(range.GetMaxValue())){
		clippedRange.SetMaxValue(m_maxValue);
	}

	return clippedRange;
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


// static methods

CRange CRange::GetValid(double value1, double value2)
{
	return CRange(istd::Min(value1, value2), istd::Max(value1, value2));
}


// static attributes

CRange CRange::s_null(0, 0);
CRange CRange::s_invalid(0, -1);


} // namespace istd
