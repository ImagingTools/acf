#include "iprm/CComposedParamsSetComp.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (iprm::IParamsSet)

const iser::ISerializable* CComposedParamsSetComp::GetParameter(const std::string& id) const
{
	const ParamsMap& paramsMap = GetParamsMap();
	ParamsMap::const_iterator iter = paramsMap.find(id);
	if (iter != paramsMap.end()){
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
	ParamsMap& paramsMap = GetParamsMapRef();
	ParamsMap::iterator iter = paramsMap.find(id);
	if (iter != paramsMap.end()){
		return iter->second;
	}

	return NULL;
}


// reimplemented (icomp::IComponent)

void CComposedParamsSetComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int setsCount = istd::Min(m_parametersCompPtr.GetCount(), m_parametersIdAttrPtr.GetCount());

	for (int i = 0; i < setsCount; ++i){
		iser::ISerializable* paramPtr = m_parametersCompPtr[i];
		std::string id = m_parametersIdAttrPtr[i].ToString();

		SetEditableParameter(id, paramPtr);
	}
}


} // namespace iprm


