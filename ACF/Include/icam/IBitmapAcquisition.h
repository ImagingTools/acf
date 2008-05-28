#ifndef icam_IBitmapAcquisition_included
#define icam_IBitmapAcquisition_included


#include "istd/CIndex2d.h"

#include "isys/ITimer.h"

#include "iimg/IBitmap.h"

#include "iproc/TIAssyncProcessor.h"


namespace icam
{


/**
	Bitmap acquisition processor.
	This processor use as \c iproc::TIAssyncProcessor template parameter Input optional isys::ITimer object to indicate trigger time of acquision.
*/
class IBitmapAcquisition: virtual public iproc::TIAssyncProcessor<isys::ITimer, iimg::IBitmap>
{
public:
	/**
		Get size of acquired bitmap fo specified parameters.
		\return		if size of bitmap is known it returns this value. Elsewhere invalid size will be returned.
					\sa \c istd::TIndex::IsValid().
	*/
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const = 0;
};


} // namespace icam


#endif // !icam_IBitmapAcquisition_included


