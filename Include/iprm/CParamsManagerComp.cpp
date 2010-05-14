#include "iprm/CParamsManagerComp.h"


#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"

#include "iprm/IParamsSet.h"


namespace iprm
{


CParamsManagerComp::CParamsManagerComp()
	:m_selectedIndex(-1)
{
}


bool CParamsManagerComp::SetSetsCount(int count)
{
	int actualSetsCount = GetSetsCount();

	if (count < actualSetsCount){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
		if (count < fixedSetsCount){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		m_paramSets.resize(count - fixedSetsCount);
	}
	else if (count > actualSetsCount){
		if (!m_paramSetsFactPtr.IsValid()){
			return false;
		}

		int setsToAdd = count - GetSetsCount();
		for (int i = 0; i < setsToAdd; ++i){
			IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
			if (newParamsSetPtr == NULL){
				return false;
			}

			istd::CChangeNotifier notifier(this);

			ParamSet paramsSet;
			paramsSet.paramSetPtr.SetPtr(newParamsSetPtr);
			paramsSet.name = *m_defaultSetNameCompPtr;

			m_paramSets.push_back(paramsSet);
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


int CParamsManagerComp::GetSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + m_paramSets.size();
}


bool CParamsManagerComp::InsertSet(int index)
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

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED);

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


bool CParamsManagerComp::RemoveSet(int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (index < fixedParamsCount){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_SELECTION_CHANGED);
	
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
	I_ASSERT((index >= 0) && (index < GetSetsCount()));
	if ((index < 0) || (index >= GetSetsCount())){
		return NULL;
	}

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return const_cast<IParamsSet*>(m_paramSets[index - fixedCount].paramSetPtr.GetPtr());
}


const istd::CString& CParamsManagerComp::GetSetName(int index) const
{
	I_ASSERT((index >= 0) && (index < GetSetsCount()));

	int fixedCount = m_fixedSetNamesCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedSetNamesCompPtr[index];
	}

	if (index < fixedCount + int(m_paramSets.size())){
		return m_paramSets[index - fixedCount].name;
	}

	return *m_defaultSetNameCompPtr;
}


bool CParamsManagerComp::SetSetName(int index, const istd::CString& name)
{
	I_ASSERT((index >= 0) && (index < GetSetsCount()));

	int fixedCount = m_fixedSetNamesCompPtr.GetCount();
	if (index < fixedCount){
		return false;
	}

	if (m_paramSets[index - fixedCount].name != name){
		istd::CChangeNotifier notifier(this, CF_SET_NAME_CHANGED);

		m_paramSets[index - fixedCount].name = name;
	}

	return true;
}


// reimplemented (iprm::ISelectionParam)

int CParamsManagerComp::GetOptionsCount() const
{
	return GetSetsCount();
}


int CParamsManagerComp::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CParamsManagerComp::SetSelectedOptionIndex(int index)
{
	if (index < GetOptionsCount()){
		if (index != m_selectedIndex){
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED);

			m_selectedIndex = index;
		}

		return true;
	}

	return false;
}


const istd::CString& CParamsManagerComp::GetOptionName(int index) const
{
	return GetSetName(index);
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

	int paramsCount = GetSetsCount();
	retVal = retVal && archive.BeginMultiTag(paramsSetListTag, paramsSetTag, paramsCount);

	bool isStoring = archive.IsStoring();
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
			istd::CString name = GetSetName(i);

			retVal = retVal && archive.Process(name);
		}
		else{
			istd::CString name;

			retVal = retVal && archive.Process(name);
			if (!retVal){
				return false;
			}

			SetSetName(i, name);
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


} // namespace iprm


