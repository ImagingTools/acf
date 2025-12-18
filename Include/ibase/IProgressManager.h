#ifndef ibase_IProgressManager_included
#define ibase_IProgressManager_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QString>


// STD includes
#include <memory>

// ACF includes
#include <istd/IPolymorphic.h>
#include <ibase/IProgressLogger.h>


namespace ibase
{


/**
	Consume information about progress of some process.
*/
class IProgressManager: virtual public istd::IPolymorphic
{
public:
	/**
		Reset progress manager and make it possible to reuse it.
	*/
	virtual void ResetProgressManager() = 0;
	/**
		Create progress manager for the subtask.
		This function should be called up at the start of the task for all subtasks.
		\param	taskId			unique ID of this task.
								This ID can be used by the progress bar to distinguish between different tasks.
		\param	taskDescription	human readable description of this task.
		\param	weight			weight value
	*/
	virtual std::unique_ptr<IProgressManager> CreateSubtaskManager(
			const QByteArray& taskId,
			const QString& taskDescription,
			double weight = 1.0) = 0;

	/**
		Create progress logger.
		This function should be called once at the start of the task if it provides progress information.
		\param	isCancelable	informs, that this task can be cancel during the processing.
	*/
	virtual std::unique_ptr<IProgressLogger> StartProgressLogger(bool isCancelable = false, const QString& description = {}) = 0;
};


} // namespace ibase


#endif // !ibase_IProgressManager_included


