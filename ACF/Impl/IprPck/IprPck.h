#ifndef IprPck_included
#define IprPck_included


#include "imod/TModelWrap.h"

#include "icomp/TMakeComponentWrap.h"

#include "iipr/TMorphoProcessorComp.h"
#include "iipr/CMovingAverageProcessorComp.h"
#include "iipr/CRectangularFilterParams.h"
#include "iipr/CProcessedAcquisitionComp.h"
#include "iipr/CLocalDifferenceProcessorComp.h"
#include "iipr/CLocalDifferenceFilterParams.h"
#include "iipr/CIterativeProcessorComp.h"
#include "iipr/CIterativeProcessorParams.h"
#include "iipr/TUnaryTransformProcessorComp.h"
#include "iipr/CCaliperParams.h"

/**
	system-undependent image processing package.
*/
namespace IprPck
{


typedef iipr::CProcessedAcquisitionComp ProcessedCamera;
typedef iipr::CMovingAverageProcessorComp MovingAverageProcessor;
typedef icomp::TMakeComponentWrap<iser::ISerializable, imod::TModelWrap<iipr::CRectangularFilterParams> > RectangularFilterParams;
typedef iipr::CLocalDifferenceProcessorComp LocalDifferenceProcessor;	
typedef icomp::TMakeComponentWrap<iser::ISerializable, imod::TModelWrap<iipr::CLocalDifferenceFilterParams> > LocalDifferenceFilterParams;
typedef iipr::CIterativeProcessorComp IterationProcessor;
typedef icomp::TMakeComponentWrap<iser::ISerializable, imod::TModelWrap<iipr::CIterativeProcessorParams> > IterativeProcessorParams;

typedef iipr::CMorphoMaxOperatorComp MorphoMaxOperator;
typedef iipr::CMorphoMinOperatorComp MorphoMinOperator;
typedef iipr::CInvertOperatorComp InverOperator;

typedef icomp::TMakeComponentWrap<iipr::ICaliperParams, imod::TModelWrap<iipr::CCaliperParams> > CaliperParams;

} // namespace IprPck


#endif // !IprPck_included


