#include "iproc/CDelegatedProgressManager.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iproc
{


CDelegatedProgressManager::CDelegatedProgressManager()
:	m_nextSessionId(0),
	m_progressSum(0.0),
	m_cancelableSessionsCount(0),
	m_slaveManagerPtr(NULL),
	m_slaveSessionId(-1)
{
}


CDelegatedProgressManager::CDelegatedProgressManager(
			IProgressManager* slaveManagerPtr,
			const std::string& progressId,
			const QString& description,
			bool isCancelable)
:	m_slaveManagerPtr(slaveManagerPtr)
{
	m_slaveSessionId = m_slaveManagerPtr->BeginProgressSession(progressId, description, isCancelable);
	if (m_slaveSessionId < 0){
		m_slaveManagerPtr = NULL;
	}
}


CDelegatedProgressManager::~CDelegatedProgressManager()
{
	if (m_slaveManagerPtr != NULL){
		m_slaveManagerPtr->EndProgressSession(m_slaveSessionId);
	}

	m_slaveManagerPtr = NULL;
	m_slaveSessionId = -1;
}


double CDelegatedProgressManager::GetCumulatedProgress() const
{
	return m_progressSum / m_progressMap.size();
}


// reimplemented (iproc::IProgressManager)

int CDelegatedProgressManager::BeginProgressSession(
			const std::string& /*progressId*/,
			const QString& /*description*/,
			bool isCancelable)
{
	istd::CChangeNotifier notifier(this, CF_SESSIONS_NUMBER | CF_PROGRESS_CHANGED);

	int id = m_nextSessionId++;

	ProgressInfo& info = m_progressMap[id];
	info.progress = 0;
	info.isCancelable = isCancelable;

	if (isCancelable){
		if (m_cancelableSessionsCount == 0){
			OnCancelable(true);
		}

		++m_cancelableSessionsCount;
	}

	return id;
}


void CDelegatedProgressManager::EndProgressSession(int sessionId)
{
	istd::CChangeNotifier notifier(this, CF_SESSIONS_NUMBER | CF_PROGRESS_CHANGED);

	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	I_ASSERT(iter != m_progressMap.end());

	const ProgressInfo& info = iter->second;
	m_progressSum -= info.progress;

	if (info.isCancelable){
		--m_cancelableSessionsCount;

		if (m_cancelableSessionsCount == 0){
			OnCancelable(false);
		}
	}

	m_progressMap.erase(iter);
}


void CDelegatedProgressManager::OnProgress(int sessionId, double currentProgress)
{
	istd::CChangeNotifier notifier(this, CF_PROGRESS_CHANGED);

	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	I_ASSERT(iter != m_progressMap.end());

	m_progressSum += currentProgress - iter->second.progress;
	iter->second.progress = currentProgress;
}


bool CDelegatedProgressManager::IsCanceled(int /*sessionId*/) const
{
	if (m_slaveManagerPtr != NULL){
		return m_slaveManagerPtr->IsCanceled(m_slaveSessionId);
	}

	return false;
}


// protected methods

int CDelegatedProgressManager::GetOpenSessionsCount() const
{
	return int(m_progressMap.size());
}


void CDelegatedProgressManager::OnCancelable(bool /*cancelState*/)
{
}


// reimplemented (istd::IChangeable)

void CDelegatedProgressManager::OnEndChanges(int /*changeFlags*/, istd::IPolymorphic* /*changeParamsPtr*/)
{
	if (m_slaveManagerPtr != NULL){
		m_slaveManagerPtr->OnProgress(m_slaveSessionId, GetCumulatedProgress());
	}
}


} // namespace iproc


