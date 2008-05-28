#ifndef iproc_TIAssyncProcessor_included
#define iproc_TIAssyncProcessor_included


#include "iprm/IParamsSet.h"


namespace iproc
{


/**
	Template interface for all assynchrone processors.
	Assynchrone processors can process many tasks parallel. Each task is identified using its ID.
	New assynchrone processing task can be started using method \c BeginTask and finish using \c WaitTaskFinished or \c ResetAllTasks.
	Each task has its input object, output object and parameter set.
	\param	Input	type of input object.
	\param	Output	type of output object.
*/
template <class Input, class Output>
class TIAssyncProcessor: virtual public istd::IPolymorphic
{
public:
	typedef Input InputType;
	typedef Output OutputType;

	/**
		State of processing.
	*/
	enum ProcessorState{
		/**
			Processing state is unknown or not initialized.
		 */
		PS_UNKNOWN,
		/**
			Processor is during initalisation.
		 */
		PS_INITIALIZATION,
		/**
			Processor is initialized and ready.
		 */
		PS_READY,
		/**
			Processor cannot be initialized.
		 */
		PS_INVALID
	};

	/**
		Describe state of task.
	 */
	enum TaskState{
		/**
			Task state is undefined.
		 */
		TS_NONE,
		/**
			Task is during execution.
		 */
		TS_WAIT,
		/**
			Task finished correctly.
		 */
		TS_OK,
		/**
			Task was canceled by user.
		 */
		TS_CANCELED,
		/**
			Task finished but error is occured.
		 */
		TS_INVALID
	};

	/**
		Get state of this processor for processing of specified parameters.
		\param	paramsPtr	pointer to parameter object or NULL if general state is mean.
		\return				state of this processor \sa ProcessorState.
	 */
	virtual int GetProcessorState(const iprm::IParamsSet* paramsPtr) const = 0;

	/**
		Remove all task from queue.
	 */
	virtual void ResetAllTasks() = 0;

	/**
		Check if specified parameter set is accepted for this processor.
	 */
	virtual bool AreParamsAccepted(const iprm::IParamsSet* paramsPtr) const = 0;

	/**
		Begin new task for this processor and add them to queue.
		If this method success, new task is added into the queue.
							To remove it from queue you have to call \c WaitTaskFinished or \c ResetAllTasks.
		\param	paramsPtr	set of parameters controlling processing task.
							Pointed object can be used till task is finished.
							It can be NULL.
		\param	inputPtr	input object typically being source object used to processing.
							Pointed object can be used till task is finished.
							It can be NULL.
		\param	outputPtr	output object storing processing result.
							Pointed object can be used till task is finished.
							It can be NULL.
		\return				task ID or -1 if it was not possible to add new task.
	*/
	virtual int BeginTask(const iprm::IParamsSet* paramsPtr, const InputType* inputPtr, OutputType* outputPtr) = 0;

	/**
		Wait for task is finished.
		\param	taskId			ID of task returned by BeginTask() or -1 if all tasks are mean.
		\param	timeoutTime		maximal waiting time in seconds or negative value if timeout is not allowed.
								If this time will be exceeded, it returns TS_WAIT or TS_INVALID if killOnTimeout is set.
		\param	killOnTimeout	indicate that task must be killed by timeout.
		\return					state of selected task or all tasks \sa TaskState.
	*/
	virtual int WaitTaskFinished(
					int taskId = -1,
					double timeoutTime = -1,
					bool killOnTimeout = true) = 0;

	/**
		Get the ID of any ready task.
		Also for ready task it is necessary to call WaitTaskFinished().
		\return	ID of ready task or -1.
	*/
	virtual int GetReadyTask() = 0;

	/**
		Get actual camera state.
		Please note that image state will be only stored
		till WaitSnapFinished() is called.
		\param	frameId	frame ID number returned by AddSnap() method.
						If it is -1 state of the last camera will be returned.
		\return					state of selected task or all tasks \sa TaskState.
	*/
	virtual int GetTaskState(int taskId = -1) const = 0;

	/**
		Init processor to optimal process with specified params.
		Please note, that initalizing will be also done automatically in method \c BeginTask
		but, to avoid unpredictable waiting time during time critical processing,
		it is recommended to use this method while parameter initialization.
		\param	paramsPtr	set of parameters controlling processing task.
							Pointed object can be used till task is finished.
							It can be NULL.
	*/
	virtual void InitProcessor(const iprm::IParamsSet* paramsPtr) = 0;
};


} // namespace iproc


#endif // !iproc_TIAssyncProcessor_included


