#ifndef iipr_TImageProcessorCompBase_included
#define iipr_TImageProcessorCompBase_included


#include "ibase/TMessageProducerWrap.h"

#include "iproc/TSyncProcessorCompBase.h"

#include "icomp/CComponentBase.h"

#include "iimg/CGeneralBitmap.h"

#include "iwin/CTimer.h"


namespace iipr
{


/**
	Basic implementation for a simple bitmap processor.
*/
template <class ParameterType>
class TImageProcessorCompBase: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	// abstract methods
	virtual bool ProcessImage(
				const ParameterType* parameterPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;
};


// public methods

// reimplemented (iproc::IProcessor)

template <class ParameterType>
int TImageProcessorCompBase<ParameterType>::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);

	if (		(inputBitmapPtr == NULL) ||
				(outputBitmapPtr == NULL) ||
				(paramsPtr == NULL) ||
				!m_paramsIdAttrPtr.IsValid()){
		return TS_INVALID;
	}

	iwin::CTimer timer;

	iimg::CGeneralBitmap bufferBitmap;

	if (m_preprocessorCompPtr.IsValid()){
		int retVal = m_preprocessorCompPtr->DoProcessing(paramsPtr, inputBitmapPtr, &bufferBitmap);
		if (retVal == TS_OK){
			inputBitmapPtr = &bufferBitmap;
		}
	}

	// create output image:
	outputBitmapPtr->CopyImageFrom(*inputBitmapPtr);

	const ParameterType* processorParamsPtr = dynamic_cast<const ParameterType*>(
				paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	
	// do image processing:
	if (!ProcessImage(processorParamsPtr, *inputBitmapPtr, *outputBitmapPtr)){
		return TS_INVALID;
	}
	
	double processingTime = timer.GetElapsed();
	SendInfoMessage(0, istd::CString("Processed in ") + istd::CString::FromNumber(processingTime * 1000) + "ms");

	return TS_OK;
}


} // namespace iipr


#endif // !iipr_TImageProcessorCompBase_included

