#ifndef iqtnemo_CNemoSensorsModelComp_h_included
#define iqtnemo_CNemoSensorsModelComp_h_included


#include "iqtnemo.h"


#include <set>


#include <QObject>
#include <QtSql>


#include "Comp/Component.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "Model/ModelTemplate.h"

#include "idb/IDatabaseConnector.h"

#include "inemo/INemoSensors.h"
#include "inemo/CNemoSensor.h"

#include "iqtdb/CTableModelCompBase.h"


namespace iqtnemo
{


/**
	Implementation of the model wrapper for the SENSORS table.
*/
class CNemoSensorsModelComp: public iqtdb::CTableModelCompBase, public inemo::INemoSensors
{
public:
	typedef iqtdb::CTableModelCompBase BaseClass;

	CNemoSensorsModelComp();

	// reimplemented (acf::INemoSensors)
	virtual int GetSensorCount() const;
	virtual inemo::INemoSensor& GetSensor(int sensorIndex) const;

protected:
	// reimplemented (iqtdb::CTableModelCompBase)
	virtual bool IsModelChanged() const;
	virtual void SynchronizeModelWithTable();

private:
	typedef acf::PointerVector<inemo::CNemoSensor> Sensors;

	Sensors m_sensors;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoSensorsModelComp_h_included

