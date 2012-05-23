#include "iprm/CVariableParamComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iprm
{


// reimplemented (iprm::IParamsSet)

IVariableParam::TypeIds CVariableParamComp::GetKnownTypeIds() const
{
	TypeIds retVal;

	int typesCount = qMin(m_factoryIdsAttrPtr.GetCount(), m_factoriesFact.GetCount());
	for (int i = 0; i < typesCount; ++i){
		retVal.insert(m_factoryIdsAttrPtr[i].toLocal8Bit());
	}

	return retVal;
}


QByteArray CVariableParamComp::GetParameterTypeId() const
{
	return m_paramTypeId;
}


iser::ISerializable* CVariableParamComp::GetParameter() const
{
	return m_paramPtr.GetPtr();
}


bool CVariableParamComp::AssignTypeId(const QByteArray& typeId)
{
	if (typeId == m_paramTypeId){
		return true;
	}

	if (typeId.isEmpty()){
		m_paramPtr.Reset();
		m_paramTypeId = typeId;

		return true;
	}

	int typesCount = qMin(m_factoryIdsAttrPtr.GetCount(), m_factoriesFact.GetCount());
	for (int i = 0; i < typesCount; ++i){
		if (m_factoryIdsAttrPtr[i].toLocal8Bit() == typeId){
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
	static iser::CArchiveTag paramTypeIdTag("TypeId", "Paramter type ID used to create specified type object");

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

	QByteArray paramTypeId = m_paramTypeId;
	retVal = retVal && archive.BeginTag(paramTypeIdTag);
	retVal = retVal && archive.Process(paramTypeId);
	retVal = retVal && archive.EndTag(paramTypeIdTag);

	if (!retVal){
		return false;
	}

	if (!isStoring && !AssignTypeId(paramTypeId)){
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


quint32 CVariableParamComp::GetMinimalVersion(int versionId) const
{
	quint32 retVal = (versionId == iser::IVersionInfo::AcfVersionId)? 1582: 0;
	if (m_paramPtr.IsValid()){
		retVal = qMax(retVal, m_paramPtr->GetMinimalVersion(versionId));
	}

	return retVal;
}


} // namespace iprm


