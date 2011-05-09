#ifndef iproc_CModelBasedProcessingTriggerComp_included
#define iproc_CModelBasedProcessingTriggerComp_included


// ACF includes
#include "ibase/TLoggerCompWrap.h"

#include "imod/TSingleModelObserverBase.h"

#include "iproc/IProcessor.h"


namespace iproc
{


/**
	Component to trigger a processing action depending on model changes.
*/
class CModelBasedProcessingTriggerComp:
			public ibase::CLoggerComponentBase,
			public imod::CSingleModelObserverBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	
	I_BEGIN_COMPONENT(CModelBasedProcessingTriggerComp);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_processorCompPtr, "Processor", "Target processing component", true, "Processor");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress manager", false, "ProgressManager");
		I_ASSIGN(m_paramsSetCompPtr, "ProcessingParamsSet", "Processing parameters", false, "ProcessingParameters");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Processing output data", true, "OutputData");
		I_ASSIGN(m_triggerOnParamsChangeAttrPtr, "TriggerOnParameterChanges", "Start processing if the parameter have been changed", false, false);
	I_END_COMPONENT;

	CModelBasedProcessingTriggerComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	void DoProcessing();

private:
	class ParamsObserver: public imod::TSingleModelObserverBase<iprm::IParamsSet>
	{
	public:
		ParamsObserver(CModelBasedProcessingTriggerComp& parent);

		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CModelBasedProcessingTriggerComp& m_parent;
	};

private:
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_ATTR(bool, m_triggerOnParamsChangeAttrPtr);

	ParamsObserver m_paramsObserver;
};


} // namespace iproc


#endif // !iproc_CModelBasedProcessingTriggerComp_included


