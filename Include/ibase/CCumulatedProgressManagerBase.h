#pragma once


// STL includes
#include <vector>
#include <map>

// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include <ibase/IProgressManager.h>


namespace ibase
{


/**
	Progress manager delegating cumulated progress result to another manager.
	It allows to access cumulated progress and list of tasks beeing curently processed.
*/
class CCumulatedProgressManagerBase: virtual public ibase::IProgressManager
{
public:
	enum class TaskStatus
	{
		/**
			Task was created, but not started.
		*/
		Created,

		/**
			Task is running. Progress information is beeing reported.
		*/
		Running,

		/**
			Task was finished. No progress information will be emitted anymore.
		*/
		Finished
	};

	struct TaskInfo
	{
		QByteArray id;
		QString description;
	};

	struct TaskProgressInfo: public TaskInfo
	{
		TaskStatus status;
		double progress = 0.0;
	};

	/**
		Create cumulated progress manager.
	*/
	CCumulatedProgressManagerBase(const TaskInfo& defaultTaskInfo = {"", ""});
	virtual ~CCumulatedProgressManagerBase();

	/**
		Set information of the default task.
	*/
	void SetDefaultTaskInfo(const TaskInfo& defaultTaskInfo);

	/**
		Get final progress beeing result of all task progress.
	*/
	double GetCumulatedProgress() const;
	/**
		Get list of current processed tasks.
		\param	preferSorted	Flag indicating, that the task list should be sorted from the most important to less important.
		\param	maxCount		Maximal number of tasks should be returned.
	*/
	std::vector<TaskProgressInfo> GetProcessedTasks(bool preferSorted = false, int maxCount = -1) const;

	bool IsCancelable() const;
	virtual bool IsCanceled() const;
	void SetCanceled(bool state = true);

	// reimplemented (ibase::IProgressManager)
	virtual void ResetProgressManager() override;
	std::unique_ptr<IProgressManager> CreateSubtaskManager(
				const QByteArray& taskId,
				const QString& taskDescription,
				double weight = 1.0) override;
	virtual std::unique_ptr<IProgressLogger> StartProgressLogger(bool isCancelable = false, const QString& description = {}) override;

protected:
	// Methods designed to be overriden
	virtual void OnProgressChanged(double cumulatedValue);
	virtual void OnTasksChanged();

	class TaskBase: virtual public istd::IPolymorphic
	{
	public:
		TaskBase(CCumulatedProgressManagerBase* parentPtr, const TaskInfo& taskInfo, double weight, bool isCancelable);
		virtual ~TaskBase();

		void StopLogging();

	protected:
		CCumulatedProgressManagerBase* m_parentPtr;
	};
	friend class SubTaskManager;

	// low level communication with logger and manager
	virtual void OpenTask(TaskBase* taskPtr, const TaskInfo& taskInfo, double weight, bool isCancelable);
	virtual void CloseTask(TaskBase* taskPtr);
	virtual void ReportTaskProgress(TaskBase* taskPtr, double progress, TaskStatus taskStatus);

	void TryUpdateCumulatedProgress();

private:
	struct ProgressInfo
	{
		TaskStatus status = TaskStatus::Created;
		double progress = 0;
		double weight = 1;
		bool isCancelable = false;
		TaskInfo taskInfo;
	};

	class Logger: public TaskBase, public ibase::IProgressLogger
	{
	public:
		Logger(CCumulatedProgressManagerBase* parentPtr, const TaskInfo& taskInfo, bool isCancelable);

		// reimplemented (ibase::IProgressLogger)
		virtual void OnProgress(double currentProgress) override;
		virtual bool IsCanceled() const override;
	};

	TaskInfo m_defaultTaskInfo;

	std::map<TaskBase*, ProgressInfo> m_openTasks;

	double m_currentProgress;

	double m_closedTaskSum;
	double m_maxProgressSum;
	bool m_isProgressLoggerStarted;

	int m_cancelableCounter;
	bool m_isCanceled;

	mutable QMutex m_tasksMutex;
};


} // namespace ibase


