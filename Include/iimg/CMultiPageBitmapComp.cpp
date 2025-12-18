#include <iimg/CMultiPageBitmapComp.h>


namespace iimg
{


// protected methods

// reimplemented (CMultiPageBitmapBase)

IBitmapUniquePtr CMultiPageBitmapComp::CreateBitmap() const
{
	if (m_bitmapFactoryCompPtr.IsValid()){
		return m_bitmapFactoryCompPtr.CreateInstance();
	}

	return NULL;
}


} // namespace iimg


