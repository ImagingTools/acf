#include "iprm/CParamsSet.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TDelPtr.h"
#include "istd/CIdManipBase.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


CParamsSet::CParamsSet(const IParamsSet* slaveSetPtr)
:	imod::CMultiModelBridgeBase(this),
	m_paramsTypeId("Default"),
	m_slaveSetPtr(slaveSetPtr)
{
}


bool CParamsSet::SetEditableParameter(const QByteArray& id, iser::ISerializable* parameterPtr, bool releaseFlag)
{
	if (!id.isEmpty()){
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


const QByteArray& CParamsSet::GetParametersTypeId() const
{
	return m_paramsTypeId;
}


void CParamsSet::SetParametersTypeId(const QByteArray& id)
{
	m_paramsTypeId = id;
}


// reimplemented (iser::IObject)

QByteArray CParamsSet::GetFactoryId() const
{
	return m_paramsTypeId;
}


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CParamsSet::GetParamIds(bool editableOnly) const
{
	Ids retVal;

	if (!editableOnly){
		if (m_slaveSetPtr != NULL){
			retVal = m_slaveSetPtr->GetParamIds(false);
		}
	}

	for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterPtr = m_params.GetAt(parameterIndex);
		Q_ASSERT(parameterPtr != NULL);

		retVal.insert(parameterPtr->parameterId);

		const IParamsSet* subSetPtr = dynamic_cast<const IParamsSet*>(parameterPtr->parameterPtr.GetPtr());
		if (subSetPtr != NULL){
			Ids subIds = subSetPtr->GetParamIds(editableOnly);

			for (Ids::ConstIterator index = subIds.constBegin(); index != subIds.constEnd(); index++){
				retVal.insert(parameterPtr->parameterId + "/" + *index);
			}
		}
	}

	return retVal;
}


const iser::ISerializable* CParamsSet::GetParameter(const QByteArray& id) const
{
	QByteArray baseId;
	QByteArray subId;
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

	if (m_slaveSetPtr != NULL){
		return m_slaveSetPtr->GetParameter(id);
	}

	return NULL;
}


iser::ISerializable* CParamsSet::GetEditableParameter(const QByteArray& id)
{
	QByteArray baseId;
	QByteArray subId;
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


// reimplemented (iser::ISerializable)

bool CParamsSet::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag paramsSetTag("ParamsSet", "List of parameters", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag parameterTag("Parameter", "Single parameter", iser::CArchiveTag::TT_GROUP, &paramsSetTag, true);
	static iser::CArchiveTag parameterIdTag("Id", "ID of parameter", iser::CArchiveTag::TT_LEAF, &parameterTag);
	static iser::CArchiveTag parameterValueTag("Value", "Value of parameter", iser::CArchiveTag::TT_GROUP, &parameterTag);

	if (archive.IsStoring()){
		int paramsCount = m_params.GetCount();

		retVal = retVal && archive.BeginMultiTag(paramsSetTag, parameterTag, paramsCount);

		for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
			ParameterInfo* parameterInfoPtr = m_params.GetAt(parameterIndex);
			Q_ASSERT(parameterInfoPtr != NULL);

			retVal = retVal && archive.BeginTag(parameterTag);

			retVal = retVal && archive.BeginTag(parameterIdTag);
			retVal = retVal && archive.Process(parameterInfoPtr->parameterId);
			retVal = retVal && archive.EndTag(parameterIdTag);

			Q_ASSERT(parameterInfoPtr->parameterPtr.IsValid());
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

			QByteArray id;
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
			else{
				return false;
			}

			retVal = retVal && archive.EndTag(parameterTag);
		}

		retVal = retVal && archive.EndTag(paramsSetTag);
	}

	return retVal;
}


quint32 CParamsSet::GetMinimalVersion(int versionId) const
{
	quint32 retVal = 0;
	for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterInfoPtr = m_params.GetAt(parameterIndex);
		Q_ASSERT(parameterInfoPtr != NULL);

		quint32 minimalVersion = parameterInfoPtr->parameterPtr->GetMinimalVersion(versionId);
		if (minimalVersion > retVal){
			retVal = minimalVersion;
		}
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CParamsSet::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const iprm::CParamsSet* inputParamsSetPtr = dynamic_cast<const iprm::CParamsSet*>(&object);
	if (inputParamsSetPtr == NULL){
		return false;
	}

	if (mode == CM_STRICT){
		// check if external references are compatible
		if (m_slaveSetPtr != inputParamsSetPtr->m_slaveSetPtr){
			return false;
		}
	}

	CParamsSet tempSet;

	for (int parameterIndex = 0; parameterIndex < inputParamsSetPtr->m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterInfoPtr = inputParamsSetPtr->m_params.GetAt(parameterIndex);
		Q_ASSERT(parameterInfoPtr != NULL);

		istd::TDelPtr<iser::ISerializable> parameterCopyPtr;
		
		parameterCopyPtr.SetCastedOrRemove(parameterInfoPtr->parameterPtr->CloneMe(mode));

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

const CParamsSet::ParameterInfo* CParamsSet::FindParameterInfo(const QByteArray& parameterId) const
{
	for (int parameterIndex = 0; parameterIndex < m_params.GetCount(); parameterIndex++){
		const ParameterInfo* parameterPtr = m_params.GetAt(parameterIndex);
		Q_ASSERT(parameterPtr != NULL);

		if (parameterPtr->parameterId == parameterId){
			return parameterPtr;
		}
	}

	return NULL;
}


} // namespace iprm


