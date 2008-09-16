#ifndef iimg_IBitmap_included
#define iimg_IBitmap_included


#include "iimg/IRasterImage.h"


namespace iimg
{


/**
	Definition of single plane bitmap.
*/
class IBitmap: virtual public IRasterImage
{
public:
	/**
		Create bitmap with specified size, number of bits per pixel and components number per pixel.
		\param	size			bitmap size.
		\param	pixelBitsCount	number of bits per single pixel.
		\param	componentsCount	number of components per single pixel.
		\return					true if bitmap was created.
	*/
	virtual bool CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount = 8, int componentsCount = 1) = 0;

	/**
		Create bitmap using external image buffer.
		\param	size			bitmap size.
		\param	dataPtr			pointer to external image buffer.
		\param	releaseFlag		if its true, external buffer will be managed (removed) by this object.
		\param	linesDifference	address difference between next and previos line. If it equals 0, the value will be taken from size and number of bits per pixel.
		\param	pixelBitsCount	number of bits per single pixel.
		\param	componentsCount	number of components per single pixel.
		\return					true if bitmap was created.
	*/
	virtual bool CreateBitmap(const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0, int pixelBitsCount = 8, int componentsCount = 1) = 0;

	/**
		Number of bytes per single line.
	*/
	virtual int GetLineBytesCount() const = 0;

	/**
		Get address difference between next and previos line.
	*/
	virtual int GetLinesDifference() const = 0;

	/**
		Get number of bits per single pixel component.
	*/
	virtual int GetComponentBitsCount(int componentIndex = 0) const = 0;

	/**
		Get total number of bits per single pixel.
	*/
	virtual int GetPixelBitsCount() const = 0;

	/**
		Get pointer to buffer for single line.
		\param	positionY	position Y. It must be smaller than bitmap size Y component.
	*/
	virtual const void* GetLinePtr(int positionY) const = 0;

	/**
		Get pointer to buffer for single line.
		\param	positionY	position Y. It must be smaller than bitmap size Y component.
	*/
	virtual void* GetLinePtr(int positionY) = 0;
};


} // namespace iimg


#endif // !iimg_IBitmap_included


