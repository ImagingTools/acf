#include "iqtnemo/CNemoSensorsModelComp.h"


#include <QSqlTableModel>


#include "QtAcf/QtAcf.h"


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

			// set measurement data:
			int sensorId = sensorRecord.value("sensor").toInt();
			QString measurementTableName = QString("nemo.sensor%1").arg(sensorId, 5, 10, QLatin1Char('0'));
			QSqlTableModel measurementTable(this, m_tableModelPtr->database());
			measurementTable.setTable(measurementTableName);
			if (measurementTable.select()){
				acf::Sequence sequence;
				for (int row = 0; row < measurementTable.rowCount(); row++){
					QSqlRecord rowRecord = measurementTable.record(row);
					float x = row;
					float y = rowRecord.value("value").toDouble();
					sequence.addPoint(x, y);
				}

				sensor->SetMeasurementData(sequence);
			}

			m_sensors.push_back(sensor);
		}
	}
}


} // namespace iqtnemo
