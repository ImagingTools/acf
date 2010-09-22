#ifndef iproc_TSyncProcessorWrap_included
#define iproc_TSyncProcessorWrap_included


// STL includes
#include <map>

#include "iprm/IParamsSet.h"

#include "iproc/IProcessor.h"


namespace iproc
{


/**
	Wrapper of \c iproc::IProcessor for simple synchrone processor implementations.
	Assynchrone processing with \c BeginTask and \c WaitTaskFinished is redirected to simple synchrone processing by method \c DoProcessing.
	All you have to implement from whole interface \c iproc::IProcessor is method \c DoProcessing.
*/
template <class Base>
class TSyncProcessorWrap: public Base
{
public:
	typedef Base BaseClass;

	TSyncProcessorWrap();

	// pseudo-reimplemented (iproc::IProcessor)
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const;
	virtual void ResetAllTasks();
	virtual bool AreParamsAccepted(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				const istd::IChangeable* outputPtr) const;
	virtual int BeginTask(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				IProgressManager* progressManagerPtr = NULL);
	virtual int WaitTaskFinished(
					int taskId = -1,
					double timeoutTime = -1,
					bool killOnTimeout = true);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr);

private:
	typedef std::map<int, int> TaskToStateMap;

	TaskToStateMap m_taskToStateMap;
	int m_nextTaskId;
};


// public methods

template <class Base>
TSyncProcessorWrap<Base>::TSyncProcessorWrap()
:	m_nextTaskId(0)
{
}


// pseudo-reimplemented (iproc::IProcessor)

template <class Base>
int TSyncProcessorWrap<Base>::GetProcessorState(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return IProcessor::PS_READY;
}


template <class Base>
void TSyncProcessorWrap<Base>::ResetAllTasks()
{
	m_taskToStateMap.clear();
}


template <class Base>
bool TSyncProcessorWrap<Base>::AreParamsAccepted(
			const iprm::IParamsSet* /*paramsPtr*/,
			const istd::IPolymorphic* /*inputPtr*/,
			const istd::IChangeable* /*outputPtr*/) const
{
	return true;
}


template <class Base>
int TSyncProcessorWrap<Base>::BeginTask(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			IProgressManager* progressManagerPtr)
{
	int retVal = m_nextTaskId;
	m_taskToStateMap[retVal] = this->DoProcessing(paramsPtr, inputPtr, outputPtr, progressManagerPtr);

	m_nextTaskId = (m_nextTaskId + 1) & 0x7fff;

	return retVal;
}


template <class Base>
int TSyncProcessorWrap<Base>::WaitTaskFinished(
			int taskId,
			double /*timeoutTime*/,
			bool /*killOnTimeout*/)
{
	int retVal = IProcessor::TS_NONE;

	if (taskId >= 0){
		TaskToStateMap::iterator foundIter = m_taskToStateMap.find(taskId);
		if (foundIter != m_taskToStateMap.end()){
			retVal = foundIter->second;

			m_taskToStateMap.erase(foundIter);
		}
	}
	else{
		for (		TaskToStateMap::const_iterator iter = m_taskToStateMap.begin();
					iter != m_taskToStateMap.end();
					++iter){	// we are looking for worst status value. This values are sorted worst => bigger.
			int taskState = iter->second;
			if (taskState > retVal){
				retVal = taskState;
			}
		}

		this->ResetAllTasks();
	}

	return retVal;
}


template <class Base>
int TSyncProcessorWrap<Base>::GetReadyTask()
{
	if (!m_taskToStateMap.empty()){
		return m_taskToStateMap.begin()->first;
	}
	else{
		return -1;
	}
}


template <class Base>
int TSyncProcessorWrap<Base>::GetTaskState(int taskId) const
{
	int retVal = IProcessor::TS_NONE;

	if (taskId >= 0){
		TaskToStateMap::const_iterator foundIter = m_taskToStateMap.find(taskId);
		if (foundIter != m_taskToStateMap.end()){
			retVal = foundIter->second;
		}
	}
	else{
		for (		TaskToStateMap::const_iterator iter = m_taskToStateMap.begin();
					iter != m_taskToStateMap.end();
					++iter){	// we are looking for worst status value. This values are sorted worst => bigger.
			int taskState = iter->second;
			if (taskState > retVal){
				retVal = taskState;
			}
		}
	}

	return retVal;
}


template <class Base>
void TSyncProcessorWrap<Base>::InitProcessor(const iprm::IParamsSet* /*paramsPtr*/)
{
}


// global typdefs

typedef TSyncProcessorWrap<IProcessor> CSyncProcessorBase;


} // namespace iproc


#endif // !iproc_TSyncProcessorWrap_included


