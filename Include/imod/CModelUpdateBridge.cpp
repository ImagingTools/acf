#include <imod/CModelUpdateBridge.h>


namespace imod
{


// public methods

CModelUpdateBridge::CModelUpdateBridge(istd::IChangeable* changeablePtr, int updateFlags)
:	m_changeablePtr(changeablePtr),
	m_updateFlags(updateFlags)
{
}


CModelUpdateBridge::~CModelUpdateBridge()
{
	EnsureModelsDetached();
}


IModel* CModelUpdateBridge::GetObservedModel(int modelIndex) const
{
	QMutexLocker lock(&m_mutex);

	Q_ASSERT(modelIndex >= 0);
	Q_ASSERT(modelIndex < m_models.size());

	return m_models.at(modelIndex);
}


int CModelUpdateBridge::GetModelCount() const
{
	QMutexLocker lock(&m_mutex);

	return int(m_models.size());
}


void CModelUpdateBridge::EnsureModelsDetached()
{
	while (!m_models.isEmpty()){
		imod::IModel* modelPtr = m_models.front();
		Q_ASSERT(modelPtr != NULL);

		modelPtr->DetachObserver(this);
	}
}


// reimplemented (imod::IObserver)

bool CModelUpdateBridge::IsModelAttached(const imod::IModel* modelPtr) const
{
	QMutexLocker lock(&m_mutex);

	return IsAttached(modelPtr);
}


bool CModelUpdateBridge::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(modelPtr != NULL);

	QMutexLocker lock(&m_mutex);

	if (!IsAttached(modelPtr)){
		m_models.push_back(modelPtr);

		changeMask = istd::IChangeable::GetAllChanges();

		return true;
	}

	return false;
}


bool CModelUpdateBridge::OnModelDetached(IModel* modelPtr)
{
	QMutexLocker lock(&m_mutex);

	Models::iterator iter = qFind(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);
		
		return true;
	}

	return false;
}


void CModelUpdateBridge::BeforeUpdate(IModel* modelPtr)
{
	QMutexLocker lock(&m_mutex);

	if (IsAttached(modelPtr)) {
		istd::IChangeable::ChangeSet changeSet = istd::IChangeable::GetAnyChange();
		if (m_updateFlags & UF_DELEGATED){
			changeSet = istd::IChangeable::GetDelegatedChanges();
		}

		m_changeablePtr->BeginChanges(changeSet);
	}
}


void CModelUpdateBridge::AfterUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	QMutexLocker lock(&m_mutex);

	if (IsAttached(modelPtr)) {
		istd::IChangeable::ChangeSet changes(changeSet.GetDescription());
		if (m_updateFlags & UF_DELEGATED){
			changes += istd::IChangeable::GetDelegatedChanges();
		}

		if (m_updateFlags & UF_SOURCE){
			changes += changeSet;
		}

		m_changeablePtr->EndChanges(changes);
	}
}


// private methods

bool CModelUpdateBridge::IsAttached(const imod::IModel* modelPtr) const
{
	if (modelPtr != nullptr){
		Models::const_iterator foundIter = qFind(m_models.begin(), m_models.end(), modelPtr);
		return (foundIter != m_models.end());
	}

	return !m_models.isEmpty();
}


} // namespace imod

