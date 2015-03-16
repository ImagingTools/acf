#include "iprm/CSelectableParamsSetComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "imod/IModel.h"


namespace iprm
{


CSelectableParamsSetComp::CSelectableParamsSetComp()
:	BaseClass2(this),
	m_selectedIndex(-1),
	m_currentParamsSetObserver(*this)
{
}


// reimplemented (iser::IObject)

QByteArray CSelectableParamsSetComp::GetFactoryId() const
{
	return *m_typeIdAttrPtr;
}


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CSelectableParamsSetComp::GetParamIds(bool editableOnly) const
{
	Ids retVal;

	Q_ASSERT(m_selectionIdAttrPtr.IsValid());
	retVal.insert(*m_selectionIdAttrPtr);

	if (m_paramsManagerCompPtr.IsValid() && (m_selectedIndex >= 0) && (m_selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount())){
		const IParamsSet* paramsSetPtr = m_paramsManagerCompPtr->GetParamsSet(m_selectedIndex);
		if (paramsSetPtr != NULL){
			retVal += paramsSetPtr->GetParamIds(editableOnly);
		}
	}

	return retVal;
}


const iser::ISerializable* CSelectableParamsSetComp::GetParameter(const QByteArray& id) const
{
	Q_ASSERT(m_selectionIdAttrPtr.IsValid());

	const QByteArray& selectionId = *m_selectionIdAttrPtr;
	if (id == selectionId){
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


iser::ISerializable* CSelectableParamsSetComp::GetEditableParameter(const QByteArray& id)
{
	Q_ASSERT(m_selectionIdAttrPtr.IsValid());

	const QByteArray& selectionId = *m_selectionIdAttrPtr;
	if (id == selectionId){
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

const IOptionsList* CSelectableParamsSetComp::GetSelectionConstraints() const
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
		Q_ASSERT(m_paramsManagerCompPtr.IsValid());

		if (m_paramsManagerCompPtr->SetSelectedOptionIndex(index)){
			ChangeSet changeSet(CF_SELECTION_CHANGED);
			istd::CChangeNotifier notifier(this, changeSet);

			m_selectedIndex = index;

			SetupCurrentParamsSetBridge();
		}
	}

	return true;
}


ISelectionParam* CSelectableParamsSetComp::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (imod::IObserver)

void CSelectableParamsSetComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.Contains(iprm::ISelectionParam::CF_SELECTION_CHANGED)){
		SetSelectedOptionIndex(m_paramsManagerCompPtr->GetSelectedOptionIndex());
	}

	BaseClass2::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (iser::ISerializable)

bool CSelectableParamsSetComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag selectedIndexTag("Selected", "Selected index", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag paramsManagerTag("Parameters", "All parameters", iser::CArchiveTag::TT_GROUP);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

	int selectedIndex = m_selectedIndex;

	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	if (!isStoring && selectedIndex != m_selectedIndex){
		retVal = retVal && SetSelectedOptionIndex(selectedIndex);
	}

	if (m_serializeParamsManagerAttrPtr.IsValid() && *m_serializeParamsManagerAttrPtr){
		if (m_paramsManagerCompPtr.IsValid()){
			retVal = retVal && archive.BeginTag(paramsManagerTag);
			retVal = retVal && m_paramsManagerCompPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(paramsManagerTag);
		}
	}

	return retVal;
}


// protected methods

void CSelectableParamsSetComp::SetupCurrentParamsSetBridge()
{
	Q_ASSERT(m_paramsManagerCompPtr.IsValid());

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


// reimplemented (iprm::IOptionsList)

int CSelectableParamsSetComp::GetOptionsFlags() const
{
	if (m_paramsManagerCompPtr.IsValid()){
		const iprm::IOptionsList* constraintsPtr = m_paramsManagerCompPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			return constraintsPtr->GetOptionsFlags();
		}
	}

	return SCF_NONE;
}


int CSelectableParamsSetComp::GetOptionsCount() const
{
	if (m_paramsManagerCompPtr.IsValid()){
		return m_paramsManagerCompPtr->GetParamsSetsCount();
	}

	return 0;
}


QString CSelectableParamsSetComp::GetOptionName(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		return m_paramsManagerCompPtr->GetParamsSetName(index);
	}

	static QString noname("<noname>");

	return noname;
}


QString CSelectableParamsSetComp::GetOptionDescription(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		const iprm::IOptionsList* constraintsPtr = m_paramsManagerCompPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			return constraintsPtr->GetOptionDescription(index);
		}
	}

	return QString();
}


QByteArray CSelectableParamsSetComp::GetOptionId(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		const iprm::IOptionsList* constraintsPtr = m_paramsManagerCompPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			return constraintsPtr->GetOptionId(index);
		}
	}

	return QByteArray();
}


bool CSelectableParamsSetComp::IsOptionEnabled(int index) const
{
	if (m_paramsManagerCompPtr.IsValid()){
		const iprm::IOptionsList* constraintsPtr = m_paramsManagerCompPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			return constraintsPtr->IsOptionEnabled(index);
		}
	}

	return true;
}


// reimplemented (icomp::CComponentBase)

void CSelectableParamsSetComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultIndexAttrPtr.IsValid()){
		SetSelectedOptionIndex(*m_defaultIndexAttrPtr);
	}

	if (m_paramsManagerCompPtr.IsValid()){
		imod::IModel* managerModePtr = dynamic_cast<imod::IModel*>(m_paramsManagerCompPtr.GetPtr());
		if (managerModePtr != NULL){
			managerModePtr->AttachObserver(this);
		}
	}
}


void CSelectableParamsSetComp::OnComponentDestroyed()
{
	if (m_paramsManagerCompPtr.IsValid()){
		imod::IModel* managerModePtr = dynamic_cast<imod::IModel*>(m_paramsManagerCompPtr.GetPtr());
		if (managerModePtr != NULL && managerModePtr->IsAttached(this)){
			managerModePtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}



// public methods of the embedded class CurrentParamsSetObserver

CSelectableParamsSetComp::CurrentParamsSetObserver::CurrentParamsSetObserver(CSelectableParamsSetComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CSelectableParamsSetComp::CurrentParamsSetObserver::BeforeUpdate(imod::IModel* I_IF_DEBUG(modelPtr))
{
	Q_ASSERT(IsModelAttached(modelPtr));

	m_parent.BeginChanges(GetDelegatedChanges());
}


void CSelectableParamsSetComp::CurrentParamsSetObserver::AfterUpdate(imod::IModel* I_IF_DEBUG(modelPtr), const ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsModelAttached(modelPtr));

	m_parent.EndChanges(GetDelegatedChanges());
}


} // namespace iprm


