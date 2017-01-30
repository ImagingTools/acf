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
	Q_ASSERT(modelIndex >= 0);
	Q_ASSERT(modelIndex < GetModelCount());

	return m_models.at(modelIndex);
}


int CModelUpdateBridge::GetModelCount() const
{
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
	if (modelPtr == NULL){
		return !m_models.isEmpty();
	}

	Models::const_iterator foundIter = qFind(m_models.begin(), m_models.end(), modelPtr);

	return (foundIter != m_models.end());
}


bool CModelUpdateBridge::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(modelPtr != NULL);

	if (IsModelAttached(modelPtr)){
		return false;
	}

	m_models.push_back(modelPtr);

	changeMask = istd::IChangeable::GetAllChanges();

	return true;
}


bool CModelUpdateBridge::OnModelDetached(IModel* modelPtr)
{
	Models::iterator iter = qFind(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);
		
		return true;
	}

	return false;
}


void CModelUpdateBridge::BeforeUpdate(IModel* I_IF_DEBUG(modelPtr))
{
	I_IF_DEBUG(Q_ASSERT(IsModelAttached(modelPtr)));

	istd::IChangeable::ChangeSet changeSet = istd::IChangeable::GetAnyChange();
	if (m_updateFlags & UF_DELEGATED){
		changeSet = istd::IChangeable::GetDelegatedChanges();
	}

	m_changeablePtr->BeginChanges(changeSet);
}


void CModelUpdateBridge::AfterUpdate(IModel* I_IF_DEBUG(modelPtr), const istd::IChangeable::ChangeSet& changeSet)
{
	I_IF_DEBUG(Q_ASSERT(IsModelAttached(modelPtr)));

	istd::IChangeable::ChangeSet changes(changeSet.GetDescription());
	if (m_updateFlags & UF_DELEGATED){
		changes += istd::IChangeable::GetDelegatedChanges();
	}

	if (m_updateFlags & UF_SOURCE){
		changes += changeSet;
	}

	m_changeablePtr->EndChanges(changes);
}


} // namespace imod

