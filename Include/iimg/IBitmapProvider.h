#ifndef iimg_IBitmapProvider_included
#define iimg_IBitmapProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <iimg/IBitmap.h>


namespace iimg
{


/**
	Bitmap supplier allowing to access of produced bitmap.
*/
class IBitmapProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap() const = 0;
};


} // namespace iimg


#endif // !iimg_IBitmapProvider_included


