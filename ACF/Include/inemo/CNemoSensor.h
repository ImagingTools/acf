#ifndef inemo_CNemoSensor_included
#define inemo_CNemoSensor_included


#include "inemo/inemo.h"


#include "Base/NamedTemplate.h"
#include "Base/Sequence.h"

#include "Model/ModelTemplate.h"

#include "imeas/CMeasurementRange.h"

#include "inemo/INemoSensor.h"
#include "inemo/CNemoSensorSpecification.h"
#include "inemo/CNemoSensorInfo.h"
#include "inemo/CNemoSensorData.h"


namespace inemo
{


/**
	\ingroup inemo

	Common implementation of a NEMO sensor.
	Sensors are created and managed by system model.
	\sa inemo::INemoSystemModel
*/
class CNemoSensor: public acf::NamedTemplate<inemo::INemoSensor>
{
public:
	CNemoSensor();

	virtual void SetSpecification(const inemo::CNemoSensorSpecification& specification);
	virtual void SetInfo(const inemo::CNemoSensorInfo& info);
	virtual void SetData(const inemo::CNemoSensorData& data);
	virtual void SetMeasurementRange(const imeas::CMeasurementRange& measurementRange);
	virtual void SetState(int state);
	virtual void SetPredictedState(int predictedState);

	// reimplemented (inemo::INemoSensor)
	virtual const inemo::INemoSensorSpecification& GetSpecification() const;
	virtual const inemo::INemoSensorInfo& GetInfo() const;
	virtual const inemo::INemoSensorData& GetData() const;
	virtual const imeas::CMeasurementRange& GetMeasurementRange() const;
	virtual int GetState() const;
	virtual int GetPredictedState() const;

protected:
	inemo::CNemoSensorSpecification m_specification;
	inemo::CNemoSensorInfo m_info;
	inemo::CNemoSensorData m_data;
	imeas::CMeasurementRange  m_measurementRange;
	int m_state;
	int m_predictedState;
};


} // namespace inemo


#endif // !inemo_CNemoSensor_included
