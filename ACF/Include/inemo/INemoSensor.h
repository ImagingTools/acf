#ifndef inemo_INemoSensor_included
#define inemo_INemoSensor_included


#include "inemo/inemo.h"

#include "imeas/CMeasurementRange.h"

#include "Base/NamedInterface.h"
#include "Base/SequenceInterface.h"


namespace inemo
{


class INemoSensorSpecification;
class INemoSensorInfo;
class INemoSensorData;

/**
	\ingroup inemo

	Common interface of a NEMO sensor.
	This interface provides access to the meta data of the sensor.
*/
class INemoSensor: virtual public acf::NamedInterface
{
public:
	enum SensorState
	{
		StateOK = 0,
		StateWarning = 1,
		StateError = 2,
		StateUnknown = 3
	};

	/**
	* Returns the sensor specification data.
	*/
	virtual const inemo::INemoSensorSpecification& GetSpecification() const = 0;

	/**
	* Returns the additional information about the sensor.
	*/
	virtual const inemo::INemoSensorInfo& GetInfo() const = 0;

	/**
	* Returns the measurement data of the sensor.
	*/
	virtual const inemo::INemoSensorData& GetData() const = 0;

	/**
	* Returns the sensor measurment range.
	*/
	virtual const imeas::CMeasurementRange& GetMeasurementRange() const = 0;

	/**
	* Returns the current sensor state.
	*/
	virtual int GetState() const = 0;

	/**
	* Returns the current predicted sensor state.
	*/
	virtual int GetPredictedState() const = 0;
};


} // namespace inemo


#endif // !inemo_INemoSensor_included
