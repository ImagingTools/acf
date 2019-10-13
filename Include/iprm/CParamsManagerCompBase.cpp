#include <iprm/CParamsManagerCompBase.h>


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CChangeGroup.h>
#include <imod/IModel.h>
#include <imod/TModelWrap.h>
#include <iprm/ISelectionParam.h>


namespace iprm
{


const istd::IChangeable::ChangeSet s_insertChangeSet(IParamsManager::CF_SET_INSERTED, IOptionsList::CF_OPTIONS_CHANGED, ISelectionParam::CF_SELECTION_CHANGED, QObject::tr("Insert parameter"));
const istd::IChangeable::ChangeSet s_removeChangeSet(IParamsManager::CF_SET_REMOVED, IOptionsList::CF_OPTIONS_CHANGED, ISelectionParam::CF_SELECTION_CHANGED, QObject::tr("Remove parameter"));
const istd::IChangeable::ChangeSet s_moveChangeSet(IParamsManager::CF_SET_ENABLE_CHANGED, IOptionsList::CF_OPTIONS_CHANGED, IParamsManager::CF_SET_REMOVED, ISelectionParam::CF_SELECTION_CHANGED, QObject::tr("Move parameter"));
const istd::IChangeable::ChangeSet s_renameChangeSet(IParamsManager::CF_SET_NAME_CHANGED, IOptionsList::CF_OPTION_RENAMED, QObject::tr("Rename"));
const istd::IChangeable::ChangeSet s_selectChangeSet(ISelectionParam::CF_SELECTION_CHANGED, QObject::tr("Change selection"));
const istd::IChangeable::ChangeSet s_setDescriptionChangeSet(IOptionsList::CF_OPTIONS_CHANGED, QObject::tr("Change description"));
const istd::IChangeable::ChangeSet s_setEnableChangeSet(IOptionsList::CF_OPTIONS_CHANGED, QObject::tr("Enable/Disable"));
const istd::IChangeable::ChangeSet s_flagsChangeSet(IOptionsList::CF_OPTIONS_CHANGED);


CParamsManagerCompBase::CParamsManagerCompBase()
:	m_selectedIndex(-1),
	m_updateBridge(this)
{
	m_selectedParams.parentPtr = this;
}


// reimplemented (iprm::IParamsManager)

int CParamsManagerCompBase::InsertParamsSet(int typeIndex, int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index >= 0) && (index < fixedParamsCount)){
		return -1;
	}

	IParamsSet* newParamsSetPtr = CreateParamsSetInstance(typeIndex);
	if (newParamsSetPtr == NULL){
		return -1;
	}

	istd::CChangeNotifier notifier(this, &s_insertChangeSet);
	Q_UNUSED(notifier);

	ParamSetPtr paramsSetPtr(new imod::TModelWrap<ParamSet>());

	paramsSetPtr->paramSetPtr.SetPtr(newParamsSetPtr);
	paramsSetPtr->name = CalculateNewDefaultName();
#if QT_VERSION >= 0x050000
	paramsSetPtr->uuid = QUuid::createUuid().toByteArray();
#else
	paramsSetPtr->uuid = QUuid::createUuid().toString().toLatin1();
#endif

	paramsSetPtr->parentPtr = this;

	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
	if (paramsModelPtr != NULL){
		paramsModelPtr->AttachObserver(&paramsSetPtr->updateBridge);
		paramsModelPtr->AttachObserver(&m_updateBridge);
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

	int removeIndex = index - fixedParamsCount;

	if ((removeIndex >= 0) && (removeIndex < m_paramSets.count())){
		istd::CChangeNotifier notifier(this, &s_removeChangeSet);
		Q_UNUSED(notifier);
	
		m_selectedIndex = index - 1;

		istd::TDelPtr<ParamSet> removedParamsSetPtr;
		removedParamsSetPtr.TakeOver(m_paramSets[removeIndex]);

		EnsureParamsSetModelDetached(removedParamsSetPtr->paramSetPtr.GetPtr());

		m_paramSets.removeAt(removeIndex);

		notifier.Reset();

		return true;
	}

	return false;
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

	istd::CChangeNotifier notifier(this, &s_moveChangeSet);
	Q_UNUSED(notifier);

	ParamSet& paramsSet1 = *m_paramSets[index1 - fixedParamsCount];
	ParamSet& paramsSet2 = *m_paramSets[index2 - fixedParamsCount];

	paramsSet1.paramSetPtr.Swap(paramsSet2.paramSetPtr);
	qSwap(paramsSet1.name, paramsSet2.name);
	qSwap(paramsSet1.isEnabled, paramsSet2.isEnabled);
	qSwap(paramsSet1.uuid, paramsSet2.uuid);

	return true;
}


