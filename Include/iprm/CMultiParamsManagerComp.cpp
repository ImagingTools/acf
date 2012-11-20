#include "iprm/CMultiParamsManagerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"


namespace iprm
{


CMultiParamsManagerComp::CMultiParamsManagerComp()
:	m_selectedIndex(-1)
{
}


// reimplemented (iprm::IParamsManager)

int CMultiParamsManagerComp::GetIndexOperationFlags(int index) const
{
	int retVal = 0;

	if (m_paramSetsFactoriesPtr.IsValid()){
		retVal |= MF_SUPPORT_INSERT | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME;

		if (!m_paramSets.isEmpty() && (index < CMultiParamsManagerComp::GetParamsSetsCount())){
			retVal |= MF_SUPPORT_DELETE;
		}
	}
	else{
		retVal |= MF_COUNT_FIXED;
	}

	if (index >= 0){
		if (index < m_fixedParamSetsCompPtr.GetCount()){
			retVal &= ~(MF_SUPPORT_INSERT | MF_SUPPORT_DELETE | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME);
		}
	}

	return retVal;
}


int CMultiParamsManagerComp::GetParamsSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + m_paramSets.size();
}


const ISelectionConstraints* CMultiParamsManagerComp::GetParamsTypeConstraints() const
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

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED | CF_OPTIONS_CHANGED | CF_MODEL);

	QString defaultSetName = m_defaultSetNameAttrPtr.IsValid() ? *m_defaultSetNameAttrPtr: "unnamed";

	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.name = defaultSetName;
	paramSet.typeId = info.id;

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
	qSwap(paramsSet1.name, paramsSet2.name);
	qSwap(paramsSet1.typeId, paramsSet2.typeId);

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
			return QObject::tr("%1_%2").arg(*m_defaultSetNameAttrPtr).arg(index - namesCount + 1);
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


// reimplemented (iprm::ISelectionParam)

const ISelectionConstraints* CMultiParamsManagerComp::GetSelectionConstraints() const
{
	return this;
}


int CMultiParamsManagerComp::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CMultiParamsManagerComp::SetSelectedOptionIndex(int index)
{
	if (index < GetOptionsCount()){
		if (index != m_selectedIndex){
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED | CF_OPTIONS_CHANGED | CF_MODEL);

			m_selectedIndex = index;
		}

		return true;
	}

	return false;
}


ISelectionParam* CMultiParamsManagerComp::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CMultiParamsManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag paramsSetListTag("ParamsSetList", "List of parameter set");
	static iser::CArchiveTag paramsSetTag("ParamsSet", "Single parameter set", true);
	static iser::CArchiveTag typeIdTag("TypeId", "Type id of factory of parameter set");
	static iser::CArchiveTag nameTag("Name", "Name of set");	
	static iser::CArchiveTag valueTag("Value", "Value of set", true);

	bool retVal = true;

	int paramsCount = GetParamsSetsCount();
	retVal = retVal && archive.BeginMultiTag(paramsSetListTag, paramsSetTag, paramsCount);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

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
				const iprm::IParamsSet* fixedSetPtr = m_fixedParamSetsCompPtr[i - fixedSetsCount];
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

		retVal = retVal && archive.BeginTag(nameTag);
		
		QString name;

		if (isStoring){
			name = GetParamsSetName(i);

			retVal = retVal && archive.Process(name);
		}
		else{
			retVal = retVal && archive.Process(name);
			if (!retVal){
				return false;
			}			
		}
		retVal = retVal && archive.EndTag(nameTag);	

		if (!isStoring && !EnsureParamExist(typeId, i, name)){
			return false;
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

	if (!isStoring){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

		while (m_paramSets.size() > paramsCount - fixedSetsCount){
			m_paramSets.removeLast();
		}
	}

	retVal = retVal && archive.EndTag(paramsSetListTag);

	static iser::CArchiveTag selectedIndexTag("Selected", "Selected index");
	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(m_selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	return retVal;
}


// reimplemented (iprm::ISelectionConstraints)

int CMultiParamsManagerComp::GetConstraintsFlags() const
{
	return SCF_NONE;
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


bool CMultiParamsManagerComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


QByteArray CMultiParamsManagerComp::GetOptionId(int index) const
{
	return GetParamsSetName(index).toLocal8Bit();
}


// protected methods

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
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedSetsCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	m_typeInfoList.typeInfos.clear();
	m_typeInfoList.typeIdToIndexMap.clear();

	BaseClass::OnComponentDestroyed();
}


bool CMultiParamsManagerComp::EnsureParamExist(const QByteArray& typeId, int index, const QString& name)
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

		istd::CChangeNotifier notifier(NULL, CF_MODEL | CF_OPTIONS_CHANGED | CF_MODEL);	

		if (name != paramSet.name){
			notifier.SetPtr(this);

			paramSet.name = name;
		}

		if (typeId != paramSet.typeId){
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

			notifier.SetPtr(this);

			paramSet.typeId = typeId;

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(this);
			}
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

		istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED | CF_MODEL);	

		ParamSet paramSet;

		paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
		paramSet.name = name;
		paramSet.typeId = typeId;

		m_paramSets.push_back(paramSet);

		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}

	return true;
}


// public methods of embedded class TypeInfoList

// reimplemented (iprm::ISelectionConstraints)

int CMultiParamsManagerComp::TypeInfoList::GetConstraintsFlags() const
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


