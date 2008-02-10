#ifndef inemo_INemoSensorSpecification_included
#define inemo_INemoSensorSpecification_included


#include "inemo/inemo.h"

#include "Base/PolymorphicInterface.h"
#include "Base/WideString.h"

#include "istd/CRange.h"


namespace inemo
{


/**
	\ingroup inemo

	This interface provides access to the technical specification of a sensor model.
*/
class INemoSensorSpecification: virtual public acf::PolymorphicInterface
{
public:
	/**
		Returns the measurement range of the sensor.
	*/
	virtual istd::CRange GetRange() const = 0;

	/**
		Returns the measurement unit of the sensor.
	*/
	virtual std::string GetUnit() const = 0;

	/**
		Returns the measurement type of the sensor.
	*/
	virtual int GetType() const = 0;
};


} // namespace inemo


#endif // !inemo_INemoSensorSpecification_included
