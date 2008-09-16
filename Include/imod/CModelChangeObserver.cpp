#include "imod/CModelChangeObserver.h"


namespace imod
{


// public methods

CModelChangeObserver::CModelChangeObserver()
:	m_isDirty(false),
	m_modelObserver(*this)
{
}


CModelChangeObserver::CModelChangeObserver(imod::IModel* modelPtr)
:	m_isDirty(false),
	m_modelObserver(*this)
{
	SetModelPtr(modelPtr);
}


void CModelChangeObserver::SetModelPtr(imod::IModel* modelPtr)
{
	if (modelPtr != m_modelObserver.GetModelPtr()){
		m_modelObserver.EnsureDetached();

		if (modelPtr != NULL){
			modelPtr->AttachObserver(&m_modelObserver);
		}
	}
}


// protected methods

bool CModelChangeObserver::IsDirty() const
{
	return m_isDirty;
}


void CModelChangeObserver::SetDirty(bool isDirty)
{
	m_isDirty = isDirty;
}


// public methods of embedded class Observer

CModelChangeObserver::Observer::Observer(imod::CModelChangeObserver& parent)
:	m_parent(parent)
{
}


// protected methods of embedded class Observer

// reimplemented (TSingleModelObserverBase)

void CModelChangeObserver::Observer::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.SetDirty(true);
}


} // namespace imod

