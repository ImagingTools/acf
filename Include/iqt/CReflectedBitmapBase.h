#ifndef iqt_CReflectedBitmapBase_included
#define iqt_CReflectedBitmapBase_included


#include "istd/TCachedUpdateManagerWrap.h"

#include "iimg/CGeneralBitmap.h"

#include "iqt/IQImageProvider.h"


namespace iqt
{


/**
	Implementation of bitmap storing internal additionaly QImage object reflecting state of main bitmap after conversion to Qt formats.
	It allows to working with Qt with other bitmap formats.
*/
class CReflectedBitmapBase: public istd::TCachedUpdateManagerWrap<iimg::CGeneralBitmap>, virtual public IQImageProvider
{
public:
	enum ChangeFlags{
		CF_BLOCK_BITMAP_CONVERSION = 0x2000
	};

	// reimplemented (iqt::IQImageProvider)
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


} // namespace iqt


#endif // !iqt_CReflectedBitmapBase_included


