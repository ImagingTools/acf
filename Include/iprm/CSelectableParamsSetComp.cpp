#include "iprm/CSelectableParamsSetComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iprm
{


CSelectableParamsSetComp::CSelectableParamsSetComp()
:	m_selectedIndex(-1)
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
			istd::CChangeNotifier notifier(this);

			m_selectedIndex = index;
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



} // namespace iprm


