#ifndef iqt_CBitmap_included
#define iqt_CBitmap_included


// Qt includes
#include <QImage>

#include "istd/TOptDelPtr.h"

#include "iimg/CBitmapBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Bitmap implementation based on Qt QImage.
*/
class CBitmap: public iimg::CBitmapBase
{
public:
	typedef iimg::CBitmapBase BaseClass;

	CBitmap();
	CBitmap(const CBitmap& bitmap);

	virtual const QImage& GetQImage() const;
	virtual bool CopyImageFrom(const QImage& image);

	// reimplemented (iimg::IBitmap)
	virtual bool CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount = 8, int componentsCount = 1);
	virtual bool CreateBitmap(const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0, int pixelBitsCount = 8, int componentsCount = 1);
	virtual int GetLinesDifference() const;
	virtual int GetPixelBitsCount() const;
	virtual const void* GetLinePtr(int positionY) const;
	virtual void* GetLinePtr(int positionY);

	// reimplemented (iimg::IRasterImage)
	virtual istd::CIndex2d GetImageSize() const;
	virtual int GetComponentsCount() const;
	virtual bool CopyImageFrom(const IRasterImage& image);

protected:
	QImage::Format CalcQtFormat(int pixelBitsCount, int componentsCount) const;
	bool SetQImage(const QImage& image);

	virtual QImage& GetQImageRef();

private:
	istd::TOptDelPtr<I_BYTE, true> m_externalBuffer;
	QImage m_image;
};


} // namespace iqt


#endif iqt_CBitmap_included


