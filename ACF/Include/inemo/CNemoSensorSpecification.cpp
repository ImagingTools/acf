#include "inemo/CNemoSensorSpecification.h"


namespace inemo
{


CNemoSensorSpecification::CNemoSensorSpecification()
{

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
