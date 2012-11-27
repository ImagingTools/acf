#include "imath/CGeneralUnitInfo.h"


// ACF includes
#include "imath/CDoubleManip.h"


namespace imath
{


CGeneralUnitInfo::CGeneralUnitInfo(
			int type,
			const QString& name,
			double displayMultFactor,
			const istd::CRange& range)
:	m_type(type),
	m_name(name),
	m_displayMultFactor(displayMultFactor),
	m_range(range)
{
}


void CGeneralUnitInfo::SetUnitType(int type)
{
	m_type = type;
}


void CGeneralUnitInfo::SetUnitName(const QString& name)
{
	m_name = name;
}


void CGeneralUnitInfo::SetDisplayMultiplicationFactor(double factor)
{
	m_displayMultFactor = factor;
}


void CGeneralUnitInfo::SetValueRange(const istd::CRange& range)
{
	m_range = range;
}


//	reimplemented (imath::IUnitInfo)

int CGeneralUnitInfo::GetUnitType() const
{
	return m_type;
}


QString CGeneralUnitInfo::GetUnitName() const
{
	return m_name;
}


double CGeneralUnitInfo::GetDisplayMultiplicationFactor() const
{
	return m_displayMultFactor;
}


istd::CRange CGeneralUnitInfo::GetValueRange() const
{
	return m_range;
}


const imath::IDoubleManip& CGeneralUnitInfo::GetValueManip() const
{
	static imath::CDoubleManip manip;

	return manip;
}


} // namespace imath


