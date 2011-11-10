#include "iprm/CSelectableParamsSetComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"


namespace iprm
{


CSelectableParamsSetComp::CSelectableParamsSetComp()
:	m_selectedIndex(-1),
	m_currentParamsSetObserver(*this)
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

	if (m_paramsManagerCompPtr.IsValid() && (m_selectedIndex >= 0) && (m_selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount())){
		IParamsSet* paramsSetPtr = m_paramsManagerCompPtr->GetParamsSet(m_selectedIndex);
		if (paramsSetPtr != NULL){
			return paramsSetPtr->GetEditableParameter(id);
		}
	}

	return NULL;
}


// reimplemented (iprm::ISelectionParam)

const ISelectionConstraints* CSelectableParamsSetComp::GetSelectionConstraints() const
{
	return this;
}


int CSelectableParamsSetComp::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CSelectableParamsSetComp::SetSelectedOptionIndex(int index)
{
	if (!m_paramsManagerCompPtr.IsValid() && (index > 0)){
		return false;
	}

	if (index >= GetOptionsCount()){
		return false;
	}

	if (index != m_selectedIndex){
		I_ASSERT(m_paramsManagerCompPtr.IsValid());

		istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED);

		m_selectedIndex = index;

		SetupCurrentParamsSetBridge();
	}

	return true;
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

void CSelectableParamsSetComp::SetupCurrentParamsSetBridge()
{
	I_ASSERT(m_paramsManagerCompPtr.IsValid());

	if ((m_selectedIndex >= 0) && (m_selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount())){
		imod::IModel* paramsModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>((
					m_paramsManagerCompPtr->GetParamsSet(m_selectedIndex))));
		if (paramsModelPtr != NULL){
			if (m_currentParamsSetObserver.IsModelAttached(paramsModelPtr)){
				m_currentParamsSetObserver.EnsureModelDetached();

				paramsModelPtr->AttachObserver(&m_currentParamsSetObserver);
			}
		}
		else{
			m_currentParamsSetObserver.EnsureModelDetached();
		}
	}
}


// reimplemented (iprm::ISelectionConstraints)

int CSelectableParamsSetComp::GetOptionsCount() const
{
	if (m_paramsManagerCompPtr.IsValid()){
		return m_paramsManagerCompPtr->GetParamsSetsCount();
	}

	return 0;
}


istd::CString CSelectableParamsSetComp::GetOptionName(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		return m_paramsManagerCompPtr->GetParamsSetName(index);
	}

	static istd::CString noname("<noname>");

	return noname;
}

istd::CString CSelectableParamsSetComp::GetOptionDescription(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		const iprm::ISelectionConstraints* constraintsPtr = m_paramsManagerCompPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			return constraintsPtr->GetOptionDescription(index);
		}
	}

	return istd::CString();
}



// public methods of the embedded class CurrentParamsSetObserver

CSelectableParamsSetComp::CurrentParamsSetObserver::CurrentParamsSetObserver(CSelectableParamsSetComp& parent)
:	m_parent(parent)
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


