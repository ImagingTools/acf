#include "imeas\CMeasurementRange.h"


namespace imeas
{


CMeasurementRange::CMeasurementRange()
{
	m_lowerErrorLimit = 0.0;
	m_upperErrorLimit = 0.0;
	m_lowerWarningLimit = 0.0;
	m_upperWarningLimit = 0.0;
}


bool CMeasurementRange::IsValid() const
{
	bool retVal = m_lowerErrorLimit <= m_upperErrorLimit;
	retVal = retVal && (m_lowerWarningLimit <= m_upperWarningLimit);
	retVal = retVal && (m_upperWarningLimit <= m_upperErrorLimit);
	retVal = retVal && (m_lowerWarningLimit >= m_lowerErrorLimit);

	return retVal;
}


void CMeasurementRange::SetLowerErrorLimit(double lowerErrorLimit)
{
	m_lowerErrorLimit = lowerErrorLimit;
}


void CMeasurementRange::SetUpperErrorLimit(double upperErrorLimit)
{
	m_upperErrorLimit = upperErrorLimit;
}


void CMeasurementRange::SetLowerWarningLimit(double lowerWarningLimit)
{
	m_lowerWarningLimit = lowerWarningLimit;
}


void CMeasurementRange::SetUpperWarningLimit(double upperWarningLimit)
{
	m_upperWarningLimit = upperWarningLimit;
}


double CMeasurementRange::GetLowerErrorLimit() const
{
	return m_lowerErrorLimit;
}


double CMeasurementRange::GetUpperErrorLimit() const
{
	return m_upperErrorLimit;
}


double CMeasurementRange::GetLowerWarningLimit() const
{
	return m_lowerWarningLimit;
}


double CMeasurementRange::GetUpperWarningLimit() const
{
	return m_upperWarningLimit;
}


bool CMeasurementRange::IsOk(double value) const
{
	I_ASSERT(IsValid());

	return ((value - m_lowerWarningLimit) > I_EPSYLON) && ((m_upperWarningLimit - value) > I_EPSYLON);
}


bool CMeasurementRange::IsWarning(double value) const
{
	I_ASSERT(IsValid());

	return !IsOk(value) && !IsError(value);
}


bool CMeasurementRange::IsError(double value) const
{
	I_ASSERT(IsValid());

	return ((value - m_lowerErrorLimit) < I_EPSYLON) && ((value - m_upperErrorLimit) > I_EPSYLON);
}


} // namespace imeas
