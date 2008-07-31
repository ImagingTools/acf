#ifndef QtIprPck_included
#define QtIprPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtipr/CRectangularFilterParamsGuiComp.h"
#include "iqtipr/CIterativeProcessorParamsGuiComp.h"
#include "iqtipr/CSearchGuiComp.h"
#include "iqtipr/CSearchParamsGuiComp.h"
#include "iqtipr/CCaliperBasedPositionSupplierGuiComp.h"


/**
	Standard Qt image processing package.
*/
namespace QtIprPck
{


typedef iqtipr::CRectangularFilterParamsGuiComp RectangularFilterParamsGui;
typedef iqtipr::CIterativeProcessorParamsGuiComp IterationParamsGui;
typedef iqtipr::CSearchGuiComp SearchGui;
typedef iqtipr::CSearchParamsGuiComp SearchParamsGui;
typedef iqtipr::CCaliperBasedPositionSupplierGuiComp CaliperBasedPositionSupplierGui;


} // namespace QtIprPck


#endif // !QtIprPck_included

