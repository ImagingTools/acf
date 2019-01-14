#include <iprm/CMultiParamsManagerComp.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CChangeGroup.h>
#include <istd/CChangeNotifier.h>
#include <imod/TModelWrap.h>


namespace iprm
{


// static attributes
static const istd::IChangeable::ChangeSet s_optionTypeChangeSet(IOptionsList::CF_OPTIONS_CHANGED, QObject::tr("Change parameter type"));
static const istd::IChangeable::ChangeSet s_renameChangeSet(IParamsManager::CF_SET_NAME_CHANGED, QObject::tr("Change parameter name"));
static const istd::IChangeable::ChangeSet s_enableChangeSet(IParamsManager::CF_SET_ENABLE_CHANGED, QObject::tr("Enable/disable option"));
static const istd::IChangeable::ChangeSet s_insertChangeSet(IParamsManager::CF_SET_INSERTED, QObject::tr("Insert parameter"));
static const iser::CArchiveTag s_paramsSetListTag("ParamsSetList", "List of parameter set", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_paramsSetTag("ParamsSet", "Single parameter set", iser::CArchiveTag::TT_GROUP, &s_paramsSetListTag, true);
static const iser::CArchiveTag s_typeIdTag("TypeId", "Type id of factory of parameter set", iser::CArchiveTag::TT_LEAF, &s_paramsSetTag);
static const iser::CArchiveTag s_nameTag("Name", "Name of set", iser::CArchiveTag::TT_LEAF, &s_paramsSetTag);
static const iser::CArchiveTag s_uuidTag("Uuid", "Uuid of parameter set", iser::CArchiveTag::TT_LEAF, &s_paramsSetTag);
static const iser::CArchiveTag s_enabledTag("Enabled", "Is parameter set enabled", iser::CArchiveTag::TT_LEAF, &s_paramsSetTag);
static const iser::CArchiveTag s_oldValueTag("Value", "Value of set", iser::CArchiveTag::TT_GROUP, &s_paramsSetTag, true);
static const iser::CArchiveTag s_valueTag("Value", "Value of set", iser::CArchiveTag::TT_WEAK, &s_paramsSetTag, true);
static const iser::CArchiveTag s_selectedIndexTag("Selected", "Selected index", iser::CArchiveTag::TT_LEAF);


		
// reimplemented (iprm::IParamsManager)

const IOptionsList* CMultiParamsManagerComp::GetParamsTypeConstraints() const
{
	return &m_typeInfoList;
}


// reimplemented (iser::ISerializable)

bool CMultiParamsManagerComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	// Delete all dynamically created parameter sets:
	if (!isStoring){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

		while (m_paramSets.size() > fixedSetsCount){
			ParamSetPtr& parameterSetToRemove = m_paramSets.back();

			EnsureParamsSetModelDetached(parameterSetToRemove->paramSetPtr.GetPtr());

			m_paramSets.removeLast();
		}

		m_selectedIndex = -1;
	}

	int paramsCount = GetParamsSetsCount();
	retVal = retVal && archive.BeginMultiTag(s_paramsSetListTag, s_paramsSetTag, paramsCount);

	if (!isStoring && !retVal){
		return false;
	}

	for (int i = 0; i < paramsCount; ++i){
		retVal = retVal && archive.BeginTag(s_paramsSetTag);

		retVal = retVal && archive.BeginTag(s_typeIdTag);
		
		QByteArray typeId;

		if (isStoring){
			int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
			if (i < fixedSetsCount){
				const iprm::IParamsSet* fixedSetPtr = m_fixedParamSetsCompPtr[i];
				if (fixedSetPtr != NULL){
					typeId = fixedSetPtr->GetFactoryId();
				}
			}
			else{
				typeId = m_paramSets[i - fixedSetsCount]->GetFactoryId();
			}

			retVal = retVal && archive.Process(typeId);
		}
		else{
			retVal = retVal && archive.Process(typeId);
			if (!retVal){
				return false;
			}		
		}

		retVal = retVal && archive.EndTag(s_typeIdTag);

		QString name;
		QByteArray uuid;
		bool isEnabled = true;

		if (isStoring){
			name = GetParamsSetName(i);
			uuid = GetOptionId(i);
			isEnabled = IsOptionEnabled(i);
		}

		retVal = retVal && archive.BeginTag(s_nameTag);
		retVal = retVal && archive.Process(name);
		retVal = retVal && archive.EndTag(s_nameTag);

		quint32 version = 0;
		bool hasVersion = archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, version);

		if (!hasVersion || (version > 4532)){
			retVal = retVal && archive.BeginTag(s_uuidTag);
			retVal = retVal && archive.Process(uuid);
			retVal = retVal && archive.EndTag(s_uuidTag);
		}

		if (!hasVersion || (version > 3186)){
			retVal = retVal && archive.BeginTag(s_enabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(s_enabledTag);
		}

		if (!isStoring){
			if (!retVal){
				return false;
			}

			if (!EnsureParamExist(i, typeId, name, uuid, isEnabled)){
				return false;
			}
		}

		IParamsSet* paramsSetPtr = GetParamsSet(i);
		if (paramsSetPtr == NULL){
			return false;
		}

		if (!hasVersion || (version > 4745)){
			retVal = retVal && archive.BeginTag(s_valueTag);
			retVal = retVal && paramsSetPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(s_valueTag);
		}
		else{
			retVal = retVal && archive.BeginTag(s_oldValueTag);
			retVal = retVal && paramsSetPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(s_oldValueTag);
		}

		retVal = retVal && archive.EndTag(s_paramsSetTag);
	}

	retVal = retVal && archive.EndTag(s_paramsSetListTag);

	int selectedIndex = -1;
	if (*m_serializeSelectionAttrPtr){
		selectedIndex = m_selectedIndex;
	}

	retVal = retVal && archive.BeginTag(s_selectedIndexTag);
	retVal = retVal && archive.Process(selectedIndex);
	retVal = retVal && archive.EndTag(s_selectedIndexTag);

	if (!isStoring){
		if (*m_serializeSelectionAttrPtr){
			m_selectedIndex = selectedIndex;
		}
		else{
			if (m_defaultSelectedIndexAttrPtr.IsValid()){
				m_selectedIndex = *m_defaultSelectedIndexAttrPtr;
			}
		}
	}

	return retVal;
}


// reimplemented (iprm::IOptionsManager)

int CMultiParamsManagerComp::GetOptionOperationFlags(int index) const
{
	return GetIndexOperationFlags(index);
}


bool CMultiParamsManagerComp::SetOptionEnabled(int index, bool isEnabled)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if ((index < fixedSetsCount) || !*m_allowDisabledAttrPtr || !*m_supportEnablingAttrPtr){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount]->isEnabled != isEnabled){
		istd::CChangeNotifier notifier(this, &s_enableChangeSet);
		Q_UNUSED(notifier);

		m_paramSets[index - fixedSetsCount]->isEnabled = isEnabled;
	}

	return true;
}


