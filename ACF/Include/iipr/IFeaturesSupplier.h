#ifndef iipr_IFeaturesSupplier_included
#define iipr_IFeaturesSupplier_included


#include "iproc/ISupplier.h"

#include "iipr/IFeaturesContaner.h"


namespace iipr
{


class IFeaturesSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced features container.
		\param	objectId	ID of inspected object. \sa	iproc::ISupplier::BeginNextObject.
		\return	pointer to the feature container if this was correctly produced, or NULL.
	*/
	virtual const iipr::IFeaturesContainer* GetFeatures(I_DWORD objectId) const = 0;
};


} // namespace iipr


#endif // !iipr_IFeaturesSupplier_included

