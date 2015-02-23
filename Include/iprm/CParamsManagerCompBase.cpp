#include "iprm/CParamsManagerCompBase.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "imod/IModel.h"
#include "imod/TModelWrap.h"
#include "iprm/ISelectionParam.h"


namespace iprm
{


CParamsManagerCompBase::CParamsManagerCompBase()
	:m_selectedIndex(-1)
{
}


// reimplemented (iprm::IParamsManager)

int CParamsManagerCompBase::InsertParamsSet(int typeIndex, int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index >= 0) && (index < fixedParamsCount)){
		return -1;
	}

	IParamsSet* newParamsSetPtr = CreateParamsSet(typeIndex);
	if (newParamsSetPtr == NULL){
		return -1;
	}

	ChangeSet changeSet(CF_SET_INSERTED, CF_OPTIONS_CHANGED);
	istd::CChangeNotifier notifier(this, changeSet);

	ParamSetPtr paramsSetPtr(new imod::TModelWrap<ParamSet>());

	paramsSetPtr->paramSetPtr.SetPtr(newParamsSetPtr);
	paramsSetPtr->name = CalculateNewDefaultName();
	paramsSetPtr->typeId = newParamsSetPtr->GetFactoryId();
	paramsSetPtr->parentPtr = this;

	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
	if (paramsModelPtr != NULL){
		paramsModelPtr->AttachObserver(paramsSetPtr.GetPtr());
		paramsModelPtr->AttachObserver(this);
	}

	m_paramSets.push_back(ParamSetPtr());
	if (index >= 0){
		int insertIndex = index - fixedParamsCount;

		for (int i = m_paramSets.size() - 1; i > insertIndex; --i){
			m_paramSets[i].TakeOver(m_paramSets[i - 1]);
		}
		m_paramSets[insertIndex].TakeOver(paramsSetPtr);

		return index;
	}
	else{
		m_paramSets.last().TakeOver(paramsSetPtr);

		return GetParamsSetsCount() - 1;
	}
}


bool CParamsManagerCompBase::RemoveParamsSet(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < CParamsManagerCompBase::GetParamsSetsCount());

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (index < fixedParamsCount){
		return false;
	}

	ChangeSet changeSet(CF_SET_REMOVED, CF_OPTIONS_CHANGED, CF_SELECTION_CHANGED);
	istd::CChangeNotifier notifier(this, changeSet);
	
	int removeIndex = index - fixedParamsCount;

	m_selectedIndex = index - 1;

	if (removeIndex >= 0 && removeIndex < m_paramSets.count()){
		istd::TDelPtr<ParamSet> removedParamsSetPtr;
		removedParamsSetPtr.TakeOver(m_paramSets[removeIndex]);

		EnsureParamsSetModelDetached(removedParamsSetPtr->paramSetPtr.GetPtr());

		m_paramSets.removeAt(removeIndex);

		notifier.Reset();
	}

	return true;
}


bool CParamsManagerCompBase::SwapParamsSet(int index1, int index2)
{
	Q_ASSERT(index1 >= 0);
	Q_ASSERT(index1 < CParamsManagerCompBase::GetParamsSetsCount());
	Q_ASSERT(index2 >= 0);
	Q_ASSERT(index2 < CParamsManagerCompBase::GetParamsSetsCount());

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index1 < fixedParamsCount) || (index2 < fixedParamsCount)){
		return false;
	}

	ChangeSet changeSet(CF_SET_ENABLE_CHANGED, CF_SET_REMOVED, CF_SELECTION_CHANGED);
	istd::CChangeNotifier notifier(this, changeSet);

	ParamSet& paramsSet1 = *m_paramSets[index1 - fixedParamsCount];
	ParamSet& paramsSet2 = *m_paramSets[index2 - fixedParamsCount];

	paramsSet1.paramSetPtr.Swap(paramsSet2.paramSetPtr);
	qSwap(paramsSet1.typeId, paramsSet2.typeId);
	qSwap(paramsSet1.name, paramsSet2.name);
	qSwap(paramsSet1.isEnabled, paramsSet2.isEnabled);

	Q_ASSERT(paramsSet1.paramSetPtr->GetFactoryId() == paramsSet1.typeId);
	Q_ASSERT(paramsSet2.paramSetPtr->GetFactoryId() == paramsSet2.typeId);

	return true;
}