IParamsSet* CParamsManagerCompBase::GetParamsSet(int index) const
{
	Q_ASSERT((index >= 0) && (index < CParamsManagerCompBase::GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return m_fixedParamSetsCompPtr[index];
	}

	if (
		m_elementIndexParamIdAttrPtr.IsValid() ||
		m_elementUuidParamIdAttrPtr.IsValid() ||
		m_elementNameParamIdAttrPtr.IsValid() ||
		m_elementDescriptionParamIdAttrPtr.IsValid()
	){
		return m_paramSets[index - fixedSetsCount].GetPtr();
	}
	else{
		return m_paramSets[index - fixedSetsCount]->paramSetPtr.GetPtr();
	}
}


IParamsSet* CParamsManagerCompBase::CreateParameterSet(int typeIndex, int index) const
{
	IParamsSet* sourceParamsSetPtr = NULL;;
	if ((index >= 0)){
		sourceParamsSetPtr = GetParamsSet(index);
	}

	// If the type index is not specified, try to get it from the source parameter set:
	if ((typeIndex < 0) && (sourceParamsSetPtr != NULL)){
		QByteArray sourceTypeId = sourceParamsSetPtr->GetFactoryId();
		const IOptionsList* typesListPtr = GetParamsTypeConstraints();
		if (!sourceTypeId.isEmpty() && (typesListPtr != NULL)){
			for (int i = 0; i < typesListPtr->GetOptionsCount(); ++i){
				if (typesListPtr->GetOptionId(i) == sourceTypeId){
					typeIndex = i;
					break;
				}
			}
		}
	}

	IParamsSet* newParamsSetPtr = CreateParamsSetInstance(typeIndex);
	if (newParamsSetPtr == NULL){
		return NULL;
	}

	ParamSet* retVal = new imod::TModelWrap<ParamSet>();
	retVal->paramSetPtr.SetPtr(newParamsSetPtr);
	retVal->isEnabled = true;
#if QT_VERSION >= 0x050000
	retVal->uuid = QUuid::createUuid().toByteArray();
#else
	retVal->uuid = QUuid::createUuid().toString().toLatin1();
#endif

	if (sourceParamsSetPtr != NULL){
		ParamSet* sourceParamsSetImplPtr = dynamic_cast<ParamSet*>(sourceParamsSetPtr);
		if (sourceParamsSetImplPtr != NULL){
			newParamsSetPtr->CopyFrom(*sourceParamsSetImplPtr->paramSetPtr.GetPtr());

			retVal->isEnabled = sourceParamsSetImplPtr->isEnabled;
			retVal->name = sourceParamsSetImplPtr->name;
			retVal->description = sourceParamsSetImplPtr->description;
			retVal->uuid = sourceParamsSetImplPtr->uuid;
		}
		else{
			newParamsSetPtr->CopyFrom(*sourceParamsSetPtr);
		}
	}

	return retVal;
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

	int userFlags = 0;
	const int fixedCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index >= fixedCount) && (index < CParamsManagerCompBase::GetParamsSetsCount())) {
		userFlags = m_paramSets[index - fixedCount]->userFlags;
	}
	else{
		if (m_fixedParamsSetFlagsMap.contains(index)) {
			userFlags = m_fixedParamsSetFlagsMap[index];
		}
	}

	if ((index >= 0) && (index < CParamsManagerCompBase::GetParamsSetsCount())){
		retVal |= MF_SUPPORT_EDIT;

		if ((index < fixedCount) && !*m_allowEditFixedAttrPtr){
			retVal &= ~MF_SUPPORT_EDIT;
		}
	}

	return retVal | userFlags;
}


