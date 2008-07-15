#ifndef iipr_IFeaturesConsumer_included
#define iipr_IFeaturesConsumer_included


#include "istd/IPolymorphic.h"

#include "iipr/IFeature.h"


namespace iipr
{


/**
	Common interface for a features container consuming feature objects.
*/
class IFeaturesConsumer: public istd::IPolymorphic
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
		\return	true if added feature fullfill search constraints and no additionally features should be added.
	*/
	virtual bool AddFeature(const IFeature* featurePtr) = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesConsumer_included


