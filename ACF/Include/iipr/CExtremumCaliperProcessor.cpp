#include "iipr/CExtremumCaliperProcessor.h"


#include "iimg/IBitmap.h"
#include "iimg/TPixelConversion.h"

#include "iipr/ICaliperParams.h"
#include "iipr/IFeaturesConsumer.h"
#include "iipr/CCaliperFeature.h"


namespace iipr
{


// local functions

template <class PixelConversion>
bool FeaturesExtraction(
			const iimg::IBitmap& bitmap,
			bool isBackward,
			bool isRisedEdgeSupported,
			bool isDropppedEdgeSupported,
			const istd::CRange& ignoreIntensityRange,
			const istd::CRange& proportionRange,
			const PixelConversion& conversion,
			IFeaturesConsumer& results)
{
	istd::CIndex2d bitmapSize = bitmap.GetImageSize();
	if (bitmapSize.IsSizeEmpty()){
		return false;
	}

	results.ResetFeatures();

	int index = -1;
	int nextIndex = isBackward? bitmapSize.GetX() - 1: 0;
	int indexDiff = isBackward? -1: 1;
	int endIndex = isBackward? -1: bitmapSize.GetX();

	int risedCount = -1;
	int droppedCount = -1;

	PixelConversion::CalcPixelType prevPixel(0);
	PixelConversion::CalcPixelType pixel(0);

	const PixelConversion::SourcePixelType* derivativeElementsPtr = (const PixelConversion::SourcePixelType*)bitmap.GetLinePtr(0);

	for (; nextIndex != endIndex; nextIndex += indexDiff){
		PixelConversion::CalcPixelType nextPixel = conversion.GetCalc(derivativeElementsPtr[nextIndex]);

		if (conversion.GetWeight(nextPixel) > I_BIG_EPSILON){
			double pixelIntensity = conversion.GetIntensity(pixel);
			double nextPixelIntensity = conversion.GetIntensity(nextPixel);
			if (nextPixelIntensity < pixelIntensity){
				if (isRisedEdgeSupported && (risedCount > 0) && (pixelIntensity > ignoreIntensityRange.GetMaxValue())){
					double prevDelta = pixelIntensity - conversion.GetIntensity(prevPixel);
					I_ASSERT(prevDelta >= 0);

					double nextDelta = pixelIntensity - nextPixelIntensity;
					I_ASSERT(nextDelta >= 0);
					I_ASSERT(nextDelta + prevDelta > 0);

					double shift = prevDelta / (nextDelta + prevDelta);

					double position = proportionRange.GetValueFromAlpha((index + shift) / bitmapSize.GetX());
					I_ASSERT(position >= 0 - I_EPSILON);
					I_ASSERT(position <= 1 + I_EPSILON);

					bool isReady = false;
					if (		results.AddFeature(new CCaliperFeature(pixelIntensity * conversion.GetWeight(pixel), position), &isReady) ||
								isReady){
						return isReady;
					}
				}

				risedCount = 0;
				droppedCount++;
			}
			else{
				if (isDropppedEdgeSupported && (droppedCount > 0) && (pixelIntensity < ignoreIntensityRange.GetMinValue())){
					double prevDelta = conversion.GetIntensity(prevPixel) - pixelIntensity;
					I_ASSERT(prevDelta >= 0);

					double nextDelta = nextPixelIntensity - pixelIntensity;
					I_ASSERT(nextDelta >= 0);
					I_ASSERT(nextDelta + prevDelta > 0);

					double shift = prevDelta / (nextDelta + prevDelta);

					double position = proportionRange.GetValueFromAlpha((index + shift) / bitmapSize.GetX());
					I_ASSERT(position >= 0 - I_EPSILON);
					I_ASSERT(position <= 1 + I_EPSILON);

					bool isReady = false;
					if (		results.AddFeature(new CCaliperFeature(pixelIntensity * conversion.GetWeight(pixel), position), &isReady) ||
								isReady){
						return isReady;
					}
				}

				risedCount++;
				droppedCount = 0;
			}

			prevPixel = pixel;
			pixel = nextPixel;
			index = nextIndex;
		}
	}

	return true;
}


// public methods

bool CExtremumCaliperProcessor::DoExtremumCaliper(const CProjectionData& source, const ICaliperParams& params, IFeaturesConsumer& results)
{
	const iimg::IBitmap& bitmap = source.GetProjectionImage();

	double weightThreshold = params.GetWeightThreshold();
	I_ASSERT(weightThreshold >= 0);

	const istd::CRange& proportionRange = source.GetProportionRangeX();

	bool isBackward = (params.GetDirectionMode() == ICaliperParams::DM_BACKWARD);

	int polarityMode = params.GetPolarityMode();

	iimg::CGrayGrayPixelConversion conversion;

	return FeaturesExtraction(
				bitmap,
				isBackward,
				(polarityMode != ICaliperParams::PM_DROPPED),
				(polarityMode != ICaliperParams::PM_RISED),
				istd::CRange(128 - weightThreshold * 128, 128 + weightThreshold * 128),
				proportionRange,
				conversion,
				results);
}


// reimplemented (iipr::ICaliperProcessor)

bool CExtremumCaliperProcessor::DoCaliper(
			const CProjectionData& projection,
			const ICaliperParams* paramsPtr,
			IFeaturesConsumer& results)
{
	if (paramsPtr != NULL){
		return DoExtremumCaliper(projection, *paramsPtr, results);
	}

	return false;
}


// reimplemented (iproc::TIProcessor)

int CExtremumCaliperProcessor::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const CProjectionData* inputPtr,
			IFeaturesConsumer* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	if ((inputPtr == NULL) || (paramsPtr == NULL) || m_caliperParamsId.empty()){
		return TS_INVALID;
	}

	const ICaliperParams* caliperParamsPtr = dynamic_cast<const ICaliperParams*>(paramsPtr->GetParameter(m_caliperParamsId));
	if (caliperParamsPtr == NULL){
		return TS_INVALID;
	}

	return DoExtremumCaliper(*inputPtr, *caliperParamsPtr, *outputPtr)? TS_OK: TS_INVALID;
}


} // namespace iipr

