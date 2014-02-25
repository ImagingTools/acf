#ifndef iimg_CReflectedBitmapBase_included
#define iimg_CReflectedBitmapBase_included


// ACF includes
#include "istd/TCachedUpdateManagerWrap.h"
#include "iimg/CGeneralBitmap.h"
#include "iimg/IQImageProvider.h"


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
	enum ChangeFlags
	{
		CF_BLOCK_BITMAP_CONVERSION = 1 << 21
	};

	// reimplemented (iimg::IQImageProvider)
	virtual const QImage& GetQImage() const;
	virtual bool CopyImageFrom(const QImage& image);

protected:
	virtual bool ConvertFromQImage(const QImage& image) = 0;
	virtual bool ConvertToQImage(QImage& result) const = 0;

	// reimplmented (istd::TCachedUpdateManagerWrap)
	virtual bool CalculateCache(int changeFlags);

private:
	QImage m_image;
};


} // namespace iimg


#endif // !iimg_CReflectedBitmapBase_included


