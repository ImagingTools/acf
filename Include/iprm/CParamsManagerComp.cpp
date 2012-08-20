#include "iprm/CParamsManagerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TChangeDelegator.h"

#include "imod/IModel.h"

#include "iprm/IParamsSet.h"


namespace iprm
{


CParamsManagerComp::CParamsManagerComp()
:	m_selectedIndex(-1)
{
}


bool CParamsManagerComp::SetSetsCount(int count)
{
	int actualSetsCount = GetParamsSetsCount();

	if (count < actualSetsCount){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
		if (count < fixedSetsCount){
			return false;
		}

		istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED);

		m_paramSets.resize(count - fixedSetsCount);
	}
	else if (count > actualSetsCount){
		if (!m_paramSetsFactPtr.IsValid()){
			return false;
		}

		int setsToAdd = count - GetParamsSetsCount();
		for (int i = 0; i < setsToAdd; ++i){
			IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
			if (newParamsSetPtr == NULL){
				return false;
			}

			istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED);

			m_paramSets.push_back(ParamSet());
			ParamSet& paramsSet = m_paramSets.back();

			paramsSet.paramSetPtr.SetPtr(newParamsSetPtr);
			paramsSet.name = *m_defaultSetNameCompPtr;

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(this);
			}
		}
	}

	return true;
}


// reimplemented (iprm::IParamsManager)

int CParamsManagerComp::GetIndexOperationFlags(int index) const
{
	int retVal = 0;

	if (m_paramSetsFactPtr.IsValid()){
		retVal |= MF_SUPPORT_INSERT | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME;

		if (!m_paramSets.isEmpty() && (index < CParamsManagerComp::GetParamsSetsCount())){
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


int CParamsManagerComp::GetParamsSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + m_paramSets.size();
}


IParamsManager::TypeIds CParamsManagerComp::GetSupportedTypeIds() const
{
	IParamsManager::TypeIds retVal;

	retVal.insert(*m_paramsSetTypeIdCompPtr);

	return retVal;
}


int CParamsManagerComp::InsertParamsSet(const QByteArray& typeId, int index)
{
	if (!typeId.isEmpty() && (typeId != *m_paramsSetTypeIdCompPtr)){
		return -1;
	}

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (		!m_paramSetsFactPtr.IsValid() ||
				((index >= 0) && (index < fixedParamsCount))){
		return -1;
	}

	IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
	if (newParamsSetPtr == NULL){
		return -1;
	}

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED | CF_OPTIONS_CHANGED);

	QString defaultSetName = m_defaultSetNameCompPtr.IsValid() ? *m_defaultSetNameCompPtr: "unnamed";

	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.name = defaultSetName;

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


bool CParamsManagerComp::RemoveParamsSet(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < CParamsManagerComp::GetParamsSetsCount());

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


bool CParamsManagerComp::SwapParamsSet(int index1, int index2)
{
	I_ASSERT(index1 >= 0);
	I_ASSERT(index1 < CParamsManagerComp::GetParamsSetsCount());
	I_ASSERT(index2 >= 0);
	I_ASSERT(index2 < CParamsManagerComp::GetParamsSetsCount());

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


IParamsSet* CParamsManagerComp::GetParamsSet(int index) const
{
	I_ASSERT((index >= 0) && (index < CParamsManagerComp::GetParamsSetsCount()));

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return const_cast<IParamsSet*>(m_paramSets[index - fixedCount].paramSetPtr.GetPtr());
}


QByteArray CParamsManagerComp::GetParamsSetTypeId(int index) const
{
	I_ASSERT((index >= 0) && (index < CParamsManagerComp::GetParamsSetsCount()));

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		int typeIdsCount = m_fixedSetTypeIdsCompPtr.GetCount();
		if (typeIdsCount > 0){
			int realIndex = qMin(index, typeIdsCount);

			return m_fixedSetTypeIdsCompPtr[realIndex];
		}
	}

	return *m_paramsSetTypeIdCompPtr;
}


QString CParamsManagerComp::GetParamsSetName(int index) const
{
	I_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

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


bool CParamsManagerComp::SetParamsSetName(int index, const QString& name)
{
	I_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

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

const ISelectionConstraints* CParamsManagerComp::GetSelectionConstraints() const
{
	return this;
}


int CParamsManagerComp::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CParamsManagerComp::SetSelectedOptionIndex(int index)
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


ISelectionParam* CParamsManagerComp::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CParamsManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag paramsSetListTag("ParamsSetList", "List of parameter set");
	static iser::CArchiveTag paramsSetTag("ParamsSet", "Single parameter set", true);
	static iser::CArchiveTag nameTag("Name", "Name of set");
	static iser::CArchiveTag valueTag("Value", "Value of set", true);

	bool retVal = true;

	int paramsCount = GetParamsSetsCount();
	retVal = retVal && archive.BeginMultiTag(paramsSetListTag, paramsSetTag, paramsCount);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
		if (!retVal || !SetSetsCount(paramsCount)){
			return false;
		}
	}

	for (int i = 0; i < paramsCount; ++i){
		IParamsSet* paramsSetPtr = GetParamsSet(i);

		if (paramsSetPtr == NULL){
			return false;
		}

		retVal = retVal && archive.BeginTag(paramsSetTag);

		retVal = retVal && archive.BeginTag(nameTag);
		if (isStoring){
			QString name = GetParamsSetName(i);

			retVal = retVal && archive.Process(name);
		}
		else{
			QString name;

			retVal = retVal && archive.Process(name);
			if (!retVal){
				return false;
			}

			SetParamsSetName(i, name);
		}
		retVal = retVal && archive.EndTag(nameTag);

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

int CParamsManagerComp::GetConstraintsFlags() const
{
	return SCF_NONE;
}


int CParamsManagerComp::GetOptionsCount() const
{
	return GetParamsSetsCount();
}


QString CParamsManagerComp::GetOptionName(int index) const
{
	return GetParamsSetName(index);
}

QString CParamsManagerComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CParamsManagerComp::GetOptionId(int index) const
{
	return GetParamsSetName(index).toLocal8Bit();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CParamsManagerComp::OnComponentCreated()
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


void CParamsManagerComp::OnComponentDestroyed()
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


} // namespace iprm


