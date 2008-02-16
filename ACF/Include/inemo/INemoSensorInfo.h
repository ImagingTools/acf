#ifndef inemo_INemoSensorInfo_included
#define inemo_INemoSensorInfo_included


#include "inemo/inemo.h"


#include <string>


#include "Base/PolymorphicInterface.h"


namespace inemo
{


/**
	\ingroup inemo

	This interface provides access to the some additional information about the sensor, 
	such as location, install time, alarm mail adresses and so on. 

*/
class INemoSensorInfo: virtual public acf::PolymorphicInterface
{
public:
	/**
	* Returns the first level sensor location.
	*/
	virtual ::std::string GetFirstLevelLocation() const = 0;

	/**
	* Returns the second level sensor location.
	*/
	virtual ::std::string GetSecondLevelLocation() const = 0;

	/**
	* Returns the third level sensor location.
	*/
	virtual ::std::string GetThirdLevelLocation() const = 0;

	/**
	* Returns the fourth level sensor location.
	*/
	virtual ::std::string GetFourthLevelLocation() const = 0;

};


} // namespace inemo


#endif // !inemo_INemoSensorInfo_included
