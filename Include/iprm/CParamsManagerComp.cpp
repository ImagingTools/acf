#include "iprm/CParamsManagerComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TChangeDelegator.h"
#include "imod/IModel.h"
#include "iprm/IParamsSet.h"


namespace iprm
{


bool CParamsManagerComp::SetSetsCount(int count)
{
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (count < fixedSetsCount){
		return false;
	}

	int actualSetsCount = GetParamsSetsCount();
	if (count != actualSetsCount){
		if (!m_paramSetsFactPtr.IsValid()){
			return false;
		}

		static ChangeSet changeSet(CF_OPTIONS_CHANGED);
		istd::CChangeNotifier notifier(this, changeSet);

		while (m_paramSets.size() < (count - fixedSetsCount)){
			m_paramSets.append(ParamSetPtr());
		}

		while (m_paramSets.size() > (count - fixedSetsCount)){
			ParamSetPtr& paramsPtr = m_paramSets.back();

			EnsureParamsSetModelDetached(paramsPtr->paramSetPtr.GetPtr());

			m_paramSets.pop_back();
		}

		if (count > actualSetsCount){
			for (int i = actualSetsCount; i < count; ++i){
				IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
				if (newParamsSetPtr == NULL){
					return false;
				}

				static ChangeSet changeSet(CF_OPTIONS_CHANGED);
				istd::CChangeNotifier notifier(this, changeSet);

				ParamSetPtr paramsSetPtr(new imod::TModelWrap<ParamSet>());

				paramsSetPtr->paramSetPtr.SetPtr(newParamsSetPtr);
				paramsSetPtr->name = CalculateNewDefaultName();
				paramsSetPtr->parentPtr = this;

				imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
				if (paramsModelPtr != NULL){
					paramsModelPtr->AttachObserver(paramsSetPtr.GetPtr());
					paramsModelPtr->AttachObserver(this);
				}

				m_paramSets[i - fixedSetsCount].TakeOver(paramsSetPtr);
			}
		}
	}

	return true;
}


// reimplemented (iprm::IParamsManager)

const IOptionsList* CParamsManagerComp::GetParamsTypeConstraints() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CParamsManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag paramsSetListTag("ParamsSetList", "List of parameter set", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag paramsSetTag("ParamsSet", "Single parameter set", iser::CArchiveTag::TT_GROUP, &paramsSetListTag, true);
	static iser::CArchiveTag nameTag("Name", "Name of set", iser::CArchiveTag::TT_LEAF, &paramsSetTag);
	static iser::CArchiveTag enabledTag("Enabled", "Is parameter set enabled", iser::CArchiveTag::TT_LEAF, &paramsSetTag);
	static iser::CArchiveTag valueTag("Value", "Value of set", iser::CArchiveTag::TT_GROUP, &paramsSetTag, true);

	bool retVal = true;

	int paramsCount = GetParamsSetsCount();
	retVal = retVal && archive.BeginMultiTag(paramsSetListTag, paramsSetTag, paramsCount);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
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
			QString name = GetParamsSetName(i);

			retVal = retVal && archive.Process(name);
		}
		else{
			QString name;

			retVal = retVal && archive.Process(name);
			if (!retVal){
				return false;
			}

			SetParamsSetName(i, name);
		}
		retVal = retVal && archive.EndTag(nameTag);

		bool isEnabled = true;
		if (isStoring){
			isEnabled = IsOptionEnabled(i);
		}

		quint32 version = 0;
		if (		!archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, version) ||
					(version > 3185)){
			bool isEnabled = true;

			retVal = retVal && archive.BeginTag(enabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(enabledTag);
			if (!retVal){
				return false;
			}
		}

		if (!isStoring){
			SetOptionEnabled(i, isEnabled);
		}

		retVal = retVal && archive.BeginTag(valueTag);
		retVal = retVal && paramsSetPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(valueTag);

		retVal = retVal && archive.EndTag(paramsSetTag);
	}

	retVal = retVal && archive.EndTag(paramsSetListTag);

	int selectedIndex = -1;
	if (*m_serializeSelectionAttrPtr){
		selectedIndex = m_selectedIndex;
	}

	static iser::CArchiveTag selectedIndexTag("Selected", "Selected index");
	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	if (!isStoring){
		if (*m_serializeSelectionAttrPtr){
			m_selectedIndex = selectedIndex;
		}
	}

	if (m_selectedIndex >= CParamsManagerComp::GetParamsSetsCount()){
		m_selectedIndex = -1;
	}

	return retVal;
}


