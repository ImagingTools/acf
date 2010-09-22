#include "iproc/CCascadedProcessorComp.h"


#include "istd/TPointerVector.h"

#include "iimg/CGeneralBitmap.h"

#include "iproc/CDelegatedProgressManager.h"


namespace iproc
{


// reimplemented (iproc::IProcessor)

int CCascadedProcessorComp::GetProcessorState(const iprm::IParamsSet* paramsPtr) const
{
	int retVal = BaseClass2::GetProcessorState(paramsPtr);

	int processorsCount = m_processorsCompPtr.GetCount();
	for (int i = 0; i < processorsCount; ++i){
		const iproc::IProcessor* processorPtr = m_processorsCompPtr[i];
		I_ASSERT(processorPtr != NULL);	// flag 'isObligatory' was set to true

		retVal = istd::Max(retVal, processorPtr->GetProcessorState(paramsPtr));
	}

	return retVal;
}


void CCascadedProcessorComp::ResetAllTasks()
{
	BaseClass2::ResetAllTasks();

	int processorsCount = m_processorsCompPtr.GetCount();
	for (int i = 0; i < processorsCount; ++i){
		iproc::IProcessor* processorPtr = m_processorsCompPtr[i];
		I_ASSERT(processorPtr != NULL);	// flag 'isObligatory' was set to true

		processorPtr->ResetAllTasks();
	}
}


bool CCascadedProcessorComp::AreParamsAccepted(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			const istd::IChangeable* outputPtr) const
{
	int buffersCount = m_buffersCompPtr.GetCount();

	int processorsCount = m_processorsCompPtr.GetCount();
	for (int i = 0; i < processorsCount; ++i){
		const istd::IPolymorphic* processorInputPtr = NULL;
		if (i > 0){
			int bufferIndex = i - 1;
			if (bufferIndex < buffersCount){
				processorInputPtr = m_buffersCompPtr[bufferIndex];
			}
		}
		else{
			processorInputPtr = inputPtr;
		}

		const istd::IChangeable* processorOutputPtr = NULL;
		if (i < processorsCount - 1){
			if (i < buffersCount){
				processorOutputPtr = m_buffersCompPtr[i];
			}
		}
		else{
			processorOutputPtr = outputPtr;
		}

		const iproc::IProcessor* processorPtr = m_processorsCompPtr[i];
		I_ASSERT(processorPtr != NULL);	// flag 'isObligatory' was set to true

		if (!processorPtr->AreParamsAccepted(paramsPtr, processorInputPtr, processorOutputPtr)){
			return false;
		}
	}

	return true;
}


int CCascadedProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			IProgressManager* progressManagerPtr)
{
	int processorsCount = m_processorsCompPtr.GetCount();
	istd::TPointerVector<CDelegatedProgressManager> progressDelegators;
	progressDelegators.SetCount(processorsCount);

	if (progressManagerPtr != NULL){
		int managersCount = istd::Min(processorsCount, m_progressIdsAttrPtr.GetCount());
		for (int i = 0; i < managersCount; ++i){
			const istd::CString& progressId = m_progressIdsAttrPtr[i];
			if (!progressId.IsEmpty()){
				istd::CString description = progressId;
				if (i < m_progressDescriptionsAttrPtr.GetCount()){
					description = m_progressDescriptionsAttrPtr[i];
				}

				progressDelegators.SetElementAt(i, new CDelegatedProgressManager(progressManagerPtr, progressId.ToString(), description));
			}
		}
	}

	int buffersCount = m_buffersCompPtr.GetCount();

	for (int i = 0; i < processorsCount; ++i){
		const istd::IPolymorphic* processorInputPtr = NULL;
		if (i > 0){
			int bufferIndex = i - 1;
			if (bufferIndex < buffersCount){
				processorInputPtr = m_buffersCompPtr[bufferIndex];
			}
		}
		else{
			processorInputPtr = inputPtr;
		}

		istd::IChangeable* processorOutputPtr = NULL;
		if (i < processorsCount - 1){
			if (i < buffersCount){
				processorOutputPtr = m_buffersCompPtr[i];
			}
		}
		else{
			processorOutputPtr = outputPtr;
		}

		iproc::IProcessor* processorPtr = m_processorsCompPtr[i];
		I_ASSERT(processorPtr != NULL);	// flag 'isObligatory' was set to true

		int taskState = processorPtr->DoProcessing(paramsPtr, processorInputPtr, processorOutputPtr, progressDelegators.GetAt(i));
		if (taskState != TS_OK){
			return taskState;
		}
	}

	return TS_OK;
}


void CCascadedProcessorComp::InitProcessor(const iprm::IParamsSet* paramsPtr)
{
	BaseClass2::InitProcessor(paramsPtr);

	int processorsCount = m_processorsCompPtr.GetCount();
	for (int i = 0; i < processorsCount; ++i){
		iproc::IProcessor* processorPtr = m_processorsCompPtr[i];
		I_ASSERT(processorPtr != NULL);	// flag 'isObligatory' was set to true

		processorPtr->InitProcessor(paramsPtr);
	}
}


} // namespace iproc


