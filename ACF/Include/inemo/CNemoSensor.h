#ifndef CNemoSensor_h_included
#define CNemoSensor_h_included


#include "inemo/inemo.h"
#include "inemo/INemoSensor.h"

#include "Model/ModelTemplate.h"

#include "Base/NamedTemplate.h"


namespace inemo
{


/**
	\ingroup inemo

	Common implementation of a NEMO sensor.
*/
class CNemoSensor: public acf::ModelTemplate<acf::NamedTemplate<inemo::INemoSensor> >
{
public:
	CNemoSensor();
};


} // namespace inemo


#endif // !CNemoSensor_h_included