// reimplemented (iprm::IOptionsManager)

int CParamsManagerComp::GetOptionOperationFlags(int index) const
{
	return GetIndexOperationFlags(index);
}


bool CParamsManagerComp::SetOptionEnabled(int index, bool isEnabled)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if ((index < fixedSetsCount) || !*m_allowDisabledAttrPtr || !*m_supportEnablingAttrPtr){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount]->isEnabled != isEnabled){
		static ChangeSet changeSet(CF_SET_ENABLE_CHANGED);
		istd::CChangeNotifier notifier(this, changeSet);

		m_paramSets[index - fixedSetsCount]->isEnabled = isEnabled;
	}

	return true;
}


bool CParamsManagerComp::RemoveOption(int index)
{
	return RemoveParamsSet(index);
}


bool CParamsManagerComp::InsertOption(
			const QString& /*optionName*/,
			const QByteArray& /*optionId*/,
			const QString& /*optionDescription*/,
			int /*index*/)
{
	return false;
}


bool CParamsManagerComp::SwapOptions(int index1, int index2)
{
	return SwapParamsSet(index1, index2);
}


bool CParamsManagerComp::SetOptionName(int optionIndex, const QString& optionName)
{
	return SetParamsSetName(optionIndex, optionName);
}


bool CParamsManagerComp::SetOptionDescription(int /*optionIndex*/, const QString& /*optionDescription*/)
{
	return false;
}


// reimplemented (iprm::IOptionsList)

int CParamsManagerComp::GetOptionsFlags() const
{
	return GetIndexOperationFlags(-1);
}


int CParamsManagerComp::GetOptionsCount() const
{
	return GetParamsSetsCount();
}


QString CParamsManagerComp::GetOptionName(int index) const
{
	return GetParamsSetName(index);
}


QString CParamsManagerComp::GetOptionDescription(int index) const
{
	return GetParamsSetDescription(index);
}


QByteArray CParamsManagerComp::GetOptionId(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		int idsCount = m_fixedSetIdsAttrPtr.GetCount();

		if (index < idsCount){
			return m_fixedSetIdsAttrPtr[index];
		}
		else{
			return QByteArray();
		}
	}

	return GetParamsSetName(index).toLocal8Bit();
}


bool CParamsManagerComp::IsOptionEnabled(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (!*m_allowDisabledAttrPtr || (index < fixedSetsCount)){
		return true;
	}

	return m_paramSets[index - fixedSetsCount]->isEnabled;
}


// reimplemented (CParamsManagerCompBase)

bool CParamsManagerComp::IsParameterCreationSupported() const
{
	return m_paramSetsFactPtr.IsValid();
}


int CParamsManagerComp::GetCreatedParamsSetsCount() const
{
	return m_paramSets.count();
}


iprm::IParamsSet* CParamsManagerComp::CreateParamsSet(int /*typeIndex*/) const
{
	if (m_paramSetsFactPtr.IsValid()){
		return m_paramSetsFactPtr.CreateInstance();
	}

	return NULL;
}



// reimplemented (icomp::CComponentBase)

void CParamsManagerComp::OnComponentCreated()
{
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedSetsCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}

	BaseClass::OnComponentCreated();
}


void CParamsManagerComp::OnComponentDestroyed()
{
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();

	for (int setIndex = 0; setIndex < fixedSetsCount; setIndex++){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_fixedParamSetsCompPtr[setIndex]);
		if (modelPtr != NULL && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


