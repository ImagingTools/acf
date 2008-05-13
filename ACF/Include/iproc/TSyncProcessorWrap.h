#ifndef iproc_TSyncProcessorWrap_included
#define iproc_TSyncProcessorWrap_included


// STL includes
#include <map>

#include "iproc/TIAssyncProcessor.h"


namespace iproc
{


class IParamsSet;


/**
	Wrapper of \c iproc::TIAssyncProcessor for simple synchrone processor implementations.
	Synchrone processors process its data direct while method \c BeginTask is called.
*/
template <class Base, class Input, class Output>
class TSyncProcessorWrap: public Base
{
public:
	TSyncProcessorWrap();

	// pseudo-reimplemented (iproc::TIAssyncProcessor)
	virtual int GetProcessorState(const IParamsSet* paramsPtr) const;
	virtual void ResetAllTasks();
	virtual bool AreParamsAccepted(const IParamsSet* paramsPtr) const;
	virtual int BeginTask(
				const IParamsSet* paramsPtr,
				const typename TIAssyncProcessor<Input, Output>::InputType* inputPtr,
				typename TIAssyncProcessor<Input, Output>::OutputType* outputPtr);
	virtual int WaitTaskFinished(
					int taskId = -1,
					double timeoutTime = -1,
					bool killOnTimeout = true);
	virtual int GetReadyTask();
	virtual int GetTaskState(int taskId = -1) const;
	virtual void InitProcessor(const IParamsSet* paramsPtr);

protected:
	// abstract methods
	/**
		Do synchronized processing.
		\return					state of processing task \sa TaskState.
	*/
	virtual int DoSyncProcess(
				const IParamsSet* paramsPtr,
				const typename TIAssyncProcessor<Input, Output>::InputType* inputPtr,
				typename TIAssyncProcessor<Input, Output>::OutputType* outputPtr) = 0;

private:
	typedef std::map<int, int> TaskToStateMap;

	TaskToStateMap m_taskToStateMap;
	int m_nextTaskId;
};


// public methods

template <class Base, class Input, class Output>
TSyncProcessorWrap<Base, Input, Output>::TSyncProcessorWrap()
:	m_nextTaskId(0)
{
}


// pseudo-reimplemented (iproc::TIAssyncProcessor)

template <class Base, class Input, class Output>
int TSyncProcessorWrap<Base, Input, Output>::GetProcessorState(const IParamsSet* /*paramsPtr*/) const
{
	return PS_READY;
}


template <class Base, class Input, class Output>
void TSyncProcessorWrap<Base, Input, Output>::ResetAllTasks()
{
	m_taskToStateMap.clear();
}


template <class Base, class Input, class Output>
bool TSyncProcessorWrap<Base, Input, Output>::AreParamsAccepted(const IParamsSet* /*paramsPtr*/) const
{
	return true;
}


template <class Base, class Input, class Output>
int TSyncProcessorWrap<Base, Input, Output>::BeginTask(
			const IParamsSet* paramsPtr,
			const typename TIAssyncProcessor<Input, Output>::InputType* inputPtr,
			typename TIAssyncProcessor<Input, Output>::OutputType* outputPtr)
{
	int retVal = m_nextTaskId;
	m_taskToStateMap[retVal] = DoSyncProcess(paramsPtr, inputPtr, outputPtr);

	m_nextTaskId = (m_nextTaskId + 1) & 0x7fff;

	return retVal;
}


template <class Base, class Input, class Output>
int TSyncProcessorWrap<Base, Input, Output>::WaitTaskFinished(
			int taskId,
			double /*timeoutTime*/,
			bool /*killOnTimeout*/)
{
	int retVal = TS_NONE;

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

		ResetAllTasks();
	}

	return retVal;
}


template <class Base, class Input, class Output>
int TSyncProcessorWrap<Base, Input, Output>::GetReadyTask()
{
	if (!m_taskToStateMap.empty()){
		return m_taskToStateMap.begin()->first;
	}
	else{
		return -1;
	}
}


template <class Base, class Input, class Output>
int TSyncProcessorWrap<Base, Input, Output>::GetTaskState(int taskId) const
{
	int retVal = TS_NONE;

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


template <class Base, class Input, class Output>
void TSyncProcessorWrap<Base, Input, Output>::InitProcessor(const IParamsSet* /*paramsPtr*/)
{
}


} // namespace iproc


#endif // !iproc_TSyncProcessorWrap_included


