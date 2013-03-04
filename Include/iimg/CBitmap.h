#ifndef iimg_CBitmap_included
#define iimg_CBitmap_included


// ACF includes
#include "istd/TOptDelPtr.h"
#include "ibase/IObjectSnap.h"
#include "i2d/ICalibrationProvider.h"
#include "i2d/CAffineTransformation2d.h"
#include "iimg/CBitmapBase.h"
#include "iimg/IQImageProvider.h"


namespace iimg
{


/**
	Bitmap implementation based on Qt QImage.
*/
class CBitmap:
			public iimg::CBitmapBase,
			virtual public IQImageProvider,
			virtual public ibase::IObjectSnap
{
public:
	typedef iimg::CBitmapBase BaseClass;

	CBitmap();
	CBitmap(const CBitmap& bitmap);
	CBitmap(const QImage& image);

	QImage& GetQImageRef();

	// reimplemented (iimg::IQImageProvider)
	virtual const QImage& GetQImage() const;
	virtual bool CopyImageFrom(const QImage& image);

	// reimplemented (ibase::IObjectSnap)
	virtual bool GetSnap(
				const istd::IChangeable& data,
				iimg::IBitmap& objectSnap,
				const istd::CIndex2d& size) const;

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
	virtual istd::CIndex2d GetImageSize() const;
	virtual int GetComponentsCount() const;
	virtual icmm::CVarColor GetColorAt(const istd::CIndex2d& position) const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object);
	virtual istd::IChangeable* CloneMe() const;

protected:
	QImage::Format CalcQtFormat(PixelFormat pixelFormat) const;
	PixelFormat CalcFromQtFormat(QImage::Format imageFormat) const;
	bool SetQImage(const QImage& image);

private:
	istd::TOptDelPtr<quint8, true> m_externalBuffer;
	QImage m_image;
};


// inline methods

inline QImage& CBitmap::GetQImageRef()
{
	return m_image;
}


} // namespace iimg


#endif //!iimg_CBitmap_included


