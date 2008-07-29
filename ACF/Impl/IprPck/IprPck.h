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
#include "iipr/TGaussianProcessorComp.h"
#include "iipr/CGaussian2dFilterParams.h"
#include "iipr/CCaliperParams.h"
#include "iipr/CExtremumCaliperProcessorComp.h"
#include "iipr/CLineProjectionProcessorComp.h"
#include "iipr/CSearchResultSet.h"


/**
	system-undependent image processing package.
*/
namespace IprPck
{


typedef iipr::CProcessedAcquisitionComp ProcessedCamera;
typedef iipr::CMovingAverageProcessorComp MovingAverageProcessor;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CRectangularFilterParams>, iser::ISerializable> RectangularFilterParams;
typedef iipr::CLocalDifferenceProcessorComp LocalDifferenceProcessor;	
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CLocalDifferenceFilterParams>, iser::ISerializable> LocalDifferenceFilterParams;
typedef iipr::CIterativeProcessorComp IterationProcessor;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CIterativeProcessorParams>, iser::ISerializable> IterativeProcessorParams;
typedef iipr::CMorphoMaxOperatorComp MorphoMaxOperator;
typedef iipr::CMorphoMinOperatorComp MorphoMinOperator;
typedef iipr::CInvertOperatorComp InvertOperator;
typedef iipr::CGaussianFilterComp GaussianFilter;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CGaussian2dFilterParams>, iser::ISerializable> GaussianFilterParams;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CCaliperParams>, iipr::ICaliperParams, iser::ISerializable> CaliperParams;
typedef iipr::CExtremumCaliperProcessorComp ExtremumCaliperProcessor;
typedef iipr::CLineProjectionProcessorComp LineProjectionProcessor;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CSearchResultSet>, 
			iipr::ISearchResultSet, 
			iipr::IFeaturesConsumer, 
			iipr::IFeaturesContainer, 
			iser::ISerializable, 
			imod::IModel> SearchResults;


} // namespace IprPck


#endif // !IprPck_included


