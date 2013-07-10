#ifndef iimg_CGeneralBitmap_included
#define iimg_CGeneralBitmap_included


#include "istd/TOptDelPtr.h"

#include "iimg/CBitmapBase.h"


namespace iimg
{


/**
	Standard device- and platform-independent bitmap definition.
*/
class CGeneralBitmap: public CBitmapBase
{
public:
	CGeneralBitmap();

	// reimplemented (iimg::IBitmap)
	virtual bool IsFormatSupported(PixelFormat pixelFormat) const;
	virtual PixelFormat GetPixelFormat() const;
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size);
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0);
	virtual int GetLinesDifference() const;
	virtual int GetPixelsDifference() const;
	virtual int GetPixelBitsCount() const;
	virtual const void* GetLinePtr(int positionY) const;
	virtual void* GetLinePtr(int positionY);

	// reimplemented (iimg::IRasterImage)
	virtual void ResetImage();
	virtual void ClearImage();
	virtual istd::CIndex2d GetImageSize() const;
	virtual int GetComponentsCount() const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

protected:
	/**
		Create bitmap with specified size, number of bits per pixel and components number per pixel.
		\param	size			bitmap size.
		\param	pixelBitsCount	number of bits per single pixel.
		\param	componentsCount	number of components per single pixel.
		\return					true if bitmap was created.
	*/
	virtual bool CreateBitmap(
				const istd::CIndex2d& size,
				int pixelBitsCount,
				int componentsCount,
				PixelFormat pixelFormat);

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
	virtual bool CreateBitmap(
				const istd::CIndex2d& size,
				void* dataPtr,
				bool releaseFlag,
				int linesDifference,
				int pixelBitsCount,
				int componentsCount,
				PixelFormat pixelFormat);

private:
	istd::TOptDelPtr<quint8, true> m_buffer;

	istd::CIndex2d m_size;
	int m_linesDifference;
	int m_pixelBitsCount;
	int m_componentsCount;
	PixelFormat m_pixelFormat;
};


} // namespace iimg


#endif // !iimg_CGeneralBitmap_included


