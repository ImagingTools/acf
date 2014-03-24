#ifndef iimg_CBitmapSurface_included
#define iimg_CBitmapSurface_included


// ACF includes
#include "imath/CSampledFunction2d.h"
#include "iimg/CBitmap.h"


namespace iimg
{


/**
	Union of a bitmap objects and its samples represented as a 2D sampled function.
*/
class CBitmapSurface: public iimg::CBitmap, public imath::CSampledFunction2d
{
public:
	typedef iimg::CBitmap BaseClass;
	typedef imath::CSampledFunction2d BaseClass2;

	CBitmapSurface();
	CBitmapSurface(const CBitmapSurface& bitmap);

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

private:
	void CreateSurfaceFromBitmap();
};


} // namespace iimg


#endif //!iimg_CBitmapSurface_included


