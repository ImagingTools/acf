#ifndef iimg_IMultiPageBitmapController_included
#define iimg_IMultiPageBitmapController_included


// ACF includes
#include "istd/IPolymorphic.h"


namespace iimg
{


class IMultiPageBitmapController: virtual public istd::IPolymorphic
{
public:
	virtual void Reset() = 0;
	virtual iimg::IBitmap* InsertBitmap(
				iimg::IBitmap::PixelFormat pixelFormat,
				const istd::CIndex2d& size) = 0;
	virtual iimg::IBitmap* InsertBitmap(
				iimg::IBitmap::PixelFormat pixelFormat,
				const istd::CIndex2d& size,
				void* dataPtr,
				bool releaseFlag,
				int linesDifference = 0) = 0;
	virtual void RemoveBitmap(int index) = 0;
};


} // namespace iimg


#endif // iimg_IMultiPageBitmapController_included


