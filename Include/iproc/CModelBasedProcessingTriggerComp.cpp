#include "iproc/CModelBasedProcessingTriggerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"


namespace iproc
{


// public methods

CModelBasedProcessingTriggerComp::CModelBasedProcessingTriggerComp()
	:m_paramsObserver(*this)
{
}

// reimplemented (icomp::IComponent)

void CModelBasedProcessingTriggerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_paramsSetCompPtr.IsValid() && m_triggerOnParamsChangeAttrPtr.IsValid() && *m_triggerOnParamsChangeAttrPtr){
		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(m_paramsSetCompPtr.GetPtr());
		if (paramsModelPtr != NULL){
			paramsModelPtr->AttachObserver(&m_paramsObserver);
		}
	}
}


void CModelBasedProcessingTriggerComp::OnComponentDestroyed()
{
	if (m_paramsSetCompPtr.IsValid() && m_triggerOnParamsChangeAttrPtr.IsValid() && *m_triggerOnParamsChangeAttrPtr){
		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(m_paramsSetCompPtr.GetPtr());
		if (paramsModelPtr != NULL && paramsModelPtr->IsAttached(&m_paramsObserver)){
			paramsModelPtr->DetachObserver(&m_paramsObserver);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// protected methods
	
// reimplemented (imod::CSingleModelObserverBase)

void CModelBasedProcessingTriggerComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	DoProcessing();
}


// private methods

void CModelBasedProcessingTriggerComp::DoProcessing()
{
	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "Processor not set");

		return;
	}

	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Output data object not set");

		return;
	}

	istd::CChangeNotifier changePtr(m_outputDataCompPtr.GetPtr());

	istd::IChangeable* inputDataPtr = dynamic_cast<istd::IChangeable*>(GetModelPtr());

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDataPtr,
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing failed");
	}
}


// public methods of the embedded class ParamsObserver

CModelBasedProcessingTriggerComp::ParamsObserver::ParamsObserver(CModelBasedProcessingTriggerComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CModelBasedProcessingTriggerComp::ParamsObserver::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_parent.DoProcessing();
}


} // namespace iproc


