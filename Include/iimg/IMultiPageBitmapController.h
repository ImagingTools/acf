#ifndef iimg_IMultiPageBitmapController_included
#define iimg_IMultiPageBitmapController_included


// ACF includes
#include <iimg/IMultiBitmapProvider.h>
#include <idoc/IDocumentMetaInfo.h>


namespace iimg
{


/**
	Interface of a controller for handling multi-page bitmaps.
*/
class IMultiPageBitmapController: virtual public IMultiBitmapProvider
{
public:
	/**
		Inserts a new bitmap.
		\param	pixelFormat	The pixel format.
		\param	size	Size of the page bitmap.
		\return	null if it fails, else a pointer to the inserted bitmap.
	*/
	virtual iimg::IBitmap* InsertBitmap(
				iimg::IBitmap::PixelFormat pixelFormat,
				const istd::CIndex2d& size,
				const idoc::IDocumentMetaInfo* metaInfoPtr = NULL) = 0;

	/**
		Inserts a new bitmap.
		
		\param	pixelFormat			The pixel format.
		\param	size				The size of the page bitmap.
		\param [in,out]	dataPtr		If non-null, the bitmap data pointer.
		\param	releaseFlag			Memory controlling flag.
		\param	linesDifference	(Optional) The distance between adjacent lines in bytes.
		
		\return	null if it fails, else a pointer to the inserted bitmap.
	*/
	virtual iimg::IBitmap* InsertBitmap(
				iimg::IBitmap::PixelFormat pixelFormat,
				const istd::CIndex2d& size,
				void* dataPtr,
				bool releaseFlag,
				int linesDifference = 0,
				const idoc::IDocumentMetaInfo* metaInfoPtr = NULL) = 0;

	/**
		Removes the bitmap at the given index.
		\param	index Index of the bitmap page.
	*/
	virtual void RemoveBitmap(int index) = 0;
};


} // namespace iimg


#endif // iimg_IMultiPageBitmapController_included


