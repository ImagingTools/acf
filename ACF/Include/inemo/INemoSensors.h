#ifndef INemoSensors_h_included
#define INemoSensors_h_included


#include "inemo/inemo.h"

#include "Model/ModelInterface.h"

#include "Base/NamedInterface.h"


namespace inemo
{


class INemoSensor;


/**
	\ingroup inemo
*/
class INemoSensors: virtual public acf::ModelInterface
{
public:
	/**
	* Returns the number of sensors in the database.
	*/
	virtual int GetSensorCount() const = 0;

	/**
	* Returns the sensor with index \c sensorIndex.
	*/
	virtual inemo::INemoSensor& GetSensor(int sensorIndex) const = 0;
};


} // namespace inemo


#endif // !INemoSensors_h_included
