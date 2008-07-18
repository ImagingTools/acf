#ifndef iipr_ISearchProcessor_included
#define iipr_ISearchProcessor_included


#include "iimg/IBitmap.h"

#include "iproc/TIProcessor.h"

#include "iipr/IFeaturesConsumer.h"


namespace iipr
{


/**
	Interface for the shape based pattern matching processor.
*/
class ISearchProcessor: virtual public iproc::TIProcessor<iimg::IBitmap, iipr::IFeaturesConsumer>
{
};


} // namespace iipr


#endif // !iipr_ISearchProcessor_included

