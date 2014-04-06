#include "iprm/CMultiParamsManagerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "imod/IModel.h"


namespace iprm
{


// reimplemented (iprm::IParamsManager)

const IOptionsList* CMultiParamsManagerComp::GetParamsTypeConstraints() const
{
	return &m_typeInfoList;
}


int CMultiParamsManagerComp::InsertParamsSet(int typeIndex, int index)
{
	if (typeIndex >= m_typeInfoList.typeInfos.size()){
		return -1;
	}

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index >= 0) && (index < fixedParamsCount)){
		return -1;
	}

	const TypeInfo& info = (typeIndex >= 0)? m_typeInfoList.typeInfos[typeIndex]: m_typeInfoList.typeInfos[0];

	IParamsSet* newParamsSetPtr = m_paramSetsFactoriesPtr.CreateInstance(info.factoryIndex);
	if (newParamsSetPtr == NULL){
		return -1;
	}

	Q_ASSERT(newParamsSetPtr->GetFactoryId() == info.id);

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED | CF_OPTIONS_CHANGED | CF_MODEL);

	QString defaultSetName;
	if (m_defaultSetNameAttrPtr.IsValid()){
		defaultSetName = info.name + " " + *m_defaultSetNameAttrPtr;
		if (defaultSetName.contains("%1")){			
			QString tmpName;
			for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
				tmpName = defaultSetName;
				tmpName.replace(QString("%1"), QString::number(suffixIndex));
				if (FindParamSetIndex(tmpName) < 0){
					defaultSetName = tmpName;
					break;
				}
			}
		}
	}
	else {
		for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
			defaultSetName = info.name + " " + QObject::tr("unnamed%1").arg(suffixIndex);
			if (FindParamSetIndex(defaultSetName) < 0){
				break;
			}
		}		
	}

	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.typeId = info.id;
	paramSet.name = defaultSetName;

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
	if (modelPtr != NULL){
		modelPtr->AttachObserver(this);
	}

	if (index >= 0){
		int insertIndex = index - fixedParamsCount;

		m_paramSets.insert(m_paramSets.begin() + insertIndex, paramSet);

		return index;
	}
	else{
		m_paramSets.push_back(paramSet);

		return int(m_paramSets.size()) - 1;
	}	
}


bool CMultiParamsManagerComp::RemoveParamsSet(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < CMultiParamsManagerComp::GetParamsSetsCount());

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (index < fixedParamsCount){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_CHANGED | CF_SELECTION_CHANGED | CF_MODEL);
	
	int removeIndex = index - fixedParamsCount;

	m_paramSets.removeAt(removeIndex);

	m_selectedIndex = index - 1;

	return true;
}


bool CMultiParamsManagerComp::SwapParamsSet(int index1, int index2)
{
	Q_ASSERT(index1 >= 0);
	Q_ASSERT(index1 < CMultiParamsManagerComp::GetParamsSetsCount());
	Q_ASSERT(index2 >= 0);
	Q_ASSERT(index2 < CMultiParamsManagerComp::GetParamsSetsCount());

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index1 < fixedParamsCount) || (index2 < fixedParamsCount)){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_CHANGED | CF_SELECTION_CHANGED | CF_MODEL);

	ParamSet& paramsSet1 = m_paramSets[index1 - fixedParamsCount];
	ParamSet& paramsSet2 = m_paramSets[index2 - fixedParamsCount];

	paramsSet1.paramSetPtr.Swap(paramsSet2.paramSetPtr);
	qSwap(paramsSet1.typeId, paramsSet2.typeId);
	qSwap(paramsSet1.name, paramsSet2.name);
	qSwap(paramsSet1.isEnabled, paramsSet2.isEnabled);

	Q_ASSERT(paramsSet1.paramSetPtr->GetFactoryId() == paramsSet1.typeId);
	Q_ASSERT(paramsSet2.paramSetPtr->GetFactoryId() == paramsSet2.typeId);

	return true;
}


