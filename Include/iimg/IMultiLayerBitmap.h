#ifndef iimg_IMultiLayerBitmap_included
#define iimg_IMultiLayerBitmap_included


// ACF includes
#include "iimg/IMultiBitmapProvider.h"


namespace iimg
{


class IMultiLayerBitmap: public iimg::IMultiBitmapProvider
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


#endif // iimg_IMultiLayerBitmap_included