bool CMultiParamsManagerComp::RemoveOption(int index)
{
	return RemoveParamsSet(index);
}


bool CMultiParamsManagerComp::InsertOption(
			const QString& /*optionName*/,
			const QByteArray& /*optionId*/,
			const QString& /*optionDescription*/,
			int /*index*/)
{
	return false;
}


bool CMultiParamsManagerComp::SwapOptions(int index1, int index2)
{
	return SwapParamsSet(index1, index2);
}


bool CMultiParamsManagerComp::SetOptionName(int optionIndex, const QString& optionName)
{
	return SetParamsSetName(optionIndex, optionName);
}


bool CMultiParamsManagerComp::SetOptionDescription(int /*optionIndex*/, const QString& /*optionDescription*/)
{
	return false;
}


// protected methods

bool CMultiParamsManagerComp::EnsureParamExist(int index, const QByteArray& typeId, const QString& name, const QByteArray& uuid, bool isEnabled)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	int paramSetsIndex = index - fixedParamsCount;
	if (paramSetsIndex < 0){
		return true;
	}

	if (typeId.isEmpty()){
		return false;
	}

	if (paramSetsIndex < m_paramSets.size()){
		ParamSet& paramSet = *m_paramSets[paramSetsIndex];

		istd::CChangeGroup changeGroup(this);
		Q_UNUSED(changeGroup);

		if (typeId != paramSet.GetFactoryId()){
			QMap<QByteArray, int>::ConstIterator typeListIter = m_typeInfoList.typeIdToIndexMap.constFind(typeId);
			if (typeListIter == m_typeInfoList.typeIdToIndexMap.constEnd()){
				return false;
			}

			int typeListIndex = typeListIter.value();
			Q_ASSERT(typeListIndex >= 0);
			Q_ASSERT(typeListIndex < m_typeInfoList.typeInfos.size());

			const TypeInfo& typeInfo = m_typeInfoList.typeInfos[typeListIndex];
			Q_ASSERT(typeInfo.id == typeId);
			Q_ASSERT(typeInfo.factoryIndex >= 0);
			Q_ASSERT(typeInfo.factoryIndex < m_paramSetsFactoriesPtr.GetCount());

			IParamsSet* newParamsSetPtr = m_paramSetsFactoriesPtr.CreateInstance(typeInfo.factoryIndex);
			if (newParamsSetPtr == NULL){
				return false;
			}

			Q_ASSERT(newParamsSetPtr->GetFactoryId() == typeInfo.id);

			istd::CChangeNotifier notifier(this, &s_optionTypeChangeSet);
			Q_UNUSED(notifier);

			paramSet.paramSetPtr.SetPtr(newParamsSetPtr);

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(&m_updateBridge);
			}
		}

		if ((name != paramSet.name) || (uuid != paramSet.uuid) || (isEnabled != paramSet.isEnabled)){
			istd::CChangeNotifier notifier(this);
			Q_UNUSED(notifier);

			paramSet.name = name;
			paramSet.uuid = uuid;
			paramSet.isEnabled = isEnabled;
		}
	}
	else{
		QMap<QByteArray, int>::ConstIterator typeListIter = m_typeInfoList.typeIdToIndexMap.constFind(typeId);
		if (typeListIter == m_typeInfoList.typeIdToIndexMap.constEnd()){
			return false;
		}

		int typeListIndex = typeListIter.value();
		Q_ASSERT(typeListIndex >= 0);
		Q_ASSERT(typeListIndex < m_typeInfoList.typeInfos.size());

		const TypeInfo& typeInfo = m_typeInfoList.typeInfos[typeListIndex];
		Q_ASSERT(typeInfo.factoryIndex >= 0);
		Q_ASSERT(typeInfo.factoryIndex < m_paramSetsFactoriesPtr.GetCount());

		IParamsSet* newParamsSetPtr = m_paramSetsFactoriesPtr.CreateInstance(typeInfo.factoryIndex);
		if (newParamsSetPtr == NULL){
			return false;
		}
		
		Q_ASSERT(newParamsSetPtr->GetFactoryId() == typeInfo.id);

		istd::CChangeNotifier notifier(this, &s_insertChangeSet);	
		Q_UNUSED(notifier);

		ParamSetPtr paramsSetPtr(new imod::TModelWrap<ParamSet>());

		paramsSetPtr->paramSetPtr.SetPtr(newParamsSetPtr);
		paramsSetPtr->name = name.isEmpty() ? CalculateNewDefaultName() : name;
		paramsSetPtr->uuid = uuid;
		paramsSetPtr->isEnabled = isEnabled;
		paramsSetPtr->parentPtr = this;

		m_paramSets.push_back(ParamSetPtr());
		m_paramSets.back().TakeOver(paramsSetPtr);

		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
		if (paramsModelPtr != NULL){
			paramsModelPtr->AttachObserver(&m_paramSets.back()->updateBridge);
			paramsModelPtr->AttachObserver(&m_updateBridge);
		}

	}

	return true;
}


