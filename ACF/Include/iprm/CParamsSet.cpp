#include "iprm/CParamsSet.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


CParamsSet::CParamsSet(const IParamsSet* parentSetPtr)
:	m_parentSetPtr(parentSetPtr)
{
}


bool CParamsSet::SetEditableParameter(const std::string& id, iser::ISerializable* parameterPtr)
{
	if (!id.empty()){
		ParamsMap::const_iterator findIter = m_paramsMap.find(id);
		if (findIter == m_paramsMap.end()){
			m_paramsMap[id] = parameterPtr;

			return true;
		}
	}

	return false;
}


// reimplemented (iprm::IParamsSet)

const iser::ISerializable* CParamsSet::GetParameter(const std::string& id) const
{
	ParamsMap::const_iterator iter = m_paramsMap.find(id);
	if (iter != m_paramsMap.end()){
		return iter->second;
	}

	if (m_parentSetPtr != NULL){
		return m_parentSetPtr->GetParameter(id);
	}

	return NULL;
}


iser::ISerializable* CParamsSet::GetEditableParameter(const std::string& id)
{
	ParamsMap::iterator iter = m_paramsMap.find(id);
	if (iter != m_paramsMap.end()){
		return iter->second;
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CParamsSet::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag paramsSetTag("ParamsSet", "List of parameters");
	static iser::CArchiveTag parameterTag("Parameter", "Single parameter", true);
	static iser::CArchiveTag parameterIdTag("Id", "ID of parameter");
	static iser::CArchiveTag parameterValueTag("Value", "Value of parameter");

	if (archive.IsStoring()){
		int paramsCount = int(m_paramsMap.size());

		retVal = retVal && archive.BeginMultiTag(paramsSetTag, parameterTag, paramsCount);

		for (		ParamsMap::iterator iter = m_paramsMap.begin();
					iter != m_paramsMap.end();
					++iter){
			I_ASSERT(iter->second != NULL);

			retVal = retVal && archive.BeginTag(parameterTag);

			std::string id = iter->first;
			retVal = retVal && archive.BeginTag(parameterIdTag);
			retVal = retVal && archive.Process(id);
			retVal = retVal && archive.EndTag(parameterIdTag);

			retVal = retVal && archive.BeginTag(parameterValueTag);
			retVal = retVal && iter->second->Serialize(archive);
			retVal = retVal && archive.EndTag(parameterValueTag);

			retVal = retVal && archive.EndTag(parameterTag);
		}

		retVal = retVal && archive.EndTag(paramsSetTag);
	}
	else{
		int paramsCount = 0;

		retVal = retVal && archive.BeginMultiTag(paramsSetTag, parameterTag, paramsCount);

		if (!retVal){
			return false;
		}

		istd::CChangeNotifier notifier(this);

		for (int i = 0; i < paramsCount; ++i){
			retVal = retVal && archive.BeginTag(parameterTag);

			std::string id;
			retVal = retVal && archive.BeginTag(parameterIdTag);
			retVal = retVal && archive.Process(id);
			retVal = retVal && archive.EndTag(parameterIdTag);

			if (!retVal){
				return false;
			}

			ParamsMap::iterator foundParameter = m_paramsMap.find(id);
			if (foundParameter != m_paramsMap.end()){
				retVal = retVal && archive.BeginTag(parameterValueTag);
				retVal = retVal && foundParameter->second->Serialize(archive);
				retVal = retVal && archive.EndTag(parameterValueTag);
			}

			retVal = retVal && archive.EndTag(parameterTag);
		}

		retVal = retVal && archive.EndTag(paramsSetTag);
	}

	return retVal;
}


I_DWORD CParamsSet::GetMinimalVersion(int versionId) const
{
	I_DWORD retVal = 0;
	for (		ParamsMap::const_iterator iter = m_paramsMap.begin();
				iter != m_paramsMap.end();
				++iter){
		I_ASSERT(iter->second != NULL);

		I_DWORD minimalVersion = iter->second->GetMinimalVersion(versionId);
		if (minimalVersion > retVal){
			retVal = minimalVersion;
		}
	}

	return retVal;
}


} // namespace iprm


