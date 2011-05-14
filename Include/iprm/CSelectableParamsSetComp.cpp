#include "iprm/CSelectableParamsSetComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"


namespace iprm
{


CSelectableParamsSetComp::CSelectableParamsSetComp()
:	m_selectedIndex(-1),
	m_currentParamsSetObserver(*this),
	m_currentParamsModelPtr(NULL)
{
}


// reimplemented (iprm::IParamsSet)

const iser::ISerializable* CSelectableParamsSetComp::GetParameter(const std::string& id) const
{
	I_ASSERT(m_selectionIdAttrPtr.IsValid());

	const istd::CString& selectionId = *m_selectionIdAttrPtr;
	if (id == selectionId.ToString()){
		return static_cast<const ISelectionParam*>(this);
	}

	if (m_paramsManagerCompPtr.IsValid() && (m_selectedIndex >= 0) && (m_selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount())){
		const IParamsSet* paramsSetPtr = m_paramsManagerCompPtr->GetParamsSet(m_selectedIndex);
		if (paramsSetPtr != NULL){
			return paramsSetPtr->GetParameter(id);
		}
	}

	return NULL;
}


iser::ISerializable* CSelectableParamsSetComp::GetEditableParameter(const std::string& id)
{
	I_ASSERT(m_selectionIdAttrPtr.IsValid());

	const istd::CString& selectionId = *m_selectionIdAttrPtr;
	if (id == selectionId.ToString()){
		return static_cast<ISelectionParam*>(this);
	}

	return NULL;
}


// reimplemented (iprm::ISelectionParam)

const ISelectionConstraints* CSelectableParamsSetComp::GetConstraints() const
{
	return this;
}


int CSelectableParamsSetComp::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CSelectableParamsSetComp::SetSelectedOptionIndex(int index)
{
	if (index < GetOptionsCount()){
		if (index != m_selectedIndex){
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED);

			m_selectedIndex = index;

			SetupCurrentParamsSetBridge();
		}

		return true;
	}

	return false;
}


ISelectionParam* CSelectableParamsSetComp::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectableParamsSetComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag selectedIndexTag("Selected", "Selected index");

	bool retVal = true;

	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(m_selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	return retVal;
}


// protected methods

// reimplemented (iprm::ISelectionConstraints)

int CSelectableParamsSetComp::GetOptionsCount() const
{
	if (m_paramsManagerCompPtr.IsValid()){
		return m_paramsManagerCompPtr->GetParamsSetsCount();
	}

	return 0;
}


const istd::CString& CSelectableParamsSetComp::GetOptionName(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		return m_paramsManagerCompPtr->GetParamsSetName(index);
	}

	static istd::CString noname("<noname>");

	return noname;
}


// private methods

void CSelectableParamsSetComp::SetupCurrentParamsSetBridge()
{
	const imod::IModel* currentParamsModelPtr = NULL;

	if (m_selectedIndex >= 0 && m_selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount()){
		currentParamsModelPtr = dynamic_cast<const imod::IModel*>((m_paramsManagerCompPtr->GetParamsSet(m_selectedIndex)));
		if (currentParamsModelPtr != m_currentParamsModelPtr){
			if (m_currentParamsModelPtr != NULL && m_currentParamsModelPtr->IsAttached(&m_currentParamsSetObserver)){
				m_currentParamsModelPtr->DetachObserver(&m_currentParamsSetObserver);

				m_currentParamsModelPtr = NULL;
			}

			if (currentParamsModelPtr != NULL && const_cast<imod::IModel*>(currentParamsModelPtr)->AttachObserver(&m_currentParamsSetObserver)){
				m_currentParamsModelPtr  = const_cast<imod::IModel*>(currentParamsModelPtr);
			}
		}
	}
}


// public methods of the embedded class CurrentParamsSetObserver

CSelectableParamsSetComp::CurrentParamsSetObserver::CurrentParamsSetObserver(CSelectableParamsSetComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CSelectableParamsSetComp::CurrentParamsSetObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_parent.BeginChanges(updateFlags | istd::IChangeable::CF_DELEGATED, updateParamsPtr);
}


void CSelectableParamsSetComp::CurrentParamsSetObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_parent.EndChanges(updateFlags | istd::IChangeable::CF_DELEGATED, updateParamsPtr);
}


} // namespace iprm