IParamsSet* CParamsManagerCompBase::GetParamsSet(int index) const
{
	Q_ASSERT((index >= 0) && (index < CParamsManagerCompBase::GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return m_fixedParamSetsCompPtr[index];
	}

	if (m_elementIndexParamIdAttrPtr.IsValid() || m_elementNameParamIdAttrPtr.IsValid() || m_elementDescriptionParamIdAttrPtr.IsValid()){
		return const_cast<ParamSet*>(m_paramSets[index - fixedSetsCount].GetPtr());
	}
	else{
		return const_cast<IParamsSet*>(m_paramSets[index - fixedSetsCount]->paramSetPtr.GetPtr());
	}
}


int CParamsManagerCompBase::GetIndexOperationFlags(int index) const
{
	Q_ASSERT(index < GetParamsSetsCount());

	int retVal = 0;

	if (IsParameterCreationSupported()){
		if ((index < 0) || (index >= m_fixedParamSetsCompPtr.GetCount())){
			retVal |= MF_SUPPORT_INSERT | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME;
			
			if (*m_allowDisabledAttrPtr){
				retVal |= MF_DISABLE_ALLOWED;
			}

			if (*m_supportEnablingAttrPtr){
				retVal |= OOF_SUPPORT_ENABLING;
			}

			if (GetCreatedParamsSetsCount() > 0){
				retVal |= MF_SUPPORT_DELETE;
			}
		}
	}
	else{
		retVal |= MF_COUNT_FIXED;
	}

	if ((index >= 0) && (index < CParamsManagerCompBase::GetParamsSetsCount())){
		retVal |= MF_SUPPORT_EDIT;

		if (index < m_fixedParamSetsCompPtr.GetCount() && !*m_allowEditFixedAttrPtr){
			retVal &= ~MF_SUPPORT_EDIT;		
		}
	}

	return retVal;
}


int CParamsManagerCompBase::GetParamsSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + GetCreatedParamsSetsCount();
}


QString CParamsManagerCompBase::GetParamsSetName(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		int namesCount = m_fixedSetNamesAttrPtr.GetCount();

		if (index < namesCount){
			return m_fixedSetNamesAttrPtr[index];
		}
		else{
			return QObject::tr("%1_%2").arg(m_defaultSetNameAttrPtr.IsValid() ? *m_defaultSetNameAttrPtr : "unnamed").arg(index - namesCount + 1);
		}
	}

	return m_paramSets[index - fixedSetsCount]->name;
}


bool CParamsManagerCompBase::SetParamsSetName(int index, const QString& name)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if (index < fixedSetsCount){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount]->name != name){
		ChangeSet changeSet(CF_SET_NAME_CHANGED, CF_OPTION_RENAMED);
		istd::CChangeNotifier notifier(this, changeSet);

		m_paramSets[index - fixedSetsCount]->name = name;
	}

	return true;
}


QString CParamsManagerCompBase::GetParamsSetDescription(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		int descriptionsCount = m_fixedSetDescriptionsAttrPtr.GetCount();

		if (index < descriptionsCount){
			return m_fixedSetDescriptionsAttrPtr[index];
		}
		else{
			return QString();
		}
	}

	return m_paramSets[index - fixedSetsCount]->description.GetName();
}


void CParamsManagerCompBase::SetParamsSetDescription(int index, const QString& description)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if (index < fixedSetsCount){
		return;
	}

	if (m_paramSets[index - fixedSetsCount]->description.GetName() != description){
		ChangeSet changeSet(CF_OPTIONS_CHANGED);
		istd::CChangeNotifier notifier(this, changeSet);

		m_paramSets[index - fixedSetsCount]->description.SetName(description);
	}
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CParamsManagerCompBase::GetSelectionConstraints() const
{
	return this;
}


int CParamsManagerCompBase::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CParamsManagerCompBase::SetSelectedOptionIndex(int index)
{
	if (index < GetOptionsCount()){
		if (index != m_selectedIndex){
			ChangeSet changeSet(CF_OPTIONS_CHANGED, CF_SELECTION_CHANGED);
			istd::CChangeNotifier notifier(this, changeSet);

			m_selectedIndex = index;
		}

		return true;
	}

	return false;
}


ISelectionParam* CParamsManagerCompBase::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iprm::IOptionsList)

int CParamsManagerCompBase::GetOptionsFlags() const
{
	return GetIndexOperationFlags(-1);
}


int CParamsManagerCompBase::GetOptionsCount() const
{
	return GetParamsSetsCount();
}


QString CParamsManagerCompBase::GetOptionName(int index) const
{
	return GetParamsSetName(index);
}


QString CParamsManagerCompBase::GetOptionDescription(int index) const
{
	return GetParamsSetDescription(index);
}


QByteArray CParamsManagerCompBase::GetOptionId(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		int idsCount = m_fixedSetIdsAttrPtr.GetCount();

		if (index < idsCount){
			return m_fixedSetIdsAttrPtr[index];
		}
		else{
			return QByteArray();
		}
	}

	QByteArray uuid = m_paramSets[index - fixedSetsCount]->uuid;
	if (uuid.isEmpty()){
		uuid = GetParamsSetName(index).toLocal8Bit();
	}

	return uuid;
}


bool CParamsManagerCompBase::IsOptionEnabled(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (!*m_allowDisabledAttrPtr || (index < fixedSetsCount)){
		return true;
	}

	return m_paramSets[index - fixedSetsCount]->isEnabled;
}


// protected methods

void CParamsManagerCompBase::EnsureParamsSetModelDetached(iprm::IParamsSet* paramsSetPtr) const
{
	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr);
	if (modelPtr != NULL){
		modelPtr->DetachAllObservers();				
	}
}


