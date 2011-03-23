#include "iprm/CParamsSet.h"


#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


CParamsSet::CParamsSet(const IParamsSet* slaveSetPtr)
:	m_slaveSetPtr(slaveSetPtr)
{
}


bool CParamsSet::SetEditableParameter(const std::string& id, iser::ISerializable* parameterPtr, bool releaseFlag)
{
	if (!id.empty()){
		const ParameterInfo* parameterInfoPtr = FindParameterInfo(id);
		if (parameterInfoPtr == NULL){
			m_params.PushBack(new ParameterInfo(id, parameterPtr, releaseFlag));

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(parameterPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(this);
			}

			return true;
		}
	}

	return false;
}


const CParamsSet::ParameterInfos& CParamsSet::GetParameterInfos() const
{
	return m_params;
}


// reimplemented (iprm::IParamsSet)

const iser::ISerializable* CParamsSet::GetParameter(const std::string& id) const
{
	const ParameterInfo* parameterInfoPtr = FindParameterInfo(id);
	if (parameterInfoPtr != NULL){
		return parameterInfoPtr->parameterPtr.GetPtr();
	}

	if (m_slaveSetPtr != NULL){
		return m_slaveSetPtr->GetParameter(id);
	}

	return NULL;
}


iser::ISerializable* CParamsSet::GetEditableParameter(const std::string& id)
{
	const ParameterInfo* parameterInfoPtr = FindParameterInfo(id);
	if (parameterInfoPtr != NULL){
		return parameterInfoPtr->parameterPtr.GetPtr();
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
		int paramsCount = m_params.GetCount();

		retVal = retVal && archive.BeginMultiTag(paramsSetTag, parameterTag, paramsCount);

		for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
			ParameterInfo* parameterInfoPtr = m_params.GetAt(parameterIndex);
			I_ASSERT(parameterInfoPtr != NULL);

			retVal = retVal && archive.BeginTag(parameterTag);

			retVal = retVal && archive.BeginTag(parameterIdTag);
			retVal = retVal && archive.Process(parameterInfoPtr->parameterId);
			retVal = retVal && archive.EndTag(parameterIdTag);

			I_ASSERT(parameterInfoPtr->parameterPtr.IsValid());
			retVal = retVal && archive.BeginTag(parameterValueTag);
			retVal = retVal && parameterInfoPtr->parameterPtr->Serialize(archive);
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

			const ParameterInfo* parameterInfoPtr = FindParameterInfo(id);
			if (parameterInfoPtr != NULL){
				retVal = retVal && archive.BeginTag(parameterValueTag);
				retVal = retVal && parameterInfoPtr->parameterPtr->Serialize(archive);
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
	for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterInfoPtr = m_params.GetAt(parameterIndex);
		I_ASSERT(parameterInfoPtr != NULL);

		I_DWORD minimalVersion = parameterInfoPtr->parameterPtr->GetMinimalVersion(versionId);
		if (minimalVersion > retVal){
			retVal = minimalVersion;
		}
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CParamsSet::CopyFrom(const IChangeable& object)
{
	const iprm::CParamsSet* inputParamsSetPtr = dynamic_cast<const iprm::CParamsSet*>(&object);
	if (inputParamsSetPtr == NULL){
		return false;
	}

	CParamsSet tempSet;

	for (int parameterIndex = 0; parameterIndex < inputParamsSetPtr->m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterInfoPtr = inputParamsSetPtr->m_params.GetAt(parameterIndex);
		I_ASSERT(parameterInfoPtr != NULL);

		istd::TDelPtr<iser::ISerializable> parameterCopyPtr;
		
		parameterCopyPtr.SetCastedOrRemove(parameterInfoPtr->parameterPtr->CloneMe());

		if (parameterCopyPtr.IsValid()){
			tempSet.SetEditableParameter(parameterInfoPtr->parameterId, parameterCopyPtr.PopPtr(), true);
		}
		else{
			return false;
		}
	}

	// copy params into the target parameter set:
	istd::CChangeNotifier notifier(this);

	m_params.Reset();

	while (!tempSet.m_params.IsEmpty()){
		m_params.PushBack(tempSet.m_params.PopAt(0));
	}

	return true;
}


// protected methods

const CParamsSet::ParameterInfo* CParamsSet::FindParameterInfo(const std::string& parameterId) const
{
	for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterPtr = m_params.GetAt(parameterIndex);
		I_ASSERT(parameterPtr != NULL);

		if (parameterPtr->parameterId == parameterId){
			return parameterPtr;
		}
	}

	return NULL;
}


} // namespace iprm


