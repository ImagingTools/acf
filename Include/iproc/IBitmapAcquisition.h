#ifndef iproc_IBitmapAcquisition_included
#define iproc_IBitmapAcquisition_included


#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"

#include "iproc/IProcessor.h"


namespace iproc
{


/**
	Bitmap acquisition processor.
	This processor use as \c iproc::IProcessor template parameter Input optional istd::ITimeStamp object to indicate trigger time of acquision.
*/
class IBitmapAcquisition: virtual public IProcessor
{
public:
	/**
		Get size of acquired bitmap fo specified parameters.
		\return		if size of bitmap is known it returns this value. Elsewhere invalid size will be returned.
					\sa \c istd::TIndex::IsValid().
	*/
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const = 0;
};


} // namespace iproc


#endif // !iproc_IBitmapAcquisition_included


