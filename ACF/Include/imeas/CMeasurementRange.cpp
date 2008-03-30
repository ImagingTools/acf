#include "imeas\CMeasurementRange.h"


namespace imeas
{


CMeasurementRange::CMeasurementRange()
{
}


bool CMeasurementRange::IsValid() const
{
	return (m_warningRange.IsValid() && m_errorRange.IsValid()) && (!m_warningRange.IsEmpty() && !m_errorRange.IsEmpty());
}


void CMeasurementRange::SetLowerErrorLimit(double lowerErrorLimit)
{
	m_errorRange.SetMinValue(lowerErrorLimit);
}


void CMeasurementRange::SetUpperErrorLimit(double upperErrorLimit)
{
	m_errorRange.SetMaxValue(upperErrorLimit);
}


void CMeasurementRange::SetLowerWarningLimit(double lowerWarningLimit)
{
	m_warningRange.SetMinValue(lowerWarningLimit);
}


void CMeasurementRange::SetUpperWarningLimit(double upperWarningLimit)
{
	m_warningRange.SetMaxValue(upperWarningLimit);
}


double CMeasurementRange::GetLowerErrorLimit() const
{
	return m_errorRange.GetMinValue();
}


double CMeasurementRange::GetUpperErrorLimit() const
{
	return m_errorRange.GetMaxValue();
}


double CMeasurementRange::GetLowerWarningLimit() const
{
	return m_warningRange.GetMinValue();
}


double CMeasurementRange::GetUpperWarningLimit() const
{
	return m_warningRange.GetMaxValue();
}


bool CMeasurementRange::IsOk(double value) const
{
	I_ASSERT(IsValid());

	return m_warningRange.IsInside(value) && m_errorRange.IsInside(value);
}


bool CMeasurementRange::IsWarning(double value) const
{
	I_ASSERT(IsValid());

	return m_warningRange.IsInside(value);
}


bool CMeasurementRange::IsError(double value) const
{
	I_ASSERT(IsValid());

	return (!IsOk(value) && !IsWarning(value));
}


bool CMeasurementRange::operator == (const CMeasurementRange& otherRange) const
{
	return (m_warningRange == otherRange.m_warningRange) && (m_errorRange == otherRange.m_errorRange);
}


bool CMeasurementRange::operator != (const CMeasurementRange& otherRange) const
{
	return !operator == (otherRange);
}



} // namespace imeas