IParamsSet* CMultiParamsManagerComp::GetParamsSet(int index) const
{
	Q_ASSERT((index >= 0) && (index < CMultiParamsManagerComp::GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return const_cast<IParamsSet*>(m_paramSets[index - fixedSetsCount].paramSetPtr.GetPtr());
}


QString CMultiParamsManagerComp::GetParamsSetName(int index) const
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

	return m_paramSets[index - fixedSetsCount].name;
}


bool CMultiParamsManagerComp::SetParamsSetName(int index, const QString& name)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if (index < fixedSetsCount){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount].name != name){
		istd::CChangeNotifier notifier(this, CF_SET_NAME_CHANGED | CF_OPTION_RENAMED | CF_MODEL);

		m_paramSets[index - fixedSetsCount].name = name;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CMultiParamsManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag paramsSetListTag("ParamsSetList", "List of parameter set");
	static iser::CArchiveTag paramsSetTag("ParamsSet", "Single parameter set", true);
	static iser::CArchiveTag typeIdTag("TypeId", "Type id of factory of parameter set");
	static iser::CArchiveTag nameTag("Name", "Name of set");	
	static iser::CArchiveTag enabledTag("Enabled", "Is parameter set enabled");
	static iser::CArchiveTag valueTag("Value", "Value of set", true);

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	// Delete all dynamically created parameter sets:
	if (!isStoring){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

		while (m_paramSets.size() > fixedSetsCount){
			ParamSet& parameterSetToRemove = m_paramSets.back();

			EnsureParamsSetModelDetached(parameterSetToRemove.paramSetPtr.GetPtr());

			m_paramSets.removeLast();
		}

		m_selectedIndex = -1;
	}

	int paramsCount = GetParamsSetsCount();
	retVal = retVal && archive.BeginMultiTag(paramsSetListTag, paramsSetTag, paramsCount);

	if (!isStoring && !retVal){
		return false;
	}

	for (int i = 0; i < paramsCount; ++i){
		retVal = retVal && archive.BeginTag(paramsSetTag);

		retVal = retVal && archive.BeginTag(typeIdTag);
		
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
				typeId = m_paramSets[i - fixedSetsCount].typeId;
			}

			retVal = retVal && archive.Process(typeId);
		}
		else{
			retVal = retVal && archive.Process(typeId);
			if (!retVal){
				return false;
			}			
		}

		retVal = retVal && archive.EndTag(typeIdTag);

		QString name;
		bool isEnabled = true;

		if (isStoring){
			name = GetParamsSetName(i);
			isEnabled = IsOptionEnabled(i);
		}

		retVal = retVal && archive.BeginTag(nameTag);
		retVal = retVal && archive.Process(name);
		retVal = retVal && archive.EndTag(nameTag);	

		quint32 version = 0;
		if (		!archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, version) ||
					(version > 3186)){
			retVal = retVal && archive.BeginTag(enabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(enabledTag);
		}

		if (!isStoring){
			if (!retVal){
				return false;
			}			

			if (!EnsureParamExist(i, typeId, name, isEnabled)){
				return false;
			}
		}

		IParamsSet* paramsSetPtr = GetParamsSet(i);
		if (paramsSetPtr == NULL){
			return false;
		}

		retVal = retVal && archive.BeginTag(valueTag);		
		retVal = retVal && paramsSetPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(valueTag);

		retVal = retVal && archive.EndTag(paramsSetTag);
	}

	retVal = retVal && archive.EndTag(paramsSetListTag);

	int selectedIndex = -1;
	if (*m_serializeSelectionAttrPtr){
		selectedIndex = m_selectedIndex;
	}

	static iser::CArchiveTag selectedIndexTag("Selected", "Selected index");
	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	if (*m_serializeSelectionAttrPtr && !archive.IsStoring()){
		m_selectedIndex = selectedIndex;
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

	if (m_paramSets[index - fixedSetsCount].isEnabled != isEnabled){
		istd::CChangeNotifier notifier(this, CF_MODEL);

		m_paramSets[index - fixedSetsCount].isEnabled = isEnabled;
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


// reimplemented (iprm::IOptionsList)

int CMultiParamsManagerComp::GetOptionsFlags() const
{
	return GetIndexOperationFlags(-1);
}


int CMultiParamsManagerComp::GetOptionsCount() const
{
	return GetParamsSetsCount();
}


QString CMultiParamsManagerComp::GetOptionName(int index) const
{
	return GetParamsSetName(index);
}


QString CMultiParamsManagerComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


bool CMultiParamsManagerComp::IsOptionEnabled(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (!*m_allowDisabledAttrPtr || (index < fixedSetsCount)){
		return true;
	}

	return m_paramSets[index - fixedSetsCount].isEnabled;
}


QByteArray CMultiParamsManagerComp::GetOptionId(int index) const
{
	return GetParamsSetName(index).toLocal8Bit();
}


// protected methods

bool CMultiParamsManagerComp::EnsureParamExist(int index, const QByteArray& typeId, const QString& name, bool isEnabled)
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
		ParamSet& paramSet = m_paramSets[paramSetsIndex];

		istd::CChangeNotifier notifier(NULL, CF_OPTIONS_CHANGED | CF_MODEL);

		if (typeId != paramSet.typeId){
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

			notifier.SetPtr(this);

			paramSet.typeId = typeId;

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(this);
			}
		}

		if (name != paramSet.name){
			notifier.SetPtr(this);

			paramSet.name = name;
		}

		if (isEnabled != paramSet.isEnabled){
			notifier.SetPtr(this);

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

		istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED | CF_MODEL);	

		ParamSet paramSet;

		paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
		paramSet.typeId = typeId;
		paramSet.name = name;
		paramSet.isEnabled = isEnabled;

		m_paramSets.push_back(paramSet);

		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
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


// reimplemented (icomp::CComponentBase)

void CMultiParamsManagerComp::OnComponentCreated()
{
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedSetsCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
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
		if (modelPtr != NULL && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	m_typeInfoList.typeInfos.clear();
	m_typeInfoList.typeIdToIndexMap.clear();

	BaseClass::OnComponentDestroyed();
}


// private methods

void CMultiParamsManagerComp::EnsureParamsSetModelDetached(iprm::IParamsSet* paramsSetPtr) const
{
	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr);
	if (modelPtr != NULL){
		modelPtr->DetachAllObservers();				
	}
}


int CMultiParamsManagerComp::FindParamSetIndex(const QString& name) const
{
	int paramsCount = m_paramSets.size();
	for (int i = 0; i < paramsCount; ++i){
		if (m_paramSets.at(i).name == name){
			return i;
		}
	}

	return -1;
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

// public methods of embedded class ParamSet

CMultiParamsManagerComp::ParamSet::ParamSet()
:	isEnabled(true)
{
}


} // namespace iprm


