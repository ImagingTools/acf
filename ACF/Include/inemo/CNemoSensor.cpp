#include "inemo/CNemoSensor.h"


namespace inemo
{


CNemoSensor::CNemoSensor()
{
	m_state = StateUnknown;
}


void CNemoSensor::SetSpecification(const inemo::CNemoSensorSpecification& specification)
{
	m_specification = specification;
}


void CNemoSensor::SetInfo(const inemo::CNemoSensorInfo& info)
{
	m_info = info;
}


void CNemoSensor::SetData(const inemo::CNemoSensorData& data)
{
	m_data = data;
}


void CNemoSensor::SetMeasurementRange(const imeas::CMeasurementRange& measurementRange)
{
	m_measurementRange = measurementRange;
}


void CNemoSensor::SetState(int state)
{
	m_state = state;
}


void CNemoSensor::SetPredictedState(int predictedState)
{
	m_predictedState = predictedState;
}


// reimplemented (inemo::INemoSensor)

const INemoSensorSpecification& CNemoSensor::GetSpecification() const
{
	return m_specification;
}


const inemo::INemoSensorInfo& CNemoSensor::GetInfo() const
{
	return m_info;
}


const inemo::INemoSensorData& CNemoSensor::GetData() const
{
	return m_data;
}


const imeas::CMeasurementRange& CNemoSensor::GetMeasurementRange() const
{
	return m_measurementRange;
}


int CNemoSensor::GetState() const
{
	return m_state;
}


int CNemoSensor::GetPredictedState() const
{
	return m_predictedState;
}


} // namespace inemo
