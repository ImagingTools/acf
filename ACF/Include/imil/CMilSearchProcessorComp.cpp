#include "imil/CMilSearchProcessorComp.h"

#include "iimg/CBitmapBase.h"

#include "iwin/CTimer.h"

#include "iipr/CSearchResultSet.h"
#include "iipr/CSearchFeature.h"


namespace imil
{


// public methods

// reimplemented (iproc::TSyncProcessorWrap<iipr::ISearchProcessor>)

int CMilSearchProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iipr::IFeaturesConsumer* outputPtr)
{
	iwin::CTimer timer;

	if (inputPtr == NULL || outputPtr == NULL){
		return PS_INVALID;
	}

	outputPtr->ResetFeatures();

	const CMilSearchParams* milParamsPtr = NULL;
	if (m_searchParamsIdAttrPtr.IsValid()){
		milParamsPtr = dynamic_cast<const CMilSearchParams*>(paramsPtr->GetParameter(m_searchParamsIdAttrPtr->GetValue().ToString()));
	}

	if (milParamsPtr == NULL){
		return TS_INVALID;		
	}

	const imil::CMilSearchModel* milModelPtr = dynamic_cast<const imil::CMilSearchModel*>(&milParamsPtr->GetModel()); 
	if (milModelPtr == NULL || !milModelPtr->IsValid()){
		return TS_INVALID;
	}

	iipr::CSearchResultSet* resultSetPtr = dynamic_cast<iipr::CSearchResultSet*>(outputPtr);
	if (resultSetPtr == NULL){
		return TS_INVALID;
	}

	i2d::CRectangle searchAoi = milParamsPtr->GetSearchRegion();
	i2d::CRectangle bitmapRect = i2d::CRectangle(0,0,inputPtr->GetImageSize().GetX(), inputPtr->GetImageSize().GetY());
	searchAoi = bitmapRect.GetIntersection(searchAoi);
	if (searchAoi.IsEmpty()){
		return TS_INVALID;
	}

	int regionWidth = int(searchAoi.GetWidth());
	int regionHeight = int(searchAoi.GetHeight());
	int bitsOffset = int(searchAoi.GetTop() * inputPtr->GetLineBytesCount() + searchAoi.GetLeft());

	// Create MIL-image region from the user defined AOI:
	MIL_ID milImage = MbufCreate2d(
		m_engine.GetSystemId(), 
		regionWidth, 
		regionHeight, 
		8, 
		M_IMAGE + M_PROC, 
		M_HOST_ADDRESS | M_PITCH_BYTE, 
		inputPtr->GetLineBytesCount(), 
		(char*)inputPtr->GetLinePtr(0) + bitsOffset, 
		M_NULL);
	
	if (milImage == M_NULL){
		return TS_INVALID;
	}

	// Ensure model preproccesing:
	(const_cast<imil::CMilSearchParams*>(milParamsPtr))->EnsurePreprocessing();

	// Allocate a result buffer:
	MIL_ID milResult = MmodAllocResult(m_engine.GetSystemId(), M_DEFAULT, M_NULL);
	if (milResult == M_NULL){
		MbufFree(milImage);

		return false;
	}

	// Find the model:
	MmodFind(milModelPtr->GetContextId(), milImage, milResult);

	long numResults;

	// Get the number of models found.
	MmodGetResult(milResult, M_DEFAULT, M_NUMBER + M_TYPE_LONG, &numResults);

	// If a model was found above the acceptance threshold.
	if (numResults >= 1){
		double scaleX = 0.0;
		double posy = 0.0; 
		double posx = 0.0;
		double angle = 0.0;
		double score = 0.0;

		// Get the results of the single model:
		for (int resultIndex = 0; resultIndex < numResults; resultIndex++){
			MmodGetResult(milResult, resultIndex, M_POSITION_X, &posx);
			MmodGetResult(milResult, resultIndex, M_POSITION_Y, &posy);
			MmodGetResult(milResult, resultIndex, M_ANGLE, &angle);
			MmodGetResult(milResult, resultIndex, M_SCALE, &scaleX);
			MmodGetResult(milResult, resultIndex, M_SCORE, &score);

			posy += searchAoi.GetTop();
			posx += searchAoi.GetLeft();

			i2d::CVector2d position(posx, posy);
			i2d::CVector2d scale(scaleX, scaleX);	

			angle = (fmod(1 + angle / 180.0, 2) - 1)  * I_PI;

			outputPtr->AddFeature(new iipr::CSearchFeature(position, scale, angle, score));
		}
	}

	// Release the allocated image buffer:
	MbufFree(milImage);

	// Release the model result buffer:
	MmodFree(milResult);

	// set search duration:

	resultSetPtr->SetTime(timer.GetElapsed() * 1000);

	return TS_OK;
}


} // namespace imil

