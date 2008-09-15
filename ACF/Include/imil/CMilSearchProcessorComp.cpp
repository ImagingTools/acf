#include "imil/CMilSearchProcessorComp.h"


#include "istd/TChangeNotifier.h"

#include "iimg/CBitmapBase.h"

#include "iwin/CTimer.h"

#include "iipr/CSearchFeature.h"


namespace imil
{


// public methods

// reimplemented (iproc::IProcessor)

int CMilSearchProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	iipr::IFeaturesConsumer* outputConsumerPtr = dynamic_cast<iipr::IFeaturesConsumer*>(outputPtr);

	if (		(inputBitmapPtr == NULL) ||
				(outputConsumerPtr == NULL) ||
				(paramsPtr == NULL) ||
				!m_paramsIdAttrPtr.IsValid()){
		return TS_INVALID;
	}

	const CMilSearchParams* milParamsPtr = dynamic_cast<const CMilSearchParams*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	if (milParamsPtr == NULL){
		SendErrorMessage(0, "Invalid parameter type");

		return TS_INVALID;
	}

	return DoSearch(*milParamsPtr, *inputBitmapPtr, *outputConsumerPtr)? TS_OK: TS_INVALID;
}


// protected methods

bool CMilSearchProcessorComp::DoSearch(
			const CMilSearchParams& params,
			const iimg::IBitmap& bitmap,
			iipr::IFeaturesConsumer& result)
{
	iwin::CTimer timer;

	istd::TChangeNotifier<iipr::IFeaturesConsumer> consumerPtr(&result);
	I_ASSERT(consumerPtr.IsValid());

	consumerPtr->ResetFeatures();

	const imil::CMilSearchModel& searchModel = params.GetModel(); 
	if (!searchModel.IsValid()){
		SendErrorMessage(0, "Invalid model or model type");

		return false;
	}

	i2d::CRectangle searchRegion = params.GetSearchRegion();
	i2d::CRectangle bitmapRect = i2d::CRectangle(0, 0, bitmap.GetImageSize().GetX(), bitmap.GetImageSize().GetY());
	searchRegion = bitmapRect.GetIntersection(searchRegion);
	if (searchRegion.IsEmpty()){
		SendErrorMessage(0, "Search region is empty");

		return false;
	}

	int regionWidth = int(searchRegion.GetWidth());
	int regionHeight = int(searchRegion.GetHeight());
	int bitsOffset = int(searchRegion.GetTop() * bitmap.GetLineBytesCount() + searchRegion.GetLeft());

	// Create MIL-image region from the user defined region:
	MIL_ID milImage = MbufCreate2d(
				m_engine.GetSystemId(), 
				regionWidth, 
				regionHeight, 
				8, 
				M_IMAGE + M_PROC, 
				M_HOST_ADDRESS | M_PITCH_BYTE, 
				bitmap.GetLineBytesCount(), 
				(char*)bitmap.GetLinePtr(0) + bitsOffset, 
				M_NULL);
	
	if (milImage == M_NULL){
		SendErrorMessage(0, "MIL image could not be created");

		return false;
	}

	// Ensure model preproccesing:
	searchModel.EnsurePreprocessing(params);

	// Allocate the result buffer:
	MIL_ID milResult = MmodAllocResult(m_engine.GetSystemId(), M_DEFAULT, M_NULL);
	if (milResult == M_NULL){
		SendErrorMessage(0, "MIL result could not be created");

		MbufFree(milImage);

		return false;
	}

	// Find the model:
	MmodFind(searchModel.GetContextId(), milImage, milResult);

	// Get the number of models found:
	long resultsCount = 0;
	MmodGetResult(milResult, M_DEFAULT, M_NUMBER + M_TYPE_LONG, &resultsCount);

	// If a model was found above the acceptance threshold.
	if (resultsCount >= 1){
		double scaleX = 0.0;
		double posy = 0.0; 
		double posx = 0.0;
		double angle = 0.0;
		double score = 0.0;

		// Get the results of the single model:
		for (int resultIndex = 0; resultIndex < resultsCount; resultIndex++){
			MmodGetResult(milResult, resultIndex, M_POSITION_X, &posx);
			MmodGetResult(milResult, resultIndex, M_POSITION_Y, &posy);
			MmodGetResult(milResult, resultIndex, M_ANGLE, &angle);
			MmodGetResult(milResult, resultIndex, M_SCALE, &scaleX);
			MmodGetResult(milResult, resultIndex, M_SCORE, &score);

			posy += searchRegion.GetTop();
			posx += searchRegion.GetLeft();

			i2d::CVector2d position(posx, posy);
			i2d::CVector2d scale(scaleX, scaleX);	

			angle = (fmod(1 + angle / 180.0, 2) - 1)  * I_PI;

			consumerPtr->AddFeature(new iipr::CSearchFeature(position, scale, angle, score));
		}
	}

	// Release the allocated image buffer:
	MbufFree(milImage);

	// Release the model result buffer:
	MmodFree(milResult);

	return true;
}


} // namespace imil


