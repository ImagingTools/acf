#ifndef iqt_IQtBitmap_included
#define iqt_IQtBitmap_included


// Qt includes
#include <QImage>

#include "istd/IPolymorphic.h"

#include "iqt/iqt.h"


namespace iqt
{


class IQImageProvider: public istd::IPolymorphic
{
public:
	virtual const QImage& GetQImage() const = 0;
	virtual bool CopyImageFrom(const QImage& image) = 0;
};


} // namespace iqt


#endif // !iqt_IQtBitmap_included


