#ifndef iimg_CGeneralBitmap_included
#define iimg_CGeneralBitmap_included

// ACF includes
#include <istd/TOptDelPtr.h>
#include <iimg/CBitmapBase.h>


namespace iimg
{


/**
	Standard device- and platform-independent bitmap definition.
*/
class CGeneralBitmap: public CBitmapBase
{
public:
	CGeneralBitmap();
	CGeneralBitmap(const CGeneralBitmap& bitmap);

	// reimplemented (iimg::IBitmap)
	virtual bool IsFormatSupported(PixelFormat pixelFormat) const;
	virtual PixelFormat GetPixelFormat() const;
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int pixelBitsCount = 0, int componentsCount = 0);
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0);
	virtual int GetLinesDifference() const;
	virtual int GetPixelsDifference() const;
	virtual const void* GetLinePtr(int positionY) const;
	virtual void* GetLinePtr(int positionY);

	// reimplemented (iimg::IRasterImage)
	virtual void ResetImage();
	virtual void ClearImage();
	virtual istd::CIndex2d GetImageSize() const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// operators
	CGeneralBitmap& operator=(const CGeneralBitmap& bitmap);
	bool operator==(const CGeneralBitmap& bitmap) const;
	bool operator!=(const CGeneralBitmap& bitmap) const;

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
	istd::TOptDelPtr<uint8_t, true> m_buffer;

	istd::CIndex2d m_size;
	int m_linesDifference;
	PixelFormat m_pixelFormat;
};


// inline methods

// reimplemented (iimg::IBitmap)

inline int CGeneralBitmap::GetLinesDifference() const
{
	return m_linesDifference;
}


inline int CGeneralBitmap::GetPixelsDifference() const
{
	return GetPixelBitsCount() >> 3;
}


inline const void* CGeneralBitmap::GetLinePtr(int positionY) const
{
	Q_ASSERT(positionY >= 0);
	Q_ASSERT(positionY < m_size.GetY());

	return m_buffer.GetPtr() + m_linesDifference * positionY;
}


inline void* CGeneralBitmap::GetLinePtr(int positionY)
{
	Q_ASSERT(positionY >= 0);
	Q_ASSERT(positionY < m_size.GetY());

	return m_buffer.GetPtr() + m_linesDifference * positionY;
}


// reimplemented (iimg::IRasterImage)

inline istd::CIndex2d CGeneralBitmap::GetImageSize() const
{
	return m_size;
}


} // namespace iimg


#endif // !iimg_CGeneralBitmap_included


