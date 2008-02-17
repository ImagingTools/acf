#include "inemo/CNemoSensorSpecification.h"


namespace inemo
{


CNemoSensorSpecification::CNemoSensorSpecification()
{

}


void CNemoSensorSpecification::SetRange(const istd::CRange& range)
{
	m_range = range;
}


void CNemoSensorSpecification::SetUnit(const ::std::string& unit)
{
	m_unit = unit;
}


void CNemoSensorSpecification::SetType(int type)
{
	m_type = type;
}


// reimplemented (inemo::INemoSensorSpecification)

istd::CRange CNemoSensorSpecification::GetRange() const
{
	return m_range;
}

::std::string CNemoSensorSpecification::GetUnit() const
{
	return m_unit;
}


int CNemoSensorSpecification::GetType() const
{
	return m_type;
}


} // namespace inemo
