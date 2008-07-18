#ifndef iipr_IFeaturesContainer_included
#define iipr_IFeaturesContainer_included


#include <vector>


#include "iipr/IFeaturesConsumer.h"


namespace iipr
{


/**
	Common interface for a features container.
*/
class IFeaturesContainer: virtual public iipr::IFeaturesConsumer
{
public:
	typedef std::vector<const iipr::IFeature*> Features;

	/**
		Get container features.
	*/
	virtual Features GetFeatures() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesContainer_included