bool CParamsManagerCompBase::SetIndexOperationFlags(int index, int flags)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	istd::CChangeGroup changeGroup(this);

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		if (!m_fixedParamsSetFlagsMap.contains(index) || m_fixedParamsSetFlagsMap[index] != flags){
			istd::CChangeNotifier notifier(this, &s_flagsChangeSet);
			Q_UNUSED(notifier);

			if (flags <= 0){
				m_fixedParamsSetFlagsMap.remove(index);
			}
			else{
				m_fixedParamsSetFlagsMap[index] = flags;
			}
		}
	}
	else{
		if (m_paramSets[index - fixedSetsCount]->userFlags != flags){
			istd::CChangeNotifier notifier(this, &s_flagsChangeSet);
			Q_UNUSED(notifier);

			m_paramSets[index - fixedSetsCount]->userFlags = flags;
		}
	}

	// Set selection to some avaialable index, if the currently selected option was set to inactive:
	if (flags & MF_INACTIVE){
		int selectedIndex = GetSelectedOptionIndex();
		if (selectedIndex == index){
			for (int i = 0; i < GetOptionsCount(); ++i) {
				if ((GetIndexOperationFlags(i) & MF_INACTIVE) == 0){
					SetSelectedOptionIndex(i);

					break;
				}
			}

			// Of no enabled parameter set could not be selected, reset selection:
			if (selectedIndex == GetSelectedOptionIndex()){
				SetSelectedOptionIndex(-1);
			}
		}
	}

	return true;
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
			return QObject::tr("%1_%2").arg(*m_defaultSetNameAttrPtr).arg(index - namesCount + 1);
		}
	}

	return m_paramSets[index - fixedSetsCount]->name;
}


bool CParamsManagerCompBase::SetParamsSetName(int index, const QString& name)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount]->name != name){
		istd::CChangeNotifier notifier(this, &s_renameChangeSet);
		Q_UNUSED(notifier);

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

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return;
	}

	if (m_paramSets[index - fixedSetsCount]->description.GetName() != description){
		istd::CChangeNotifier notifier(this, &s_setDescriptionChangeSet);
		Q_UNUSED(notifier);

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
			istd::CChangeNotifier notifier(this, &s_selectChangeSet);
			Q_UNUSED(notifier);

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
	return SCF_SUPPORT_UNIQUE_ID;
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
	QString defaultSetName = *m_defaultSetNameAttrPtr;
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

	int paramsCount = qMin(m_fixedParamSetsCompPtr.GetCount(), m_fixedSetNamesAttrPtr.GetCount());
	for (int i = 0; i < paramsCount; ++i){
		if (m_fixedSetNamesAttrPtr[i] == name){
			return i;
		}
	}

	return -1;
}


// reimplemented (icomp::CComponentBase)

void CParamsManagerCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_selectedIndex = -1;

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

	if (m_defaultSelectedIndexAttrPtr.IsValid() && (*m_defaultSelectedIndexAttrPtr < fixedSetsCount)){
		m_selectedIndex = *m_defaultSelectedIndexAttrPtr;
	}
}


// public methods of embedded class UuidParam

CParamsManagerCompBase::UuidParam::UuidParam(const ParamSet& parent)
	:m_parent(parent)
{
}


// reimplemented (iprm::INameParam)

const QString& CParamsManagerCompBase::UuidParam::GetName() const
{
	m_uuid = m_parent.uuid;

	return m_uuid;
}


void CParamsManagerCompBase::UuidParam::SetName(const QString& /*name*/)
{
}


bool CParamsManagerCompBase::UuidParam::IsNameFixed() const
{
	return true;
}


// reimplemented (iser::ISerializable)

bool CParamsManagerCompBase::UuidParam::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// public methods of embedded class ParamSet

CParamsManagerCompBase::ParamSet::ParamSet()
:	isEnabled(true),
	parentPtr(NULL),
	updateBridge(this),
	uuidParam(*this),
	userFlags(0)
{
}


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CParamsManagerCompBase::ParamSet::GetParamIds(bool editableOnly) const
{
	Q_ASSERT(paramSetPtr.IsValid());

	IParamsSet::Ids ids = paramSetPtr->GetParamIds(editableOnly);

	if (!editableOnly){
		if ((parentPtr != NULL) && parentPtr->m_elementIndexParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementIndexParamIdAttrPtr);
		}

		if ((parentPtr != NULL) && parentPtr->m_elementUuidParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementUuidParamIdAttrPtr);
		}

		if ((parentPtr != NULL) && parentPtr->m_elementNameParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementNameParamIdAttrPtr);
		}

		if ((parentPtr != NULL) && parentPtr->m_elementDescriptionParamIdAttrPtr.IsValid()){
			ids += *(parentPtr->m_elementDescriptionParamIdAttrPtr);
		}
	}

	return ids;
}


