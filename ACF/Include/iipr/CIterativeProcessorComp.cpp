#include "iipr/CIterativeProcessorComp.h"


#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CIterativeProcessorComp::DoProcessing(
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

	int retVal = TS_INVALID;

	const CIterativeProcessorParams* processorParamsPtr = dynamic_cast<const CIterativeProcessorParams*>(
				paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	if ((processorParamsPtr != NULL) && m_slaveProcessorCompPtr.IsValid()){
		int iterationsCount = processorParamsPtr->GetIterationsCount();

		iimg::CGeneralBitmap bufferBitmap;

		for (int iterationIndex = 0; iterationIndex < iterationsCount; iterationIndex++){
			retVal = ProcessSlave(paramsPtr, inputBitmapPtr, outputBitmapPtr);
			if (retVal != TS_OK){
				return retVal;
			}

			if (iterationIndex < iterationsCount - 1){
				bufferBitmap.CopyImageFrom(*outputBitmapPtr);

				inputBitmapPtr = &bufferBitmap;
			}
		}
	}

	return retVal;
}


// protected methods

int CIterativeProcessorComp::ProcessSlave(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iimg::IBitmap* outputPtr)
{
	I_ASSERT(m_slaveProcessorCompPtr.IsValid());

	int retVal = TS_INVALID;

	int taskId = m_slaveProcessorCompPtr->BeginTask(paramsPtr, inputPtr, outputPtr);
	if (taskId >= 0){
		retVal = m_slaveProcessorCompPtr->WaitTaskFinished(taskId);
	}

	return retVal;
}


} // namespace iipr

