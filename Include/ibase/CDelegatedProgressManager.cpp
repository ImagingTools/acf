#include "ibase/CDelegatedProgressManager.h"


// ACF includes
#include "istd/CChangeNotifier.h"


namespace ibase
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
			const QByteArray& progressId,
			const QString& description,
			bool isCancelable)
:	m_nextSessionId(0),
	m_progressSum(0.0),
	m_cancelableSessionsCount(0),
	m_slaveManagerPtr(slaveManagerPtr)
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
	if (m_progressMap.isEmpty()){
		return 0.0;
	}

	return m_progressSum / m_progressMap.size();
}


// reimplemented (ibase::IProgressManager)

int CDelegatedProgressManager::BeginProgressSession(
			const QByteArray& /*progressId*/,
			const QString& /*description*/,
			bool isCancelable)
{
	ChangeSet changeSet(CF_SESSIONS_NUMBER, CF_PROGRESS_CHANGED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

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
	ChangeSet changeSet(CF_SESSIONS_NUMBER, CF_PROGRESS_CHANGED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	ProgressMap::iterator iter = m_progressMap.find(sessionId);
	Q_ASSERT(iter != m_progressMap.constEnd());

	const ProgressInfo& info = iter.value();
	m_progressSum -= info.progress;

	if (info.isCancelable){
		--m_cancelableSessionsCount;

		if (m_cancelableSessionsCount == 0){
			OnCancelable(false);
		}
	}

	m_progressMap.erase(iter);

	if (m_progressMap.isEmpty()){
		m_progressSum = 0;
	}
}


void CDelegatedProgressManager::OnProgress(int sessionId, double currentProgress)
{
	Q_ASSERT(m_progressMap.contains(sessionId));

	ChangeSet changeSet(CF_PROGRESS_CHANGED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	ProgressInfo& value = m_progressMap[sessionId];

	m_progressSum += currentProgress - value.progress;
	value.progress = currentProgress;
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

void CDelegatedProgressManager::OnEndChanges(const ChangeSet& /*changeSet*/)
{
	if (m_slaveManagerPtr != NULL){
		m_slaveManagerPtr->OnProgress(m_slaveSessionId, GetCumulatedProgress());
	}
}


} // namespace ibase


