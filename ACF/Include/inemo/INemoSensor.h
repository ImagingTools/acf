#ifndef INemoSensor_h_included
#define INemoSensor_h_included


#include "inemo/inemo.h"

#include "imeas/CMeasurementRange.h"

#include "Base/NamedInterface.h"


namespace inemo
{


class CSensorSpecification;

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
	virtual const CSensorSpecification& GetSensorSpecification() const = 0;

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

	/**
	* Returns the first level sensor location.
	*/
	virtual std::string GetFirstLevelLocation() const = 0;

	/**
	* Returns the second level sensor location.
	*/
	virtual std::string GetSecondLevelLocation() const = 0;

	/**
	* Returns the third level sensor location.
	*/
	virtual std::string GetThirdLevelLocation() const = 0;

	/**
	* Returns the fourth level sensor location.
	*/
	virtual std::string GetFourthLevelLocation() const = 0;
};


} // namespace inemo


#endif // !INemoSensor_h_included
