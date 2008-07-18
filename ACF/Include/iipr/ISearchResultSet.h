#ifndef iipr_ISearchResultSet_included
#define iipr_ISearchResultSet_included


#include "iipr/iipr.h"


#include "iipr/IFeaturesConsumer.h"

#include "i2d/CTransform.h"
#include "i2d/CVector2d.h"


namespace iipr
{


/**	
	Interface for search results container.
*/
class ISearchResultSet: virtual public iipr::IFeaturesConsumer
{
public:
	/*
		Returns the time need for search in ms.
	*/
	virtual double GetTime() const = 0;
};


} // namespace iipr


#endif // !iipr_ISearchResultSet_included