// reimplemented (CParamsManagerCompBase)

bool CMultiParamsManagerComp::IsParameterCreationSupported() const
{
	return (m_paramSetsFactoriesPtr.GetCount() > 0);
}


int CMultiParamsManagerComp::GetCreatedParamsSetsCount() const
{
	return m_paramSets.count();
}


iprm::IParamsSet* CMultiParamsManagerComp::CreateParamsSetInstance(int typeIndex) const
{
	if (typeIndex >= m_typeInfoList.typeInfos.size()){
		return NULL;
	}

	const TypeInfo& info = (typeIndex >= 0)? m_typeInfoList.typeInfos[typeIndex]: m_typeInfoList.typeInfos[0];

	IParamsSet* newParamsSetPtr = m_paramSetsFactoriesPtr.CreateInstance(info.factoryIndex);
	if (newParamsSetPtr == NULL){
		return NULL;
	}

	Q_ASSERT(newParamsSetPtr->GetFactoryId() == info.id);

	return newParamsSetPtr;
}


// reimplemented (icomp::CComponentBase)

void CMultiParamsManagerComp::OnComponentCreated()
{
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedSetsCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL){
			modelPtr->AttachObserver(&m_updateBridge);
		}
	}

	// Obtaining factory ids
	for (int factoryIndex = 0; factoryIndex < m_paramSetsFactoriesPtr.GetCount(); factoryIndex++){		
		istd::TDelPtr<IParamsSet> paramsSetPtr(m_paramSetsFactoriesPtr.CreateInstance(factoryIndex));
		
		if (paramsSetPtr.IsValid()){
			QByteArray factoryId = paramsSetPtr->GetFactoryId();
				
			if (!factoryId.isEmpty()){
				TypeInfo typeInfo;

				typeInfo.factoryIndex = factoryIndex;
				typeInfo.id = factoryId;
				typeInfo.name = factoryId;

				if (factoryIndex < m_factoryNameNameAttrPtr.GetCount()){
					typeInfo.name = m_factoryNameNameAttrPtr[factoryIndex];
				}
				if (factoryIndex < m_factoryDescriptionAttrPtr.GetCount()){
					typeInfo.description = m_factoryDescriptionAttrPtr[factoryIndex];
				}

				m_typeInfoList.typeInfos.push_back(typeInfo);
				m_typeInfoList.typeIdToIndexMap[factoryId] = m_typeInfoList.typeInfos.size() - 1;
			}
		}
	}

	BaseClass::OnComponentCreated();
}


