#ifndef iipr_IFeaturesConsumer_included
#define iipr_IFeaturesConsumer_included


#include "istd/IChangeable.h"

#include "iipr/IFeature.h"


namespace iipr
{


/**
	Common interface for a features container consuming feature objects.
*/
class IFeaturesConsumer: virtual public istd::IChangeable
{
public:
	/**
		Remove all features from this container.
	*/
	virtual void ResetFeatures() = 0;

	/**
		Add new feature to the set of features.
		\param	featurePtr		instance of feature object.
								Please note, that the container takes controll over this object, 
								so you cannot destroy this object manually.
								It cannot be NULL.

		\param isFullPtr If not NULL this paramster is used to get info whether 
		the added feature satisfy search constraints and no additionally features should be added
		
		\return	true if the pushed feature is valid for this container and was inserted correctly.
	*/
	virtual bool AddFeature(const IFeature* featurePtr, bool* isFullPtr = NULL) = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesConsumer_included

