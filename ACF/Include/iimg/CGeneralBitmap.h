#ifndef iimg_CGeneralBitmap_included
#define iimg_CGeneralBitmap_included


#include "istd/TOptDelPtr.h"

#include "iimg/CBitmapBase.h"


namespace iimg
{


class CGeneralBitmap: public CBitmapBase
{
public:
	CGeneralBitmap();

	// reimplemented (iimg::IBitmap)
	virtual bool CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount = 8, int componentsCount = 1);
	virtual bool CreateBitmap(const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0, int pixelBitsCount = 8, int componentsCount = 1);
	virtual int GetLinesDifference() const;
	virtual int GetPixelBitsCount() const;
	virtual const void* GetLinePtr(int positionY) const;
	virtual void* GetLinePtr(int positionY);

	// reimplemented (iimg::IRasterImage)
	virtual void ResetImage();
	virtual istd::CIndex2d GetImageSize() const;
	virtual int GetComponentsCount() const;
	virtual bool CopyImageFrom(const IRasterImage& image);

private:
	istd::TOptDelPtr<I_BYTE, true> m_buffer;

	istd::CIndex2d m_size;
	int m_linesDifference;
	int m_pixelBitsCount;
	int m_componentsCount;
};


} // namespace iimg


#endif // !iimg_CGeneralBitmap_included


