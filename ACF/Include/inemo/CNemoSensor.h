#ifndef inemo_CNemoSensor_included
#define inemo_CNemoSensor_included


#include "inemo/inemo.h"
#include "inemo/INemoSensor.h"

#include "Model/ModelTemplate.h"

#include "Base/NamedTemplate.h"
#include "Base/Sequence.h"

#include "inemo/CSensorSpecification.h"


namespace inemo
{


/**
	\ingroup inemo

	Common implementation of a NEMO sensor.
	Sensors are created and managed by system model.
	\sa inemo::INemoSystemModel
*/
class CNemoSensor: public acf::ModelTemplate<acf::NamedTemplate<inemo::INemoSensor> >
{
public:
	CNemoSensor();

	void SetMeasurementData(const acf::Sequence& sequence);
	virtual void SetSensorSpecification(const CSensorSpecification& sensorSpecification);
	virtual void SetMeasurementRange(const imeas::CMeasurementRange& measurementRange);
	virtual void SetState(int state);
	virtual void SetPredictedState(int predictedState);
	virtual void SetFirstLevelLocation();
	virtual void SetSecondLevelLocation();
	virtual void SetThirdLevelLocation();
	virtual void SetFourthLevelLocation();

	// reimplemented (inemo::INemoSensor)
	const acf::SequenceInterface& CNemoSensor::GetMeasurementData() const;
	virtual const CSensorSpecification& GetSensorSpecification() const;
	virtual const imeas::CMeasurementRange& GetMeasurementRange() const;
	virtual int GetState() const;
	virtual int GetPredictedState() const;
	virtual std::string GetFirstLevelLocation() const;
	virtual std::string GetSecondLevelLocation() const;
	virtual std::string GetThirdLevelLocation() const;
	virtual std::string GetFourthLevelLocation() const;

protected:
	acf::Sequence m_measurementData;
	CSensorSpecification m_specification;
	imeas::CMeasurementRange  m_range;
	int m_state;
	int m_predictedState;
};


} // namespace inemo


#endif // !inemo_CNemoSensor_included
