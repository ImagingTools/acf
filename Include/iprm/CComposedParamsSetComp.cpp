#include "iprm/CComposedParamsSetComp.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CComposedParamsSetComp::GetParamIds(bool editableOnly) const
{
	Ids retVal = BaseClass2::GetParamIds(editableOnly);

	if (!editableOnly){
		int slavesCount = m_slaveParamsCompPtr.GetCount();
		for (int i = 0; i < slavesCount; ++i){
			const IParamsSet* slavePtr = m_slaveParamsCompPtr[i];
			if (slavePtr != NULL){
				retVal += slavePtr->GetParamIds(false);
			}
		}
	}

	return retVal;
}


const iser::ISerializable* CComposedParamsSetComp::GetParameter(const QByteArray& id) const
{
	const iser::ISerializable* paramPtr = BaseClass2::GetParameter(id);
	if (paramPtr != NULL){
		return paramPtr;
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


// reimplemented (istd::IChangeable)

bool CComposedParamsSetComp::CopyFrom(const IChangeable& /*object*/)
{
	return false;
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


// reimplemented (icomp::CComponentBase)

void CComposedParamsSetComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	BaseClass2::SetParametersTypeId(*m_typeIdAttrPtr);

	int setsCount = qMin(m_parametersCompPtr.GetCount(), m_parametersIdAttrPtr.GetCount());

	for (int i = 0; i < setsCount; ++i){
		iser::ISerializable* paramPtr = m_parametersCompPtr[i];
		const QByteArray& id = m_parametersIdAttrPtr[i];

		BaseClass2::SetEditableParameter(id, paramPtr);
	}
}


} // namespace iprm


