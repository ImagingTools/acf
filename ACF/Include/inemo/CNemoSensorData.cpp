#include "inemo/CNemoSensorData.h"


namespace inemo
{


CNemoSensorData::CNemoSensorData()
{

}


void CNemoSensorData::SetMeasurementData(const acf::Sequence& sequence)
{
	m_measuredData = sequence;
}


void CNemoSensorData::SetPredictedDataA(const acf::Sequence& sequence)
{
	m_predictedDataA = sequence;
}


void CNemoSensorData::SetPredictedDataB(const acf::Sequence& sequence)
{
	m_predictedDataB = sequence;
}



// reimplemented (inemo::INemoSensorData)

const acf::SequenceInterface& CNemoSensorData::GetMeasurementData() const
{
	return m_measuredData;
}


const acf::SequenceInterface& CNemoSensorData::GetPredictedDataA() const
{
	return m_predictedDataA;
}


const acf::SequenceInterface& CNemoSensorData::GetPredictedDataB() const
{
	return m_predictedDataB;
}


} // namespace inemo
