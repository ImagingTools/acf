#ifndef iqt_CBitmapSurface_included
#define iqt_CBitmapSurface_included



#include "istd/TChangeDelegator.h"

#include "imath/CSampledFunction2d.h"

#include "iqt/CBitmap.h"


namespace iqt
{


/**
	Bitmap implementation based on Qt QImage.
*/
class CBitmapSurface: public iqt::CBitmap, public imath::CSampledFunction2d
{
public:
	typedef iqt::CBitmap BaseClass;
	typedef imath::CSampledFunction2d BaseClass2;

	CBitmapSurface();
	CBitmapSurface(const CBitmapSurface& bitmap);

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* chaneParamsPtr);

private:
	void CreateSurfaceFromBitmap();
};


} // namespace iqt


#endif //!iqt_CBitmapSurface_included


