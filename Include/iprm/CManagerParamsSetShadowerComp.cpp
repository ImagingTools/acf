#include "iprm/CManagerParamsSetShadowerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"


namespace iprm
{


CManagerParamsSetShadowerComp::CManagerParamsSetShadowerComp()
:	m_currentParamsSetObserver(this),
	m_currentParamsModelPtr(NULL)
{
}


// protected methods

bool CManagerParamsSetShadowerComp::CopyParamsSetToShadow()
{
	if (m_shadowParamsSetCompPtr.IsValid() && m_paramsManagerCompPtr.IsValid() && m_selectionParamCompPtr.IsValid()){
		int index = m_selectionParamCompPtr->GetSelectedOptionIndex();
		if ((index >= 0) && (index < m_paramsManagerCompPtr->GetParamsSetsCount())){
			const IParamsSet* selectedSetPtr = m_paramsManagerCompPtr->GetParamsSet(index);
			if (selectedSetPtr != NULL){
				m_shadowParamsSetCompPtr->CopyFrom(*selectedSetPtr);

				return true;
			}
		}
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CManagerParamsSetShadowerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (		m_paramsManagerCompPtr.IsValid() &&
				m_paramsManagerModelCompPtr.IsValid() &&
				m_selectionParamCompPtr.IsValid() &&
				m_shadowParamsSetCompPtr.IsValid()){
		m_paramsManagerModelCompPtr->AttachObserver(this);
	}
}


void CManagerParamsSetShadowerComp::OnComponentDestroyed()
{
	imod::CSingleModelObserverBase::EnsureModelDetached();
	m_currentParamsSetObserver.EnsureModelDetached();
	m_currentParamsModelPtr = NULL;

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CSingleModelObserverBase)

void CManagerParamsSetShadowerComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	Q_ASSERT(m_paramsManagerCompPtr.IsValid());
	Q_ASSERT(m_selectionParamCompPtr.IsValid());

	int index = m_selectionParamCompPtr->GetSelectedOptionIndex();
	if ((index >= 0) && (index < m_paramsManagerCompPtr->GetParamsSetsCount())){
		imod::IModel* paramsModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>((
					m_paramsManagerCompPtr->GetParamsSet(index))));
		if (paramsModelPtr != m_currentParamsModelPtr){
			m_currentParamsSetObserver.EnsureModelDetached();
			m_currentParamsModelPtr = NULL;

			if ((paramsModelPtr != NULL) && paramsModelPtr->AttachObserver(&m_currentParamsSetObserver)){
				m_currentParamsModelPtr  = paramsModelPtr;
			}
		}
	}

	CopyParamsSetToShadow();
}


// public methods of the embedded class CurrentParamsSetObserver

CManagerParamsSetShadowerComp::CurrentParamsSetObserver::CurrentParamsSetObserver(CManagerParamsSetShadowerComp* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

void CManagerParamsSetShadowerComp::CurrentParamsSetObserver::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.CopyParamsSetToShadow();
}


} // namespace iprm


