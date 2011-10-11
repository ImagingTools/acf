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
		Bitmap pixel format description.
	*/
	enum PixelFormat
	{
		/**
			Unknown image format
		*/
		PF_UNKNOWN = 0,

		/**
			Monochrome bitmap
		*/
		PF_MONO,

		/**
			8-bit grayscale bitmap.
		*/
		PF_GRAY,

		/**
			32-bit RGB-bitmap, alpha channel can be ignored.
		*/
		PF_RGB,

		/**
			32-bit RGB bitmap with alpha channel.
		*/
		PF_RGBA,
		/**
			16-bit grayscale bitmap.
		*/
		PF_GRAY16,
		/**
			32-bit grayscale bitmap.
		*/
		PF_GRAY32,
		/**
			First user defined pixel format.
		*/
		PF_USER = 256
	};


	/**
		Get the bitmap's pixel format.
		\sa PixelFormat
	*/
	virtual int GetPixelFormat() const = 0;

	/**
		Create bitmap with specified size and format.
		\param	pixelFormat		bitmap format. \sa PixelFormat
		\param	size			bitmap size.
	*/
	virtual bool CreateBitmap(int pixelFormat, const istd::CIndex2d& size) = 0;

	/**
		Create bitmap with specified size and format using external image data buffer.
		\param	pixelFormat		bitmap format. \sa PixelFormat
		\param	size			bitmap size.
		\param	dataPtr			pointer to external image buffer.
		\param	releaseFlag		if its true, external buffer will be managed (removed) by this object.
		\param	linesDifference	address difference between next and previos line. If it equals 0, the value will be taken from size and number of bits per pixel.
	*/
	virtual bool CreateBitmap(int pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0) = 0;

	/**
		Number of bytes per single line.
	*/
	virtual int GetLineBytesCount() const = 0;

	/**
		Get address difference between next and previous line.
	*/
	virtual int GetLinesDifference() const = 0;

	/**
		Get address difference between next and previous pixel.
	*/
	virtual int GetPixelsDifference() const = 0;

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


