#ifndef imebase_CMeilhausSimpleComp_included
#define imebase_CMeilhausSimpleComp_included

#include "iprm/ISelectionParam.h"

#include "isig/ISamplesProcessor.h"
#include "isig/ISamplingParams.h"

#include "icomp/CComponentBase.h"

#include "IMeilhausAccess.h"
#include "CMeContext.h"


namespace imebase
{


/**
	Implementation allowing to operate on ME4600 card's analog I/O.
*/
class CMeilhausSimpleComp:
			public icomp::CComponentBase,
			virtual public isig::ISamplesProcessor,
			virtual public IMeilhausAccess
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMeilhausSimpleComp)
		I_REGISTER_INTERFACE(isig::ISamplesProcessor)
		I_REGISTER_INTERFACE(IMeilhausAccess)

		I_ASSIGN(m_isOutputAttrPtr, "IsOutput", "If true this processor operate in output mode, otherwise input mode", true, false);
		I_ASSIGN(m_addressParamIdAttrPtr, "AddressParamId", "ID of channel address in parameter set", true, "ChannelAddress");
		I_ASSIGN(m_samplingParamsIdAttrPtr, "SamplingParamsId", "ID of sampling parameters in parameter set", true, "SamplingParams");
	I_END_COMPONENT

	CMeilhausSimpleComp();

	// reimplemented (isig::ISamplesProcessor)
	virtual istd::CRange GetValueRange(
				bool forInput = true,
				bool forOutput = true,
				const iprm::IParamsSet* paramsSetPtr = NULL) const;
	virtual int GetMaximalSamplesCount(
				bool forInput = true,
				bool forOutput = true,
				const iprm::IParamsSet* paramsSetPtr = NULL) const;

	// reimplemented (iproc::TIAssyncProcessor)
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const;
	virtual void ResetAllTasks();
	virtual bool AreParamsAccepted(const iprm::IParamsSet* paramsPtr) const;
	virtual int BeginTask(
				const iprm::IParamsSet* paramsPtr,
				const InputType* inputPtr,
				OutputType* outputPtr);
	virtual int WaitTaskFinished(
				int taskId = -1,
				double timeoutTime = -1,
				bool killOnTimeout = true);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

	// reimplemented (imebase::IMeilhausAccess)
	virtual bool CreateSelectionTree(CChannelSelectionNode& result) const;
	virtual istd::CRange GetIntervalRange() const;
	virtual bool IsSamplingModeSupported(int mode) const;

protected:
	int PullNextTaskId();
	bool GetChannelAddress(const iprm::IParamsSet* paramsPtr, CMeAddr& result) const;
	const isig::ISamplingParams* GetSamplingParams(const iprm::IParamsSet* paramsPtr) const;

	virtual WaitAllTasksFinished(double timeoutTime, bool killOnTimeout);
	virtual WaitSingleTaskFinished(int taskId, double timeoutTime, bool killOnTimeout);

private:
	I_ATTR(bool, m_isOutputAttrPtr);

	I_ATTR(istd::CString, m_addressParamIdAttrPtr);
	I_ATTR(istd::CString, m_samplingParamsIdAttrPtr);

	typedef QList<CMeContext*> CMeContextList;
	CMeContextList m_activeTaskList;

	int m_lastTaskId;
};


} // namespace imebase


#endif // !imebase_CMeilhausSimpleComp_included

