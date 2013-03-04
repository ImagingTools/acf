#ifndef iimg_IRasterImage_included
#define iimg_IRasterImage_included


// ACF includes
#include "istd/CIndex2d.h"
#include "i2d/IObject2d.h"
#include "icmm/CVarColor.h"


namespace iimg
{


/**
	General definition of image contains pixels in regular grid.
	All pixels are using the same number of color components.
*/
class IRasterImage: virtual public i2d::IObject2d
{
public:
	/**
		Return true if this image is empty.
	*/
	virtual bool IsEmpty() const = 0;

	/**
		Reset this image.
		After this operation image is set to initial state.
	*/
	virtual void ResetImage() = 0;

	/**
		Get size of this raster image.
	*/
	virtual istd::CIndex2d GetImageSize() const = 0;

	/**
		Get number of color components.
	*/
	virtual int GetComponentsCount() const = 0;

	/**
		Get color at specified pixel.
		\param	position	pixel position, must be inside of returned size.
	*/
	virtual icmm::CVarColor GetColorAt(const istd::CIndex2d& position) const = 0;

	/**
		Set color at specified pixel.
		\param	position	pixel position, must be inside of returned size.
		\param	color		color of pixel will be set.
		\return				true, if pixel color was changed.
	*/
	virtual bool SetColorAt(const istd::CIndex2d& position, const icmm::CVarColor& color) = 0;
};


} // namespace iimg


#endif // !iimg_IRasterImage_included


