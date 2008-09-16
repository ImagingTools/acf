#include "iprm/CParamsManagerComp.h"


#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"


namespace iprm
{


bool CParamsManagerComp::SetSetsCount(int count)
{
	int actualSetsCount = GetSetsCount();

	if (count < actualSetsCount){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
		if (count < fixedSetsCount){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		m_paramSets.SetCount(count - fixedSetsCount);
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

			m_paramSets.PushBack(newParamsSetPtr);
		}
	}

	m_names.resize(istd::Max(count - m_fixedSetNamesCompPtr.GetCount(), 0), *m_defaultSetNameCompPtr);

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

	if (m_paramSets.IsEmpty()){
		retVal |= MF_NO_DELETE;
	}

	return retVal;
}


int CParamsManagerComp::GetSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + m_paramSets.GetCount();
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

	istd::CChangeNotifier notifier(this);

	if (index >= 0){
		m_paramSets.InsertElementAt(index - fixedParamsCount, newParamsSetPtr);
	}
	else{
		m_paramSets.PushBack(newParamsSetPtr);
	}

	return true;
}


bool CParamsManagerComp::RemoveSet(int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (index < fixedParamsCount){
		return false;
	}

	m_paramSets.RemoveAt(index - fixedParamsCount);

	return false;
}


IParamsSet* CParamsManagerComp::GetParamsSet(int index) const
{
	I_ASSERT((index >= 0) && (index < GetSetsCount()));

	int fixedCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return m_paramSets.GetAt(index - fixedCount);
}


const istd::CString& CParamsManagerComp::GetSetName(int index) const
{
	I_ASSERT((index >= 0) && (index < GetSetsCount()));

	int fixedCount = m_fixedSetNamesCompPtr.GetCount();
	if (index < fixedCount){
		return m_fixedSetNamesCompPtr[index];
	}

	if (index < fixedCount + int(m_names.size())){
		return m_names[index - fixedCount];
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

	if (index >= fixedCount + int(m_names.size())){
		m_names.resize(index - fixedCount + 1, *m_defaultSetNameCompPtr);
	}

	m_names[index - fixedCount] = name;

	return true;
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
	if (isStoring){
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

	return retVal;
}


} // namespace iprm


