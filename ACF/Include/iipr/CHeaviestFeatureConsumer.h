#ifndef iipr_CHeaviestFeatureConsumer_included
#define iipr_CHeaviestFeatureConsumer_included


#include "istd/TDelPtr.h"

#include "iipr/IFeaturesConsumer.h"


namespace iipr
{


/**
	Feature consumer implmentation storing the heaviest feature only.
*/
class CHeaviestFeatureConsumer: virtual public IFeaturesConsumer
{
public:
	const IFeature* GetFeature() const;

	// reimplemented (iipr::IFeaturesConsumer)
	virtual void ResetFeatures();
	virtual bool AddFeature(const IFeature* featurePtr, bool* isFullPtr = NULL);

private:
	istd::TDelPtr<const IFeature> m_featurePtr;
};


} // namespace iipr


#endif // !iipr_CHeaviestFeatureConsumer_included


