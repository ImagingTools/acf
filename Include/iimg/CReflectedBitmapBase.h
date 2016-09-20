#ifndef iimg_CReflectedBitmapBase_included
#define iimg_CReflectedBitmapBase_included


// ACF includes
#include <istd/TCachedUpdateManagerWrap.h>
#include <iimg/CGeneralBitmap.h>
#include <iimg/IQImageProvider.h>


namespace iimg
{


/**
	Implementation of bitmap storing internal additionaly QImage object reflecting state of main bitmap after conversion to Qt formats.
	It allows to working with Qt with other bitmap formats.
*/
class CReflectedBitmapBase:
			public istd::TCachedUpdateManagerWrap<iimg::CGeneralBitmap>,
			virtual public IQImageProvider
{
public:
	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		CF_BLOCK_BITMAP_CONVERSION = 0x8e37293
	};

	// reimplemented (iimg::IQImageProvider)
	virtual const QImage& GetQImage() const;
	virtual bool CopyImageFrom(const QImage& image);

protected:
	virtual bool ConvertFromQImage(const QImage& image) = 0;
	virtual bool ConvertToQImage(QImage& result) const = 0;

	// reimplmented (istd::TCachedUpdateManagerWrap)
	virtual bool CalculateCache(const ChangeSet& changeSet);

private:
	QImage m_image;
};


} // namespace iimg


#endif // !iimg_CReflectedBitmapBase_included


