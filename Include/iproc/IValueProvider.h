#ifndef iproc_IValueProvider_included
#define iproc_IValueProvider_included


// ACF includes
#include "istd/IChangeable.h"
#include "imath/CVarVector.h"


namespace iproc
{


/**
	Provide generic access to some data vector.
*/
class IValueProvider: virtual public istd::IChangeable
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
		\return				if data vector cannot be calculated it returns empty vector.
	*/
	virtual imath::CVarVector GetValue(int index = -1, int valueTypeId = VTI_AUTO) const = 0;
};


} // namespace iproc


#endif // !iproc_IValueProvider_included


