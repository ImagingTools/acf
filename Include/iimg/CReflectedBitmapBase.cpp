#include <iimg/CReflectedBitmapBase.h>


// ACF includes
#include <istd/CChangeNotifier.h>


namespace iimg
{


const istd::IChangeable::ChangeSet s_blockConvChangeSet(CReflectedBitmapBase::CF_BLOCK_BITMAP_CONVERSION);


// reimplemented (iimg::IQImageProvider)

const QImage& CReflectedBitmapBase::GetQImage() const
{
	EnsureCacheValid();

	return m_image;
}


bool CReflectedBitmapBase::CopyImageFrom(const QImage& image)
{
	istd::CChangeNotifier notifier(this, &s_blockConvChangeSet);
	Q_UNUSED(notifier);

	if (ConvertFromQImage(image)){
		m_image = image;

		SetCacheValid();

		return true;
	}

	return false;
}


// protected methods

// reimplmented (istd::TCachedUpdateManagerWrap)

bool CReflectedBitmapBase::CalculateCache(const ChangeSet& changeSet)
{
	if (!changeSet.Contains(CF_BLOCK_BITMAP_CONVERSION)){
		return ConvertToQImage(m_image);
	}

	return true;
}


} // namespace iimg


