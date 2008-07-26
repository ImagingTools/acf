#ifndef iipr_TImageProcessorCompBase_included
#define iipr_TImageProcessorCompBase_included


#include "ibase/TMessageProducerWrap.h"

#include "iproc/TSyncProcessorCompBase.h"

#include "icomp/CComponentBase.h"

#include "iimg/CGeneralBitmap.h"

#include "iipr/IBitmapProcessor.h"

#include "iwin/CTimer.h"


namespace iipr
{


/**
	Basic implementation for a simple bitmap processor.
*/
template <class ParameterType>
class TImageProcessorCompBase: public iproc::TSyncProcessorCompBase<iipr::IBitmapProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IBitmapProcessor> BaseClass;

protected:
	// reimplemented (iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iimg::IBitmap* outputPtr);

	// abstract methods
	virtual bool ProcessImage(
				const ParameterType* parameterPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;
};


// protected methods

template <class ParameterType>
int TImageProcessorCompBase<ParameterType>::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iimg::IBitmap* outputPtr)
{
	iwin::CTimer timer;

	if (inputPtr == NULL || outputPtr == NULL){
		return BaseClass2::TS_INVALID;
	}

	if (!m_paramsIdAttrPtr.IsValid()){
		return BaseClass2::TS_INVALID;
	}

	iimg::CGeneralBitmap outputBitmap;

	if (m_preprocessorCompPtr.IsValid()){
		int taskId = m_preprocessorCompPtr->BeginTask(paramsPtr, inputPtr, &outputBitmap);
		if (taskId >= 0){
			int retVal = m_preprocessorCompPtr->WaitTaskFinished(taskId);
			if (retVal == TS_OK){
				inputPtr = &outputBitmap;
			}
		}
	}

	// create output image:
	outputPtr->CopyImageFrom(*inputPtr);

	const ParameterType* processorParamsPtr = 
		dynamic_cast<const ParameterType*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	
	// do image processing:
	if (!ProcessImage(processorParamsPtr, *inputPtr, *outputPtr)){
		return BaseClass2::TS_INVALID;
	}
	
	double processingTime = timer.GetElapsed();
	SendInfoMessage(0, istd::CString("Processed in ") + istd::CString::FromNumber(processingTime * 1000) + "ms");

	return BaseClass2::TS_OK;
}


} // namespace iipr


#endif // !iipr_TImageProcessorCompBase_included

