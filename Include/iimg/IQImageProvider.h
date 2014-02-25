#ifndef iimg_IQImageProvider_included
#define iimg_IQImageProvider_included


// Qt includes
#include <QtGui/QImage>

// ACF includes
#include "istd/IPolymorphic.h"
#include "iimg/iimg.h"


namespace iimg
{


class IQImageProvider: virtual public istd::IPolymorphic
{
public:
	virtual const QImage& GetQImage() const = 0;
	virtual bool CopyImageFrom(const QImage& image) = 0;
};


} // namespace iimg


#endif // !iimg_IQImageProvider_included