void CMultiParamsManagerComp::OnComponentDestroyed()
{
	int setsCount = GetParamsSetsCount();

	for (int setIndex = 0; setIndex < setsCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(GetParamsSet(setIndex));
		if (modelPtr != NULL && modelPtr->IsAttached(&m_updateBridge)){
			modelPtr->DetachObserver(&m_updateBridge);
		}
	}

	m_typeInfoList.typeInfos.clear();
	m_typeInfoList.typeIdToIndexMap.clear();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class TypeInfoList

// reimplemented (iprm::IOptionsList)

int CMultiParamsManagerComp::TypeInfoList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CMultiParamsManagerComp::TypeInfoList::GetOptionsCount() const
{
	return typeInfos.size();
}


QString CMultiParamsManagerComp::TypeInfoList::GetOptionName(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < typeInfos.size());

	const TypeInfo& info = typeInfos[index];

	return info.name;
}


QString CMultiParamsManagerComp::TypeInfoList::GetOptionDescription(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < typeInfos.size());

	const TypeInfo& info = typeInfos[index];

	return info.description;
}


QByteArray CMultiParamsManagerComp::TypeInfoList::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < typeInfos.size());

	const TypeInfo& info = typeInfos[index];

	return info.id;
}


bool CMultiParamsManagerComp::TypeInfoList::IsOptionEnabled(int /*index*/) const
{
	return true;
}


} // namespace iprm