QString CParamsManagerCompBase::CalculateNewDefaultName() const
{
	QString defaultSetName;
	if (m_defaultSetNameAttrPtr.IsValid()){
		defaultSetName = *m_defaultSetNameAttrPtr;
		if (defaultSetName.contains("%1")){			
			QString tmpName;
			for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
				tmpName = defaultSetName;
				tmpName.replace(QString("%1"), QString::number(suffixIndex));
				if (FindParamSetIndex(tmpName) < 0 && FindFixedParamSetIndex(tmpName) < 0){
					defaultSetName = tmpName;
					break;
				}
			}
		}
	}
	else {
		for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
			defaultSetName = QObject::tr("unnamed-%1").arg(suffixIndex);
			if (FindParamSetIndex(defaultSetName) < 0 && FindFixedParamSetIndex(defaultSetName) < 0){
				break;
			}
		}
	}

	return defaultSetName;
}


int CParamsManagerCompBase::FindParamSetIndex(const QString& name) const
{
	int paramsCount = m_paramSets.size();
	for (int i = 0; i < paramsCount; ++i){
		const ParamSetPtr& paramSetPtr = m_paramSets[i];
		if (paramSetPtr.IsValid() && (paramSetPtr->name == name)){
			return i;
		}
	}

	return -1;
}


int CParamsManagerCompBase::FindFixedParamSetIndex(const QString& name) const
{
	if (!m_fixedParamSetsCompPtr.IsValid()){
		return -1;
	}

	int paramsCount = qMin(m_fixedParamSetsCompPtr.GetCount(), m_fixedSetNamesAttrPtr.GetCount());;
	for (int i = 0; i < paramsCount; ++i){
		if (m_fixedSetNamesAttrPtr[i] == name){
			return i;
		}
	}

	return -1;
}


// public methods of embedded class ParamSet

CParamsManagerCompBase::ParamSet::ParamSet()
:	isEnabled(true)
{
	parentPtr = NULL;
}


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CParamsManagerCompBase::ParamSet::GetParamIds(bool editableOnly) const
{
	Q_ASSERT(paramSetPtr.IsValid());
	Q_ASSERT(parentPtr != NULL);

	IParamsSet::Ids ids = paramSetPtr->GetParamIds(editableOnly);

	if (!editableOnly){
		if (parentPtr->m_elementIndexParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementIndexParamIdAttrPtr);
		}

		if (parentPtr->m_elementNameParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementNameParamIdAttrPtr);
		}

		if (parentPtr->m_elementDescriptionParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementDescriptionParamIdAttrPtr);
		}
	}

	return ids;
}


const iser::ISerializable* CParamsManagerCompBase::ParamSet::GetParameter(const QByteArray& id) const
{
	Q_ASSERT(paramSetPtr.IsValid());
	Q_ASSERT(parentPtr != NULL);

	if (parentPtr->m_elementIndexParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementIndexParamIdAttrPtr))){
		return this;
	}

	if (parentPtr->m_elementNameParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementNameParamIdAttrPtr))){
		return this;
	}

	if (parentPtr->m_elementDescriptionParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementDescriptionParamIdAttrPtr))){
		return &description;
	}

	return paramSetPtr->GetParameter(id);
}


iser::ISerializable* CParamsManagerCompBase::ParamSet::GetEditableParameter(const QByteArray& id)
{
	Q_ASSERT(paramSetPtr.IsValid());

	return paramSetPtr->GetEditableParameter(id);
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CParamsManagerCompBase::ParamSet::GetSelectionConstraints() const
{
	return parentPtr;
}


int CParamsManagerCompBase::ParamSet::GetSelectedOptionIndex() const
{
	Q_ASSERT(parentPtr != NULL);

	int retIndex = 0;
	for (		ParamSets::ConstIterator iter = parentPtr->m_paramSets.constBegin();
				iter != parentPtr->m_paramSets.constEnd();
				++iter, ++retIndex){
		if (iter->GetPtr() == this){
			return retIndex + parentPtr->m_fixedParamSetsCompPtr.GetCount();
		}
	}

	return -1;
}


bool CParamsManagerCompBase::ParamSet::SetSelectedOptionIndex(int /*index*/)
{
	return false;
}


ISelectionParam* CParamsManagerCompBase::ParamSet::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::INameParam)

const QString& CParamsManagerCompBase::ParamSet::GetName() const
{
	return name;
}


void CParamsManagerCompBase::ParamSet::SetName(const QString& /*name*/)
{	// it is read only interface implementation
}


bool CParamsManagerCompBase::ParamSet::IsNameFixed() const
{
	return true;
}


// reimplemented (iser::ISerializable)

bool CParamsManagerCompBase::ParamSet::Serialize(iser::IArchive& archive)
{
	Q_ASSERT(paramSetPtr.IsValid());

	return paramSetPtr->Serialize(archive);
}




} // namespace iprm