const iser::ISerializable* CParamsManagerCompBase::ParamSet::GetParameter(const QByteArray& id) const
{
	Q_ASSERT(paramSetPtr.IsValid());

	if ((parentPtr != NULL) && parentPtr->m_elementIndexParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementIndexParamIdAttrPtr))){
		return this;
	}

	if ((parentPtr != NULL) && parentPtr->m_elementUuidParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementUuidParamIdAttrPtr))){
		return &uuidParam;
	}

	if ((parentPtr != NULL) && parentPtr->m_elementNameParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementNameParamIdAttrPtr))){
		return this;
	}

	if ((parentPtr != NULL) && parentPtr->m_elementDescriptionParamIdAttrPtr.IsValid() && (id == *(parentPtr->m_elementDescriptionParamIdAttrPtr))){
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
	if (parentPtr != NULL){
		int retIndex = 0;

		for (		ParamSets::ConstIterator iter = parentPtr->m_paramSets.constBegin();
					iter != parentPtr->m_paramSets.constEnd();
					++iter, ++retIndex){
			if (iter->GetPtr() == this){
				return retIndex + parentPtr->m_fixedParamSetsCompPtr.GetCount();
			}
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

// reimplemented (iser::IObject)

QByteArray CParamsManagerCompBase::ParamSet::GetFactoryId() const
{
	Q_ASSERT(paramSetPtr.IsValid());

	return paramSetPtr->GetFactoryId();
}


// reimplemented (iser::ISerializable)

bool CParamsManagerCompBase::ParamSet::Serialize(iser::IArchive& archive)
{
	Q_ASSERT(paramSetPtr.IsValid());

	return paramSetPtr->Serialize(archive);
}


// reimplemented (istd::IChangeable)

bool CParamsManagerCompBase::ParamSet::CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode)
{
	Q_ASSERT(paramSetPtr.IsValid());

	return paramSetPtr->CopyFrom(object, mode);
}


// public methods of embedded class SelectedParams

CParamsManagerCompBase::SelectedParams::SelectedParams()
:	parentPtr(NULL)
{
}


// reimplemented (iprm::IParamsSet)

iprm::IParamsSet::Ids CParamsManagerCompBase::SelectedParams::GetParamIds(bool editableOnly) const
{
	if (!editableOnly && (parentPtr != NULL) && (parentPtr->m_selectedIndex >= 0)){
		const IParamsSet* selectedParamsSetPtr = parentPtr->GetParamsSet(parentPtr->m_selectedIndex);
		if (selectedParamsSetPtr != NULL) {
			return selectedParamsSetPtr->GetParamIds();
		}
	}

	return iprm::IParamsSet::Ids();
}


const iser::ISerializable* CParamsManagerCompBase::SelectedParams::GetParameter(const QByteArray& id) const
{
	if ((parentPtr != NULL) && (parentPtr->m_selectedIndex >= 0)){
		const IParamsSet* selectedParamsSetPtr = parentPtr->GetParamsSet(parentPtr->m_selectedIndex);
		if (selectedParamsSetPtr != NULL) {
			return selectedParamsSetPtr->GetParameter(id);
		}
	}

	return NULL;
}


iser::ISerializable* CParamsManagerCompBase::SelectedParams::GetEditableParameter(const QByteArray& /*id*/)
{
	return NULL;
}


// reimplemented (iser::IObject)

QByteArray CParamsManagerCompBase::SelectedParams::GetFactoryId() const
{
	if ((parentPtr != NULL) && (parentPtr->m_selectedIndex >= 0)){
		const IParamsSet* selectedParamsSetPtr = parentPtr->GetParamsSet(parentPtr->m_selectedIndex);
		if (selectedParamsSetPtr != NULL) {
			return selectedParamsSetPtr->GetFactoryId();
		}
	}

	return "";
}


// reimplemented (iser::ISerializable)

bool CParamsManagerCompBase::SelectedParams::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace iprm


