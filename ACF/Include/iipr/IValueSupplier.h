#ifndef iipr_IValueSupplier_included
#define iipr_IValueSupplier_included


#include "imath/TVarVector.h"

#include "iproc/ISupplier.h"

#include "iipr/iipr.h"


namespace iipr
{


/**
	2D vector or position supplier.
*/
class IValueSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced vector object.
		\param	objectId	ID of inspected object. \sa	iproc::ISupplier::BeginNextObject.
		\return				if vector is accessible pointer to its instance, or NULL.
	*/
	virtual imath::CVarVector GetValue(I_DWORD objectId) const = 0;
};


} // namespace iipr


#endif // !iipr_IValueSupplier_included


