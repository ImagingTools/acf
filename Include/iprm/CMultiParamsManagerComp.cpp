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


IParamsManager::TypeIds CMultiParamsManagerComp::GetSupportedTypeIds() const
{
	IParamsManager::TypeIds retVal;
	
	for (int index = 0; index < m_paramSetsFactoriesPtr.GetCount(); index++){
		retVal.insert(m_paramSetsFactoriesPtr[index]);
	}

	return retVal;
}


int CMultiParamsManagerComp::InsertParamsSet(const QByteArray& typeId, int index)
{
	if (!typeId.isEmpty()){
		return -1;
	}

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index >= 0) && (index < fixedParamsCount)){
		return -1;
	}

	int factoryIndex = -1;

	for (int i = 0; i < m_paramSetsFactoriesPtr.GetCount(); i++){
		
		if (m_paramSetsFactoriesPtr[i] == typeId){
			factoryIndex = i;
			break;
		}
	}

	if (factoryIndex < 0){
		return -1;
	}

	IParamsSet* newParamsSetPtr = m_paramSetsFactoriesPtr.CreateInstance(factoryIndex);
	if (newParamsSetPtr == NULL){
		return -1;
	}

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED | CF_OPTIONS_CHANGED);

	QString defaultSetName = m_defaultSetNameCompPtr.IsValid() ? *m_defaultSetNameCompPtr: "unnamed";

	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.name = defaultSetName;
	paramSet.typeId = typeId;

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

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_CHANGED | CF_SELECTION_CHANGED);
	
	int removeIndex = index - fixedParamsCount;

	imod::IModel* paramsSetModelPtr = dynamic_cast<imod::IModel*>(m_paramSets[removeIndex].paramSetPtr.GetPtr());
	if (paramsSetModelPtr != NULL){
		paramsSetModelPtr->DetachAllObservers();
	}

	m_paramSets.erase(m_paramSets.begin() + removeIndex);

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

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_CHANGED | CF_SELECTION_CHANGED);

	ParamSet& paramsSet1 = m_paramSets[index1 - fixedParamsCount];
	ParamSet& paramsSet2 = m_paramSets[index2 - fixedParamsCount];

	paramsSet1.paramSetPtr.Swap(paramsSet2.paramSetPtr);
	QString tempName = paramsSet1.name;
	paramsSet1.name = paramsSet2.name;
	paramsSet2.name = tempName;

	return true;
}


IParamsSet* CMultiParamsManagerComp::GetParamsSet(int index) const
{
	Q_ASSERT((index >= 0) && (index < CMultiParamsManagerComp::GetParamsSetsCount()));

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return const_cast<IParamsSet*>(m_paramSets[index - fixedCount].paramSetPtr.GetPtr());
}


QByteArray CMultiParamsManagerComp::GetParamsSetTypeId(int index) const
{
	Q_ASSERT((index >= 0) && (index < CMultiParamsManagerComp::GetParamsSetsCount()));

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		int typeIdsCount = m_fixedSetTypeIdsCompPtr.GetCount();
		if (typeIdsCount > 0){
			int realIndex = qMin(index, typeIdsCount);

			return m_fixedSetTypeIdsCompPtr[realIndex];
		}
	}
	
	return m_paramSets[index - fixedCount].typeId;
}


QString CMultiParamsManagerComp::GetParamsSetName(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		int namesCount = m_fixedSetNamesCompPtr.GetCount();

		if (index < namesCount){
			return m_fixedSetNamesCompPtr[index];
		}
		else{
			return QObject::tr("%1_%2").arg(*m_defaultSetNameCompPtr).arg(index - namesCount + 1);
		}
	}

	return m_paramSets[index - fixedCount].name;
}


bool CMultiParamsManagerComp::SetParamsSetName(int index, const QString& name)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedCount = m_fixedSetNamesCompPtr.GetCount();
	if (index < fixedCount){
		return false;
	}

	if (m_paramSets[index - fixedCount].name != name){
		istd::CChangeNotifier notifier(this, CF_SET_NAME_CHANGED | CF_OPTION_RENAMED);

		m_paramSets[index - fixedCount].name = name;
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
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED | CF_OPTIONS_CHANGED);

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
			typeId = GetParamsSetTypeId(i);

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

		if (!isStoring){
			if (DeserializeParamsSet(typeId, i, name) < 0){
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


QByteArray CMultiParamsManagerComp::GetOptionId(int index) const
{
	return GetParamsSetName(index).toLocal8Bit();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CMultiParamsManagerComp::OnComponentCreated()
{
	int fixedCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}

	BaseClass::OnComponentCreated();
}


void CMultiParamsManagerComp::OnComponentDestroyed()
{
	int fixedCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


int CMultiParamsManagerComp::DeserializeParamsSet(const QByteArray& typeId, int index, const QString& name)
{
	if (!typeId.isEmpty()){
		return -1;
	}

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index >= 0) && (index < fixedParamsCount)){
		return -1;
	}

	int factoryIndex = -1;

	for (int i = 0; i < m_paramSetsFactoriesPtr.GetCount(); i++){
		
		if (m_paramSetsFactoriesPtr[i] == typeId){
			factoryIndex = i;
			break;
		}
	}

	if (factoryIndex < 0){
		return -1;
	}

	IParamsSet* newParamsSetPtr = m_paramSetsFactoriesPtr.CreateInstance(factoryIndex);
	if (newParamsSetPtr == NULL){
		return -1;
	}

	istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED);	

	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.name = name;
	paramSet.typeId = typeId;

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


} // namespace iprm


