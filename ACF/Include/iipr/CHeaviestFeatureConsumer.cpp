#include "iipr/CHeaviestFeatureConsumer.h"


namespace iipr
{


const IFeature* CHeaviestFeatureConsumer::GetFeature() const
{
	return m_featurePtr.GetPtr();
}


// reimplemented (iipr::IFeaturesConsumer)

void CHeaviestFeatureConsumer::ResetFeatures()
{
	m_featurePtr.Reset();
}


bool CHeaviestFeatureConsumer::AddFeature(const IFeature* featurePtr, bool* isFullPtr)
{
	I_ASSERT(featurePtr != NULL);
	if (!m_featurePtr.IsValid() || (featurePtr->GetWeight() > m_featurePtr->GetWeight())){
		m_featurePtr.SetPtr(featurePtr);
	}
	else{
		delete featurePtr;
	}

	if (isFullPtr != NULL){
		*isFullPtr = false;
	}

	return true;
}


} // namespace iipr


