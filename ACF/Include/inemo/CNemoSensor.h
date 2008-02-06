#ifndef CNemoSensor_h_included
#define CNemoSensor_h_included


#include "inemo/inemo.h"
#include "inemo/INemoSensor.h"

#include "Model/ModelTemplate.h"

#include "Base/NamedTemplate.h"

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

	virtual void SetSensorSpecification(const CSensorSpecification& sensorSpecification);
	virtual void SetMeasurementRange(const imeas::CMeasurementRange& measurementRange);
	virtual void SetState(int state);
	virtual void SetPredictedState(int predictedState);
	virtual void SetFirstLevelLocation();
	virtual void SetSecondLevelLocation();
	virtual void SetThirdLevelLocation();
	virtual void SetFourthLevelLocation();

	// reimplemented (inemo::INemoSensor)
	virtual const CSensorSpecification& GetSensorSpecification() const;
	virtual const imeas::CMeasurementRange& GetMeasurementRange() const;
	virtual int GetState() const;
	virtual int GetPredictedState() const;
	virtual std::string GetFirstLevelLocation() const;
	virtual std::string GetSecondLevelLocation() const;
	virtual std::string GetThirdLevelLocation() const;
	virtual std::string GetFourthLevelLocation() const;

protected:
	CSensorSpecification m_specification;
	imeas::CMeasurementRange  m_range;
	int m_state;
	int m_predictedState;
};


} // namespace inemo


#endif // !CNemoSensor_h_included