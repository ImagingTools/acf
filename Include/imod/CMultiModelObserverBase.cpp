#include <imod/CMultiModelObserverBase.h>


namespace imod
{


// public methods

CMultiModelObserverBase::CMultiModelObserverBase()
:	m_observedIds(istd::IChangeable::GetAllChanges())
{
}


CMultiModelObserverBase::~CMultiModelObserverBase()
{
	CMultiModelObserverBase::EnsureModelsDetached();
}


IModel* CMultiModelObserverBase::GetObservedModel(int modelIndex) const
{
	QMutexLocker lock(&m_mutex);

	Q_ASSERT(modelIndex >= 0);
	Q_ASSERT(modelIndex < m_models.size());

	return m_models.at(modelIndex);
}


IModel* CMultiModelObserverBase::GetModelPtr(int modelIndex) const
{
	return GetObservedModel(modelIndex);
}


int CMultiModelObserverBase::GetModelCount() const
{
	QMutexLocker lock(&m_mutex);

	return int(m_models.size());
}


void CMultiModelObserverBase::EnsureModelsDetached()
{
	while (!m_models.isEmpty()){
		imod::IModel* modelPtr = m_models.front();
		Q_ASSERT(modelPtr != NULL);

		modelPtr->DetachObserver(this);
	}
}


void CMultiModelObserverBase::SetObservedIds(const istd::IChangeable::ChangeSet& changeMask)
{
	m_observedIds = changeMask;
}


// reimplemented (imod::IObserver)

bool CMultiModelObserverBase::IsModelAttached(const imod::IModel* modelPtr) const
{
	QMutexLocker lock(&m_mutex);

	return IsAttached(modelPtr);
}


bool CMultiModelObserverBase::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	QMutexLocker lock(&m_mutex);

	Q_ASSERT(modelPtr != NULL);

	if (!IsAttached(modelPtr)){
		m_models.push_back(modelPtr);

		changeMask = m_observedIds;

		return true;
	}

	return false;
}


bool CMultiModelObserverBase::OnModelDetached(IModel* modelPtr)
{
	QMutexLocker lock(&m_mutex);

	Models::iterator iter = qFind(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);
		return true;
	}

	return false;
}

void CMultiModelObserverBase::BeforeUpdate(IModel* I_IF_DEBUG(modelPtr))
{
	I_IF_DEBUG(Q_ASSERT(IsModelAttached(modelPtr)));
}


void CMultiModelObserverBase::AfterUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	QMutexLocker lock(&m_mutex);

	Q_ASSERT(IsModelAttached(modelPtr));

	if (IsAttached(modelPtr)){
		OnUpdate(modelPtr, changeSet);
	}
}


// private methods

bool CMultiModelObserverBase::IsAttached(const imod::IModel* modelPtr) const
{
	if (modelPtr == NULL){
		return !m_models.isEmpty();
	}

	Models::const_iterator foundIter = qFind(m_models.begin(), m_models.end(), modelPtr);

	return (foundIter != m_models.end());
}


// protected methods

void CMultiModelObserverBase::OnUpdate(imod::IModel* /*modelPtr*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
}


} // namespace imod

