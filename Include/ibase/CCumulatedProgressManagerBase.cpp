#include <ibase/CCumulatedProgressManagerBase.h>


// STD includes
#include <algorithm>

// Qt includes
#include <QtCore/QMutexLocker>

// ACF includes
#include <istd/CChangeNotifier.h>


namespace ibase
{


/**
	Manager of subtasks being created by \c CCumulatedProgressManagerBase.
*/
class SubTaskManager: public CCumulatedProgressManagerBase::TaskBase, public CCumulatedProgressManagerBase
{
public:
	SubTaskManager(CCumulatedProgressManagerBase* parentPtr, const TaskInfo& taskInfo, double weight, bool isCancelable) : CCumulatedProgressManagerBase::TaskBase(parentPtr, taskInfo, weight, isCancelable)	{}

protected:
	// reimplemented (ibase::CCumulatedProgressManagerBase)
	void OnProgressChanged(double cumulatedValue) override
	{
		if (m_parentPtr != nullptr) {
			m_parentPtr->ReportTaskProgress(this, cumulatedValue);
		}
	}
};


// public methods

CCumulatedProgressManagerBase::CCumulatedProgressManagerBase(const TaskInfo& defaultTaskInfo)
:	m_defaultTaskInfo(defaultTaskInfo),
	m_currentProgress(0),
	m_closedTaskSum(0),
	m_maxProgressSum(0),
	m_isProgressLoggerStarted(false),
	m_cancelableCounter(0),
	m_isCanceled(false)
{
}


CCumulatedProgressManagerBase::~CCumulatedProgressManagerBase()
{
	QMutexLocker locker(&m_tasksMutex);

	for (auto&& task: m_openTasks) {
		if (task.first != nullptr) {
			task.first->StopLogging();
		}
	}
}


void CCumulatedProgressManagerBase::SetDefaultTaskInfo(const TaskInfo& defaultTaskInfo)
{
	m_defaultTaskInfo = defaultTaskInfo;
}


double CCumulatedProgressManagerBase::GetCumulatedProgress() const
{
	QMutexLocker locker(&m_tasksMutex);

	return m_currentProgress;
}


std::vector<CCumulatedProgressManagerBase::TaskInfo> CCumulatedProgressManagerBase::GetProcessedTasks(bool preferSorted, int maxCount) const
{
	QMutexLocker locker(&m_tasksMutex);

	std::vector<CCumulatedProgressManagerBase::ProgressInfo> taskList;
	for (auto&& iter: m_openTasks) {
		taskList.push_back(iter.second);
	}

	if (preferSorted) {
		std::sort(taskList.begin(), taskList.end(), [](auto&& first, auto&& second) {return first.weight < second.weight;});
	}

	if (maxCount > 0) {
		taskList.resize(maxCount);
	}

	std::vector<CCumulatedProgressManagerBase::TaskInfo> retVal;
	for (auto&& pinfo: taskList) {
		retVal.push_back(pinfo.taskInfo);
	}

	return retVal;
}


bool CCumulatedProgressManagerBase::IsCancelable() const
{
	return m_cancelableCounter > 0;
}


bool CCumulatedProgressManagerBase::IsCanceled() const
{
	return m_isCanceled;
}


void CCumulatedProgressManagerBase::SetCanceled(bool state)
{
	m_isCanceled = state;
}



// reimplemented (ibase::IProgressManager)

void CCumulatedProgressManagerBase::ResetProgressManager()
{
	QMutexLocker locker(&m_tasksMutex);

	for (auto&& task: m_openTasks) {
		if (task.first != nullptr) {
			task.first->StopLogging();
		}
	}

	m_currentProgress = 0;
	m_closedTaskSum = 0;
	m_maxProgressSum = 0;
	m_isProgressLoggerStarted = false;
	m_cancelableCounter = 0;
	m_isCanceled = false;

	m_openTasks.clear();
	locker.unlock();
}


std::unique_ptr<IProgressManager> CCumulatedProgressManagerBase::CreateSubtaskManager(
		const QByteArray& taskId,
		const QString& taskDescription,
		double weight)
{
	return std::unique_ptr<IProgressManager>(new SubTaskManager(this, TaskInfo{taskId, taskDescription}, weight, false));
}


std::unique_ptr<IProgressLogger> CCumulatedProgressManagerBase::StartProgressLogger(bool isCancelable)
{
	Q_ASSERT_X(!m_isProgressLoggerStarted, "StartProgressLogger", "Main progress logger can be started only once for progress manager!");

	m_isProgressLoggerStarted = true;

	return std::unique_ptr<IProgressLogger>(new Logger(this, m_defaultTaskInfo, isCancelable));
}


// protected methods

// Methods designed to be overriden

void CCumulatedProgressManagerBase::OnProgressChanged(double /*cumulatedValue*/)
{
}


void CCumulatedProgressManagerBase::OnTasksChanged()
{
}


void CCumulatedProgressManagerBase::OpenTask(TaskBase* taskPtr, const TaskInfo& taskInfo, double weight, bool isCancelable)
{
	QMutexLocker locker(&m_tasksMutex);

	m_openTasks[taskPtr] = ProgressInfo{0, weight, isCancelable, taskInfo};

	if (isCancelable) {
		m_cancelableCounter++;
	}

	m_maxProgressSum += weight;

	locker.unlock();

	OnTasksChanged();

	TryUpdateCumulatedProgress();
}


void CCumulatedProgressManagerBase::CloseTask(TaskBase* taskPtr)
{
	QMutexLocker locker(&m_tasksMutex);

	if (auto taskIter = m_openTasks.find(taskPtr); taskIter != m_openTasks.end()) {
		if (taskIter->second.isCancelable) {
			m_cancelableCounter--;
		}

		m_closedTaskSum += taskIter->second.weight;

		m_openTasks.erase(taskPtr);
	}

	locker.unlock();

	TryUpdateCumulatedProgress();

	OnTasksChanged();
}


void CCumulatedProgressManagerBase::ReportTaskProgress(TaskBase* taskPtr, double progress)
{
	QMutexLocker locker(&m_tasksMutex);

	if (auto taskIter = m_openTasks.find(taskPtr); taskIter != m_openTasks.end()) {
		taskIter->second.progress = progress;
	}

	locker.unlock();

	TryUpdateCumulatedProgress();
}


void CCumulatedProgressManagerBase::TryUpdateCumulatedProgress()
{
	double progressValue = 0;

	QMutexLocker locker(&m_tasksMutex);
	if (m_maxProgressSum > 0) {
		progressValue = m_closedTaskSum;

		for (const auto& task : m_openTasks) {
			const auto& taskInfo = task.second;
			progressValue += (taskInfo.progress * taskInfo.weight);
		}
	}
	locker.unlock();

	if (progressValue != m_currentProgress) {
		m_currentProgress = progressValue;

		OnProgressChanged(progressValue);
	}
}


// public methods of embedded class TaskBase

CCumulatedProgressManagerBase::TaskBase::TaskBase(CCumulatedProgressManagerBase* parentPtr, const TaskInfo& taskInfo, double weight, bool isCancelable)
:	m_parentPtr(parentPtr)
{
	Q_ASSERT(m_parentPtr != nullptr);

	m_parentPtr->OpenTask(this, taskInfo, weight, isCancelable);
}


void CCumulatedProgressManagerBase::TaskBase::StopLogging()
{
	m_parentPtr = nullptr;
}


CCumulatedProgressManagerBase::TaskBase::~TaskBase()
{
	if (m_parentPtr != nullptr) {
		m_parentPtr->CloseTask(this);
	}
}


// public methods of embedded class Logger

CCumulatedProgressManagerBase::Logger::Logger(CCumulatedProgressManagerBase* parentPtr, const TaskInfo& taskInfo, bool isCancelable)
:	TaskBase(parentPtr, taskInfo, 1, isCancelable)
{
}


// reimplemented (ibase::IProgressLogger)

void CCumulatedProgressManagerBase::Logger::OnProgress(double currentProgress)
{
	if (m_parentPtr != nullptr) {
		m_parentPtr->ReportTaskProgress(this, currentProgress);
	}
}


bool CCumulatedProgressManagerBase::Logger::IsCanceled() const
{
	if (m_parentPtr != nullptr) {
		m_parentPtr->IsCanceled();
	}

	return false;
}


} // namespace ibase


