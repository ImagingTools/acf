#include "imeas\CMeasurementRange.h"


namespace imeas
{


CMeasurementRange::CMeasurementRange()
{
}


bool CMeasurementRange::IsValid() const
{
	return m_warningRange.IsValid() && m_errorRange.IsValid();
}


void CMeasurementRange::SetLowerErrorLimit(double lowerErrorLimit)
{
	m_errorRange.SetBottomValue(lowerErrorLimit);
}


void CMeasurementRange::SetUpperErrorLimit(double upperErrorLimit)
{
	m_errorRange.SetTopValue(upperErrorLimit);
}


void CMeasurementRange::SetLowerWarningLimit(double lowerWarningLimit)
{
	m_warningRange.SetBottomValue(lowerWarningLimit);
}


void CMeasurementRange::SetUpperWarningLimit(double upperWarningLimit)
{
	m_warningRange.SetTopValue(upperWarningLimit);
}


double CMeasurementRange::GetLowerErrorLimit() const
{
	return m_errorRange.GetBottomValue();
}


double CMeasurementRange::GetUpperErrorLimit() const
{
	return m_errorRange.GetTopValue();
}


double CMeasurementRange::GetLowerWarningLimit() const
{
	return m_warningRange.GetBottomValue();
}


double CMeasurementRange::GetUpperWarningLimit() const
{
	return m_warningRange.GetTopValue();
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


} // namespace imeas
