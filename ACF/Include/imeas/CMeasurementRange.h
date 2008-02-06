#ifndef CMeasurementRange_h_included
#define CMeasurementRange_h_included


#include "imeas/imeas.h"


#include "istd/IPolymorphic.h"


namespace imeas
{


/**
	\ingroup imeas

	Implementation of a measurement range.
*/
class CMeasurementRange: virtual public istd::IPolymorphic
{
public:
	CMeasurementRange();
};


} // namespace imeas


#endif // !CMeasurementRange_h_included
