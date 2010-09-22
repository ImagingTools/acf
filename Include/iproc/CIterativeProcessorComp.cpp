#include "iproc/CIterativeProcessorComp.h"


#include "iproc/IProgressManager.h"


namespace iproc
{


// reimplemented (iproc::IProcessor)

int CIterativeProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			IProgressManager* progressManagerPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	if (		(inputPtr == NULL) ||
				(outputPtr == NULL) ||
				(paramsPtr == NULL) ||
				!m_paramsIdAttrPtr.IsValid()){
		return TS_INVALID;
	}

	int retVal = TS_INVALID;

	int progressSessionId = -1;

	const CIterativeProcessorParams* processorParamsPtr = dynamic_cast<const CIterativeProcessorParams*>(
				paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	if ((processorParamsPtr != NULL) && m_slaveProcessorCompPtr.IsValid()){
		int iterationsCount = processorParamsPtr->GetIterationsCount();

		if (progressManagerPtr != NULL){
			progressSessionId = progressManagerPtr->BeginProgressSession("IterativeProcessor", "Iteration");
		}

		for (int iterationIndex = 0; iterationIndex < iterationsCount; iterationIndex++){
			retVal = ProcessSlave(paramsPtr, inputPtr, outputPtr);
			if (retVal != TS_OK){
				break;
			}

			if (iterationIndex < iterationsCount - 1){
				if (!m_bufferObjectCompPtr.IsValid()){
					SendErrorMessage(MI_BAD_BUFFER, "No buffer object");

					retVal = TS_INVALID;

					break;
				}

				if (m_bufferObjectCompPtr->CopyFrom(*outputPtr)){
					inputPtr = m_bufferObjectCompPtr.GetPtr();
				}
				else{
					SendErrorMessage(MI_BAD_BUFFER, "Copy of used type is not supported by buffer object");

					retVal = TS_INVALID;

					break;
				}
			}

			if (progressSessionId >= 0){
				I_ASSERT(progressManagerPtr != NULL);
				progressManagerPtr->OnProgress(progressSessionId, iterationIndex / double(iterationsCount - 1));
			}
		}
	}

	if (progressSessionId >= 0){
		I_ASSERT(progressManagerPtr != NULL);
		progressManagerPtr->EndProgressSession(progressSessionId);
	}

	return retVal;
}


// protected methods

int CIterativeProcessorComp::ProcessSlave(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	I_ASSERT(m_slaveProcessorCompPtr.IsValid());

	int retVal = TS_INVALID;

	int taskId = m_slaveProcessorCompPtr->BeginTask(paramsPtr, inputPtr, outputPtr);
	if (taskId >= 0){
		retVal = m_slaveProcessorCompPtr->WaitTaskFinished(taskId);
	}

	return retVal;
}


} // namespace iproc

