#ifndef iproc_IValueSupplier_included
#define iproc_IValueSupplier_included


#include "imath/CVarVector.h"

#include "iproc/ISupplier.h"


namespace iproc
{


/**
	2D vector or position supplier.
*/
class IValueSupplier: virtual public ISupplier
{
public:
	/**
		Define standard value types.
	*/
	enum ValueTypeId
	{
		/**
			Automatic value, implementation specific.
		*/
		VTI_AUTO = 0,
		/**
			Value of position (2D or 3D).
		*/
		VTI_POSITION,
		/**
			Value of radius.
		*/
		VTI_RADIUS,
		/**
			Quality factor.
			This value should be normalized to range [0, 1].
		*/
		VTI_QUALITY,
		/**
			Angle in radian.
		*/
		VTI_ANGLE,
		/**
			Length or distance.
		*/
		VTI_LENGTH,
	};

	/**
		Get access to produced vector object.
		\return				if vector is accessible pointer to its instance, or NULL.
	*/
	virtual imath::CVarVector GetValue(int index = -1, int valueTypeId = VTI_AUTO) const = 0;
};


} // namespace iproc


#endif // !iproc_IValueSupplier_included


