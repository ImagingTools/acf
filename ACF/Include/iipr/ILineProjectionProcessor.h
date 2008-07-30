#ifndef iipr_ILineProjectionProcessor_included
#define iipr_ILineProjectionProcessor_included


#include "imath/TVarVector.h"

#include "i2d/CLine2d.h"

#include "iproc/TIProcessor.h"

#include "iimg/IBitmap.h"

#include "iipr/CProjectionData.h"


namespace iipr
{


class IProjectionParams;


class ILineProjectionProcessor: virtual public iproc::TIProcessor<iimg::IBitmap, CProjectionData>
{
public:
	/**
		Do line projection with explicite projection parameters.
	*/
	virtual bool DoProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CLine2d& projectionLine,
				const IProjectionParams* paramsPtr,
				CProjectionData& results) = 0;

	/**
		Get position in bitmap coordination system using position extracted from projection.
	*/
	virtual imath::CVarVector GetBitmapPosition(
				const imath::CVarVector& projectionPosition,
				const iprm::IParamsSet* paramsPtr) const = 0;
};


} // namespace iipr


#endif // !iipr_ILineProjectionProcessor_included


