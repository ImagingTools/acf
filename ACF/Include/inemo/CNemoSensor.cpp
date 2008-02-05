#include "inemo/CNemoSensor.h"


namespace inemo
{


CNemoSensor::CNemoSensor(const inemo::INemoSystemModel* systemModelPtr)
{
	m_systemModelPtr = systemModelPtr;

	m_state = StateUnknown;
}


// reimplemented (inemo::INemoSensor)

const CSensorSpecification& CNemoSensor::GetSensorSpecification() const
{
	return m_specification;
}


const imeas::CMeasurementRange& CNemoSensor::GetMeasurementRange() const
{
	return m_range;
}

int CNemoSensor::GetState() const
{
	return m_state;
}


int CNemoSensor::GetPredictedState() const
{
	return m_predictedState;
}



std::string CNemoSensor::GetFirstLevelLocation() const
{
	return std::string();
}

std::string CNemoSensor::GetSecondLevelLocation() const
{
	return std::string();
}


std::string CNemoSensor::GetThirdLevelLocation() const
{
	return std::string();
}


std::string CNemoSensor::GetFourthLevelLocation() const
{
	return std::string();
}


} // namespace inemo
