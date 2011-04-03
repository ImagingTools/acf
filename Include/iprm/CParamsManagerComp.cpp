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

		istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_COUNT_CHANGED);

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

			istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_COUNT_CHANGED);

			ParamSet paramsSet;
			paramsSet.paramSetPtr.SetPtr(newParamsSetPtr);
			paramsSet.name = *m_defaultSetNameCompPtr;

			m_paramSets.push_back(paramsSet);

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(this);
			}
		}
	}

	return true;
}


// reimplemented (iprm::IParamsManager)

int CParamsManagerComp::GetManagerFlags() const
{
	int retVal = 0;

	if (!m_paramSetsFactPtr.IsValid()){
		retVal |= MF_COUNT_FIXED;
		retVal |= MF_NO_DELETE;
		retVal |= MF_NO_INSERT;
	}

	if (m_paramSets.empty()){
		retVal |= MF_NO_DELETE;
	}

	return retVal;
}


int CParamsManagerComp::GetParamsSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + m_paramSets.size();
}


bool CParamsManagerComp::InsertParamsSet(int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (		!m_paramSetsFactPtr.IsValid() ||
				((index >= 0) && (index < fixedParamsCount))){
		return false;
	}

	IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
	if (newParamsSetPtr == NULL){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED | CF_OPTIONS_COUNT_CHANGED);

	istd::CString defaultSetName = m_defaultSetNameCompPtr.IsValid() ? *m_defaultSetNameCompPtr: "unnamed";

	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.name = defaultSetName;

	if (index >= 0){
		int insertIndex = index - fixedParamsCount;

		m_paramSets.insert(m_paramSets.begin() + insertIndex, paramSet);
	}
	else{
		m_paramSets.push_back(paramSet);
	}

	return true;
}


bool CParamsManagerComp::RemoveParamsSet(int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (index < fixedParamsCount){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_COUNT_CHANGED | CF_SELECTION_CHANGED);
	
	int removeIndex = index - fixedParamsCount;

	imod::IModel* paramsSetModelPtr = dynamic_cast<imod::IModel*>(m_paramSets[removeIndex].paramSetPtr.GetPtr());
	if (paramsSetModelPtr != NULL){
		paramsSetModelPtr->DetachAllObservers();
	}

	m_paramSets.erase(m_paramSets.begin() + removeIndex);

	m_selectedIndex = index - 1;

	return true;
}


IParamsSet* CParamsManagerComp::GetParamsSet(int index) const
{
	I_ASSERT((index >= 0) && (index < GetParamsSetsCount()));
	if ((index < 0) || (index >= GetParamsSetsCount())){
		return NULL;
	}

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return const_cast<IParamsSet*>(m_paramSets[index - fixedCount].paramSetPtr.GetPtr());
}


const istd::CString& CParamsManagerComp::GetParamsSetName(int index) const
{
	I_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedCount = m_fixedSetNamesCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedSetNamesCompPtr[index];
	}

	if (index < fixedCount + int(m_paramSets.size())){
		return m_paramSets[index - fixedCount].name;
	}

	return *m_defaultSetNameCompPtr;
}


bool CParamsManagerComp::SetParamsSetName(int index, const istd::CString& name)
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

const ISelectionConstraints* CParamsManagerComp::GetConstraints() const
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
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED | CF_OPTIONS_COUNT_CHANGED);

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

	istd::CChangeNotifier notifier(isStoring ? NULL : this);

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
			istd::CString name = GetParamsSetName(i);

			retVal = retVal && archive.Process(name);
		}
		else{
			istd::CString name;

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


// protected methods

// reimplemented (iprm::ISelectionConstraints)

int CParamsManagerComp::GetOptionsCount() const
{
	return GetParamsSetsCount();
}


const istd::CString& CParamsManagerComp::GetOptionName(int index) const
{
	return GetParamsSetName(index);
}


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


