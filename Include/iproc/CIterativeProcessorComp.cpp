#include "iproc/CIterativeProcessorComp.h"


// ACF includes
#include "iprm/ISelectionParam.h"
#include "iprm/TParamsPtr.h"

#include "ibase/IProgressManager.h"


namespace iproc
{


// reimplemented (iproc::IProcessor)

int CIterativeProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	if (		(inputPtr == NULL) ||
				(paramsPtr == NULL) ||
				!m_paramsIdAttrPtr.IsValid()){
		return TS_INVALID;
	}

	int retVal = TS_INVALID;

	int progressSessionId = -1;

	iprm::TParamsPtr<iprm::ISelectionParam> processorParamsPtr(paramsPtr, *m_paramsIdAttrPtr);
	if (processorParamsPtr.IsValid() && m_slaveProcessorCompPtr.IsValid()){
		int iterationsCount = processorParamsPtr->GetSelectedOptionIndex();

		if (iterationsCount <= 0){
			const istd::IChangeable* inputObjectPtr = dynamic_cast<const istd::IChangeable*>(inputPtr);

			if ((inputObjectPtr != NULL) && outputPtr->CopyFrom(*inputObjectPtr)){
				return TS_OK;
			}

			return TS_INVALID;
		}

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

				if (progressManagerPtr->IsCanceled(progressSessionId)){
					break;
				}
			}
		}
	}

	if (progressSessionId >= 0){
		I_ASSERT(progressManagerPtr != NULL);
		progressManagerPtr->EndProgressSession(progressSessionId);
	}

	return retVal;
}


// reimplemented (iprm::ISelectionConstraints)

int CIterativeProcessorComp::GetConstraintsFlags() const
{
	return SCF_NONE;
}


int CIterativeProcessorComp::GetOptionsCount() const
{
	return int(m_iterationNames.size());
}


QString CIterativeProcessorComp::GetOptionName(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_iterationNames.size()));

	return m_iterationNames[index];
}


QString CIterativeProcessorComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CIterativeProcessorComp::GetOptionId(int /*index*/) const
{
	return QByteArray();
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


// reimplemented (icomp::CComponentBase)

void CIterativeProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int optionsCount = qMax(int(0), *m_maxIterationsCountAttrPtr) + 1;
	m_iterationNames.resize(optionsCount);

	m_iterationNames[0] = "No iteration";
	if (optionsCount >= 1){
		m_iterationNames[1] = "1 iteration";

		for (int i = 2; i < optionsCount; ++i){
			m_iterationNames[i] = QString("%1").arg(i) + " iterations";
		}
	}
}


} // namespace iproc

