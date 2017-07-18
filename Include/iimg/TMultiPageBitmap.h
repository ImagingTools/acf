#ifndef iimg_TMultiPageBitmap_included
#define iimg_TMultiPageBitmap_included


// ACF includes
#include <iimg/CMultiPageBitmapBase.h>
#include <iimg/CGeneralBitmap.h>


namespace iimg
{

	
/**
	Generic implementation of a multi-page bitmap.

	\tparam	BitmapImpl	Type of the bitmap implementation.
*/
template <class BitmapImpl>
class TMultiPageBitmap: public CMultiPageBitmapBase
{
public:
	typedef CMultiPageBitmapBase BaseClass;
	typedef BitmapImpl BitmapType;

	// reimplemented (istd::IChangeable)
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

protected:
	// reimplemented (CMultiPageBitmapBase)
	virtual IBitmap* CreateBitmap() const;
};


// public methods

// reimplemented (istd::IChangeable)

template <class BitmapImpl>
istd::IChangeable* TMultiPageBitmap<BitmapImpl>::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr< TMultiPageBitmap<BitmapImpl> > clonedPtr(new TMultiPageBitmap<BitmapImpl>);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (CMultiPageBitmapBase)

template <class BitmapImpl>
IBitmap* TMultiPageBitmap<BitmapImpl>::CreateBitmap() const
{
	return new BitmapImpl;
}


typedef TMultiPageBitmap<CGeneralBitmap> CGeneralMultiPageBitmap;


} // namespace iimg


#endif // !iimg_TMultiPageBitmap_included


