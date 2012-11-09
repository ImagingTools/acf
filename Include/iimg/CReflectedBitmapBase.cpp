#include "iimg/CReflectedBitmapBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iimg
{


// reimplemented (iimg::IQImageProvider)

const QImage& CReflectedBitmapBase::GetQImage() const
{
	EnsureCacheValid();

	return m_image;
}


bool CReflectedBitmapBase::CopyImageFrom(const QImage& image)
{
	istd::CChangeNotifier notifier(this, CF_BLOCK_BITMAP_CONVERSION | CF_MODEL);

	if (ConvertFromQImage(image)){
		m_image = image;

		SetCacheValid();

		return true;
	}

	return false;
}


// protected methods

// reimplmented (istd::TCachedUpdateManagerWrap)

bool CReflectedBitmapBase::CalculateCache(int changeFlags)
{
	if ((changeFlags & CF_BLOCK_BITMAP_CONVERSION) == 0){
		return ConvertToQImage(m_image);
	}

	return true;
}


} // namespace iimg


