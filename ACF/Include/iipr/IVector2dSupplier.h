#ifndef iipr_IVector2dSupplier_included
#define iipr_IVector2dSupplier_included


#include "i2d/CVector2d.h"

#include "iproc/ISupplier.h"

#include "iipr/iipr.h"


namespace iipr
{


/**
	Bitmap acquisition processor.
	This processor use as \c iproc::TIProcessor template parameter Input optional isys::ITimer object to indicate trigger time of acquision.
*/
class IVector2dSupplier: virtual public iproc::ISupplier
{
public:
	/**
		Get access to produced vector object.
		\param	objectId	ID of inspected object. \sa	iproc::ISupplier::BeginNextObject.
		\return				if vector is accessible pointer to its instance, or NULL.
	*/
	virtual const i2d::CVector2d* GetVector2d(I_DWORD objectId) const = 0;
};


} // namespace iipr


#endif // !iipr_IVector2dSupplier_included

