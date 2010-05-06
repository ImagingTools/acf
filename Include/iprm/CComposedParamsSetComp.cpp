#include "iprm/CComposedParamsSetComp.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (iprm::IParamsSet)

const iser::ISerializable* CComposedParamsSetComp::GetParameter(const std::string& id) const
{
	std::string baseId;
	std::string subId;
	bool isSubelement = istd::CIdManipBase::SplitId(id, baseId, subId);

	const iprm::CParamsSet::ParameterInfo* parameterInfoPtr = FindParameterInfo(baseId);
	if (parameterInfoPtr != NULL){
		const iser::ISerializable* paramPtr = parameterInfoPtr->parameterPtr.GetPtr();
		if (isSubelement){
			const IParamsSet* subSetPtr = dynamic_cast<const IParamsSet*>(paramPtr);
			if (subSetPtr != NULL){
				return subSetPtr->GetParameter(subId);
			}
		}
		else{
			return paramPtr;
		}
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
	std::string baseId;
	std::string subId;
	bool isSubelement = istd::CIdManipBase::SplitId(id, baseId, subId);

	const iprm::CParamsSet::ParameterInfo* parameterInfoPtr = FindParameterInfo(baseId);
	if (parameterInfoPtr != NULL){
		iser::ISerializable* paramPtr = parameterInfoPtr->parameterPtr.GetPtr();
		if (isSubelement){
			IParamsSet* subSetPtr = dynamic_cast<IParamsSet*>(paramPtr);
			if (subSetPtr != NULL){
				return subSetPtr->GetEditableParameter(subId);
			}
		}
		else{
			return paramPtr;
		}
	}

	return NULL;
}


// reimplemented (istd::IHierarchical)

int CComposedParamsSetComp::GetHierarchicalFlags() const
{
	return HF_CHILDS_SUPPORTED;
}


int CComposedParamsSetComp::GetChildsCount() const
{
	return m_parametersCompPtr.GetCount();
}


istd::IPolymorphic* CComposedParamsSetComp::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_parametersCompPtr.GetCount());

	return m_parametersCompPtr[index];
}


istd::IPolymorphic* CComposedParamsSetComp::GetParent() const
{
	return NULL;
}


// protected methods

iprm::IParamsSet* CComposedParamsSetComp::GetSlaveParamsSet(int index) const
{
	if (!m_slaveParamsCompPtr.IsValid()){
		return NULL;
	}

	I_ASSERT(index >= 0);
	I_ASSERT(index < GetSlaveParamsSetCount());

	return m_slaveParamsCompPtr[index];
}


int CComposedParamsSetComp::GetSlaveParamsSetCount() const
{
	if (!m_slaveParamsCompPtr.IsValid()){
		return 0;
	}

	return m_slaveParamsCompPtr.GetCount();
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


