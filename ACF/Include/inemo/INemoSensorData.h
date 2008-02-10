#ifndef inemo_INemoSensorData_included
#define inemo_INemoSensorData_included


#include "inemo/inemo.h"


#include "Base/SequenceInterface.h"


namespace inemo
{


/**
	\ingroup inemo
*/
class INemoSensorData: virtual public acf::PolymorphicInterface
{
public:
	/**
		Returns the sensor measurement data.
	*/
	virtual const acf::SequenceInterface& GetMeasurementData() const = 0;

	/**
		Returns the predicted data using method A.
	*/
	virtual const acf::SequenceInterface& GetPredictedDataA() const = 0;

	/**
		Returns the predicted data using method B.
	*/
	virtual const acf::SequenceInterface& GetPredictedDataB() const = 0;
};


} // namespace inemo


#endif // !inemo_INemoSensorData_included
