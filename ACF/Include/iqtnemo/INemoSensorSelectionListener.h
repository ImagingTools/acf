#ifndef iqtnemo_INemoSensorSelectionListener_included
#define iqtnemo_INemoSensorSelectionListener_included


#include "iqtnemo/iqtnemo.h"

#include "Model/ModelInterface.h"

#include "Base/NamedInterface.h"

#include "inemo/INemoSensor.h"


namespace iqtnemo
{


/**
	\ingroup iqtnemo

	Interface for selection of a NEMO sensor.
*/
class INemoSensorSelectionListener: virtual public acf::PolymorphicInterface
{
public:
	/**
		This function will be called, if the sensor \c selectedSensorPtr was selected or with \c NULL
		if no sensor is currently selected.
	*/
	virtual void OnSensorSelected(inemo::INemoSensor* selectedSensorPtr) = 0;
};


} // namespace inemo


#endif // !iqtnemo_INemoSensorSelectionListener_included
