#include "iimg/CMultiLayerBitmap.h"


// ACF includes
#include "imod/TModelWrap.h"
#include "iimg/CGeneralBitmap.h"

namespace iimg
{


// reimplemented (iimg::IMultiLayerBitmap)

iimg::IBitmap* CMultiLayerBitmap::InsertBitmap(
	iimg::IBitmap::PixelFormat pixelFormat, 
	const istd::CIndex2d& size)
{
	imod::TModelWrap<iimg::CGeneralBitmap>* resultPtr = new imod::TModelWrap<iimg::CGeneralBitmap>();
	if (!resultPtr->CreateBitmap(pixelFormat, size)){
		delete resultPtr;

		return NULL;
	}

	m_bitmaps.PushBack(resultPtr);

	return resultPtr;
}


iimg::IBitmap* CMultiLayerBitmap::InsertBitmap(
			iimg::IBitmap::PixelFormat pixelFormat, 
			const istd::CIndex2d& size, 
			void* dataPtr, 
			bool releaseFlag, 
			int linesDifference /*= 0*/)
{
	imod::TModelWrap<iimg::CGeneralBitmap>* resultPtr = new imod::TModelWrap<iimg::CGeneralBitmap>();
	if (!resultPtr->CreateBitmap(pixelFormat, size, dataPtr, releaseFlag, linesDifference)){
		delete resultPtr;

		return NULL;
	}

	m_bitmaps.PushBack(resultPtr);

	return resultPtr;
}


} // namespace iimg

