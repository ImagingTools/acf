#include "iprm/CSelectionParam.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iprm/IOptionsList.h"


namespace iprm
{


CSelectionParam::CSelectionParam()
:	m_selectedOptionIndex(-1),
	m_constraintsPtr(NULL),
	m_constraintsObserver(*this)
{
}


void CSelectionParam::SetSelectionConstraints(const IOptionsList* constraintsPtr)
{
	m_constraintsPtr = constraintsPtr;
	if (m_constraintsPtr != NULL){
		if (m_constraintsPtr->GetOptionsFlags() & IOptionsList::SCF_SUPPORT_UNIQUE_ID){
			imod::IModel* constraintsModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(m_constraintsPtr));
			if (constraintsModelPtr != NULL){
				constraintsModelPtr->AttachObserver(&m_constraintsObserver);
			}
		}
	}
}


void CSelectionParam::SetSubselection(int selectionIndex, ISelectionParam* selectionPtr)
{
	m_subselectionMap[selectionIndex] = selectionPtr;
}


bool CSelectionParam::SetSelectedOptionById(const QByteArray& selectedOptionId)
{
	Q_ASSERT(!selectedOptionId.isEmpty());
	Q_ASSERT(m_constraintsPtr != NULL);

	if (m_constraintsPtr != NULL){
		int optionsCount = m_constraintsPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QByteArray optionId = m_constraintsPtr->GetOptionId(optionIndex);

			if (optionId == selectedOptionId && m_constraintsPtr->IsOptionEnabled(optionIndex)){
				return SetSelectedOptionIndex(optionIndex);
			}
		}
	}

	return false;
}


int CSelectionParam::GetOptionIndexById(const QByteArray& optionId) const
{
	if (m_constraintsPtr != NULL){
		int optionsCount = m_constraintsPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QByteArray currentOptionId = m_constraintsPtr->GetOptionId(optionIndex);

			if (currentOptionId == optionId){
				return optionIndex;
			}
		}
	}

	return -1;
}


ISelectionParam* CSelectionParam::GetActiveSubselection() const
{
	return GetSubselection(m_selectedOptionIndex);
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CSelectionParam::GetSelectionConstraints() const
{
	return m_constraintsPtr;
}


int CSelectionParam::GetSelectedOptionIndex() const
{
	return m_selectedOptionIndex;
}


bool CSelectionParam::SetSelectedOptionIndex(int index)
{
	if ((m_constraintsPtr != NULL) && (index >= m_constraintsPtr->GetOptionsCount())){
		return false;
	}

	if (m_selectedOptionIndex != index){
		if (index != iprm::ISelectionParam::NO_SELECTION){
			if ((m_constraintsPtr != NULL) && !m_constraintsPtr->IsOptionEnabled(index)){
				return false;
			}
		}

		static const ChangeSet changeSet(CF_SELECTION_CHANGED, "Change selection");
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		m_selectedOptionIndex = index;

		if (m_constraintsPtr != NULL){
			if (index >= 0){
				m_selectedOptionId = m_constraintsPtr->GetOptionId(index);
			}
			else{
				m_selectedOptionId.clear();
			}
		}
	}

	return true;
}


ISelectionParam* CSelectionParam::GetSubselection(int index) const
{
	if (m_subselectionMap.contains(index)){
		return m_subselectionMap[index];
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectionParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag selectedOptionIndexTag("Index", "Selected option index", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag selectedOptionIdTag("OptionId", "Selected option identifier", iser::CArchiveTag::TT_LEAF);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	int selectionOptionIndex = m_selectedOptionIndex;
	QByteArray selectedOptionId;

	bool retVal = archive.BeginTag(selectedOptionIndexTag);
	retVal = retVal && archive.Process(selectionOptionIndex);
	retVal = retVal && archive.EndTag(selectedOptionIndexTag);

	if (m_constraintsPtr != NULL){
		int optionsCount = m_constraintsPtr->GetOptionsCount();

		if ((selectionOptionIndex >= 0) && (selectionOptionIndex < optionsCount)){
			selectedOptionId = m_constraintsPtr->GetOptionId(selectionOptionIndex);
		}
	}

	retVal = retVal && archive.BeginTag(selectedOptionIdTag);
	retVal = retVal && archive.Process(selectedOptionId);
	retVal = retVal && archive.EndTag(selectedOptionIdTag);

	if (retVal && !isStoring){
		if (		(m_constraintsPtr != NULL) &&
					((m_constraintsPtr->GetOptionsFlags() & iprm::IOptionsList::SCF_SUPPORT_UNIQUE_ID) != 0) &&
					!selectedOptionId.isEmpty()){
			if (!SetSelectedOptionById(selectedOptionId)){
				SetSelectedOptionIndex(-1);
			}
		}
		else{
			SetSelectedOptionIndex(selectionOptionIndex);
		}
	}

	return retVal;
}


// public methods of the embedded class ConstraintsObserver

CSelectionParam::ConstraintsObserver::ConstraintsObserver(CSelectionParam& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::TSingleModelObserverBase<iprm::IOptionsList>)

void CSelectionParam::ConstraintsObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(m_parent.m_constraintsPtr != NULL);
	Q_ASSERT(m_parent.m_constraintsPtr->GetOptionsFlags() & IOptionsList::SCF_SUPPORT_UNIQUE_ID);

	I_IF_DEBUG(
		if (m_parent.m_selectedOptionIndex >= 0){
			Q_ASSERT(!m_parent.m_selectedOptionId.isEmpty());
		}
		else{
			Q_ASSERT(m_parent.m_selectedOptionId.isEmpty());
		}
	)

	int selectedIndexById = m_parent.GetOptionIndexById(m_parent.m_selectedOptionId);

	if (m_parent.m_selectedOptionIndex != selectedIndexById){
		static const istd::IChangeable::ChangeSet changeSet(iprm::ISelectionParam::CF_SELECTION_CHANGED);
		istd::CChangeNotifier notifier(&m_parent, &changeSet);
		Q_UNUSED(notifier);

		m_parent.m_selectedOptionIndex = selectedIndexById;

		if (m_parent.m_selectedOptionIndex < 0){
			m_parent.m_selectedOptionId.clear();
		}
	}
}


} // namespace iprm


