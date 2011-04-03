#include "iprm/CVariableParamComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iprm
{


// reimplemented (iprm::IParamsSet)

IVariableParam::TypeIds CVariableParamComp::GetKnownTypeIds() const
{
	TypeIds retVal;

	int typesCount = istd::Min(m_factoryIdsAttrPtr.GetCount(), m_factoriesFact.GetCount());
	for (int i = 0; i < typesCount; ++i){
		retVal.insert(m_factoryIdsAttrPtr[i].ToString());
	}

	return retVal;
}


std::string CVariableParamComp::GetParameterTypeId() const
{
	return m_paramTypeId;
}


iser::ISerializable* CVariableParamComp::GetParameter() const
{
	return m_paramPtr.GetPtr();
}


bool CVariableParamComp::AssignTypeId(const std::string& typeId)
{
	if (typeId == m_paramTypeId){
		return true;
	}

	if (typeId.empty()){
		m_paramPtr.Reset();
		m_paramTypeId = typeId;

		return true;
	}

	int typesCount = istd::Min(m_factoryIdsAttrPtr.GetCount(), m_factoriesFact.GetCount());
	for (int i = 0; i < typesCount; ++i){
		if (m_factoryIdsAttrPtr[i].ToString() == typeId){
			iser::ISerializable* newParamPtr = m_factoriesFact.CreateInstance(i);
			if (newParamPtr != NULL){
				if (m_paramPtr.IsValid()){
					newParamPtr->CopyFrom(*m_paramPtr);
				}

				m_paramPtr.SetPtr(newParamPtr);
				m_paramTypeId = typeId;

				return true;
			}
		}
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CVariableParamComp::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool retVal = true;

	std::string paramTypeId = m_paramTypeId;
	static iser::CArchiveTag paramTypeIdTag("TypeId", "Paramter type ID used to create specified type object");
	retVal = retVal && archive.BeginTag(paramTypeIdTag);
	retVal = retVal && archive.Process(paramTypeId);
	retVal = retVal && archive.EndTag(paramTypeIdTag);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring() && !AssignTypeId(paramTypeId)){
		return false;
	}

	if (m_paramPtr.IsValid()){
		static iser::CArchiveTag parameterTag("Parameter", "Paramter object depending on type ID");
		retVal = retVal && archive.BeginTag(parameterTag);
		retVal = retVal && m_paramPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(parameterTag);
	}

	return retVal;
}


I_DWORD CVariableParamComp::GetMinimalVersion(int versionId) const
{
	I_DWORD retVal = (versionId == iser::IVersionInfo::FrameworkVersionId)? 1582: 0;
	if (m_paramPtr.IsValid()){
		retVal = istd::Max(retVal, m_paramPtr->GetMinimalVersion(versionId));
	}

	return retVal;
}


} // namespace iprm


