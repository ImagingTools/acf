#ifndef iipr_TFeaturesContainerWrap_included
#define iipr_TFeaturesContainerWrap_included


#include "istd/TPointerVector.h"
#include "istd/TChangeNotifier.h"

#include "iipr/IFeaturesContainer.h"


namespace iipr
{


/**
	Basic implementation of a features container.
*/
template <class BaseClass, class FeatureInterface>
class TFeaturesContainerWrap: public BaseClass
{
public:
	// pseudo-reimplemented (iipr::IFeaturesConsumer)
	virtual void ResetFeatures();
	virtual bool AddFeature(const iipr::IFeature* featurePtr);

	// pseudo-reimplemented (iipr::IFeaturesContainer)
	virtual iipr::IFeaturesContainer::Features GetFeatures() const;

protected:
	/**
		Return \c true if the feature is accepted by the concrete container implementation. 
	*/
	virtual bool IsFeatureAccepted(const iipr::IFeature* featurePtr) const;

private:
	typedef istd::TPointerVector<iipr::IFeature> FeaturesList;

	FeaturesList m_featuresList;
};


// pseudo-reimplemented (iipr::IFeaturesConsumer)

template <class BaseClass, class FeatureInterface>
void TFeaturesContainerWrap<BaseClass, FeatureInterface>::ResetFeatures()
{
	istd::CChangeNotifier changePtr(this);

	m_featuresList.Reset();
}


template <class BaseClass, class FeatureInterface>
bool TFeaturesContainerWrap<BaseClass, FeatureInterface>::AddFeature(const iipr::IFeature* featurePtr)
{
	if (featurePtr != NULL && IsFeatureAccepted(featurePtr)){
		istd::CChangeNotifier changePtr(this);

		m_featuresList.PushBack(const_cast<iipr::IFeature*>(featurePtr));
		
		return true;
	}

	return false;
}


// pseudo-reimplemented (iipr::IFeaturesContainer)

template <class BaseClass, class FeatureInterface>
typename iipr::IFeaturesContainer::Features TFeaturesContainerWrap<BaseClass, FeatureInterface>::GetFeatures() const
{
	iipr::IFeaturesContainer::Features featuresList;
	
	for (int index = 0; index < m_featuresList.GetCount(); index++){
		featuresList.push_back(m_featuresList.GetAt(index));
	}

	return featuresList;
}


// protected methods

template <class BaseClass, class FeatureInterface>
bool TFeaturesContainerWrap<BaseClass, FeatureInterface>::IsFeatureAccepted(const iipr::IFeature* featurePtr) const
{
	I_ASSERT(featurePtr != NULL);

	return (dynamic_cast<const FeatureInterface*>(featurePtr) != NULL);
}


} // namespace iipr


#endif // !iipr_TFeaturesContainerWrap_included

