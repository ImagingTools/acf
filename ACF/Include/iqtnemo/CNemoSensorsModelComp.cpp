#include "QtAcf/QtAcf.h"

#include "iqtnemo/CNemoSensorsModelComp.h"


namespace iqtnemo
{


CNemoSensorsModelComp::CNemoSensorsModelComp()
	:BaseClass()
{
	registerInterface<inemo::INemoSensors>(this);
}


// reimplemented (acf::INemoSensors)

int CNemoSensorsModelComp::GetSensorCount() const
{
	return int(m_sensors.size());
}


inemo::INemoSensor& CNemoSensorsModelComp::GetSensor(int sensorIndex) const
{
	static inemo::CNemoSensor sensor;

	if (sensorIndex < GetSensorCount() && sensorIndex >= 0){
		return *m_sensors.at(sensorIndex);
	}

	return sensor; 
}


// protected methods

// reimplemented (iqtdb::CTableModelCompBase)
	
bool CNemoSensorsModelComp::IsModelChanged() const
{
	I_ASSERT(m_tableModelPtr != NULL);
	
	if (m_sensors.size() != m_tableModelPtr->rowCount()){
		return true;
	}
	else{
		
	}
	
	return false;
}


void CNemoSensorsModelComp::SynchronizeModelWithTable()
{
	if (m_tableModelPtr != NULL){
		m_sensors.reset();

		int rowCount = m_tableModelPtr->rowCount();
		for (int rowIndex = 0; rowIndex < rowCount; rowIndex++){
			QSqlRecord sensorRecord = m_tableModelPtr->record(rowIndex);

			inemo::CNemoSensor* sensor = new inemo::CNemoSensor;
			sensor->setName(acf::acfString(sensorRecord.value("name").toString()));

			m_sensors.push_back(sensor);
		}
	}
}


} // namespace iqtnemo
