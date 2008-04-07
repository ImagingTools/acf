#include "iproc/CProcessingControllerBase.h"
#include "iproc/COperatorBase.h"


namespace iproc
{


const ibase::IMessageContainer* CProcessingControllerBase::GetLog() const
{
	iproc::COperatorBase* operatorPtr = dynamic_cast<iproc::COperatorBase*>(GetObjectPtr());

	return operatorPtr;
}


void CProcessingControllerBase::AddProgressHandler(iproc::IProgressEventHandler* progressHandlerPtr)
{
	m_progressHandlers.insert(progressHandlerPtr);
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CProcessingControllerBase::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if (updateFlags == iproc::IOperator::ProgressChanged){
		iproc::IOperator* operatorPtr = GetObjectPtr();
		I_ASSERT(operatorPtr != NULL);
	
		double currentProgress = operatorPtr->GetProgress();

		for (ProgressHandlers::iterator index = m_progressHandlers.begin(); index != m_progressHandlers.end(); index++){
			(*index)->OnProgress(currentProgress);
		}
	}
}


} // namespace iproc

