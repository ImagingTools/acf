#ifndef IprPck_included
#define IprPck_included


#include "imod/TModelWrap.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iipr/TMorphoProcessorComp.h"
#include "iipr/CMovingAverageProcessorComp.h"
#include "iipr/CMultidimensionalFilterParamsComp.h"
#include "iipr/CProcessedAcquisitionComp.h"
#include "iipr/CLocalDifferenceProcessorComp.h"
#include "iipr/CLocalDifferenceFilterParams.h"
#include "iipr/CIterativeProcessorComp.h"
#include "iipr/CIterativeProcessorParams.h"
#include "iipr/TUnaryTransformProcessorComp.h"
#include "iipr/TGaussianProcessorComp.h"
#include "iipr/CGaussian2dFilterParams.h"
#include "iipr/CCaliperParamsComp.h"
#include "iipr/CExtremumCaliperProcessorComp.h"
#include "iipr/CLineProjectionProcessorComp.h"
#include "iipr/CCaliperBasedPositionSupplierComp.h"
#include "iipr/CRectDerivativeProcessorComp.h"
#include "iipr/CProjectionData.h"
#include "iipr/TFeaturesContainerWrap.h"
#include "iipr/CSearchFeature.h"


/**
	system-undependent image processing package.
*/
namespace IprPck
{

typedef iipr::TFeaturesContainerWrap<iipr::IFeaturesContainer, iipr::CSearchFeature> CSearchFeatures;


typedef iipr::CProcessedAcquisitionComp ProcessedCamera;
typedef iipr::CMovingAverageProcessorComp MovingAverageProcessor;
typedef icomp::TModelCompWrap<iipr::CMultidimensionalFilterParamsComp> RectangularFilterParams;
typedef iipr::CLocalDifferenceProcessorComp LocalDifferenceProcessor;	
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CLocalDifferenceFilterParams>, iser::ISerializable> LocalDifferenceFilterParams;
typedef iipr::CIterativeProcessorComp IterationProcessor;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CIterativeProcessorParams>, iser::ISerializable> IterativeProcessorParams;
typedef iipr::CMorphoMaxOperatorComp MorphoMaxOperator;
typedef iipr::CMorphoMinOperatorComp MorphoMinOperator;
typedef iipr::CInvertOperatorComp InvertOperator;
typedef iipr::CGaussianFilterComp GaussianFilter;
typedef icomp::TMakeComponentWrap<imod::TModelWrap<iipr::CGaussian2dFilterParams>, iser::ISerializable> GaussianFilterParams;
typedef icomp::TModelCompWrap<iipr::CCaliperParamsComp> CaliperParams;
typedef iipr::CExtremumCaliperProcessorComp ExtremumCaliperProcessor;
typedef iipr::CLineProjectionProcessorComp LineProjectionProcessor;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<CSearchFeatures>,
			iipr::IFeaturesConsumer,
			iipr::IFeaturesContainer,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> SearchResults;
typedef icomp::TModelCompWrap<iipr::CCaliperBasedPositionSupplierComp> CaliperBasedPositionSupplier;
typedef iipr::CRectDerivativeProcessorComp RectDerivativeProcessor;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<iipr::CProjectionData>,
			iipr::CProjectionData,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> ProjectionData;



} // namespace IprPck


#endif // !IprPck_included


