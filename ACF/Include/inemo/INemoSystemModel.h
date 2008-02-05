#ifndef INemoSystemModel_h_included
#define INemoSystemModel_h_included


#include "inemo/inemo.h"

#include "Model/ModelInterface.h"

#include "Persist/SerializableInterface.h"

#include "Base/NamedInterface.h"


namespace inemo
{


class INemoSensor;

/**
	\ingroup inemo

	INemoSystemModel is a high level interface to access the data in the NEMO database.	
	The system model reflects all changes in the nemo database.
	All NEMO view components are direct or indirect observers of this model.
*/
class INemoSystemModel: virtual public acf::SerializableInterface, virtual public acf::ModelInterface
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


#endif // !INemoSystemModel_h_included
