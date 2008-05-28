#include "iprm/CComposedParamsSetComp.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (IParamsSet)

const iser::ISerializable* CComposedParamsSetComp::GetParameter(const std::string& id) const
{
	I_ASSERT(m_slaveParamsCompPtr.IsValid());	// isObligatory was set to true

	ParamsMap::const_iterator iter = m_paramsMap.find(id);
	if (iter != m_paramsMap.end()){
		return iter->second;
	}

	int slavesCount = m_slaveParamsCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IParamsSet* slavePtr = m_slaveParamsCompPtr[i];
		if (slavePtr != NULL){
			const iser::ISerializable* paramPtr = slavePtr->GetParameter(id);
			if (paramPtr != NULL){
				return paramPtr;
			}
		}
	}

	return NULL;
}


iser::ISerializable* CComposedParamsSetComp::GetEditableParameter(const std::string& id)
{
	ParamsMap::iterator iter = m_paramsMap.find(id);
	if (iter != m_paramsMap.end()){
		return iter->second;
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CComposedParamsSetComp::Serialize(iser::IArchive& archive)
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


I_DWORD CComposedParamsSetComp::GetMinimalVersion(int versionId) const
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


// reimplemented (icomp::IComponent)

void CComposedParamsSetComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int setsCount = istd::Min(m_parametersCompPtr.GetCount(), m_parametersIdAttrPtr.GetCount());

	for (int i = 0; i < setsCount; ++i){
		iser::ISerializable* paramPtr = m_parametersCompPtr[i];
		std::string id = m_parametersIdAttrPtr[i].ToString();
		if (!id.empty() && (paramPtr != NULL)){
			m_paramsMap[id] = paramPtr;
		}
	}
}





} // namespace iprm


