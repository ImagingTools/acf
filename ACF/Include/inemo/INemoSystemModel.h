#ifndef inemo_INemoSystemModel_included
#define inemo_INemoSystemModel_included


#include "inemo/inemo.h"

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
class INemoSystemModel: virtual public acf::PolymorphicInterface
{
public:
	virtual int GetSensorCount() const = 0;
	virtual inemo::INemoSensor& GetSensor(int index) const = 0;
};


} // namespace inemo


#endif // !inemo_INemoSystemModel_included
