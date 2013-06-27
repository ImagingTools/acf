#include "iprm/CParamsManagerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TChangeDelegator.h"

#include "imod/IModel.h"

#include "iprm/IParamsSet.h"


namespace iprm
{


CParamsManagerComp::CParamsManagerComp()
:	m_selectedIndex(-1)
{
}


bool CParamsManagerComp::SetSetsCount(int count)
{
	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (count < fixedSetsCount){
		return false;
	}

	int actualSetsCount = m_paramSets.size();
	if (count != actualSetsCount){
		if (!m_paramSetsFactPtr.IsValid()){
			return false;
		}

		istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED | CF_MODEL);

		m_paramSets.resize(count - fixedSetsCount);

		if (count > actualSetsCount){
			for (int i = actualSetsCount; i < count; ++i){
				IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
				if (newParamsSetPtr == NULL){
					return false;
				}

				istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED);

				ParamSetPtr paramsSetPtr(new imod::TModelWrap<ParamSet>());

				paramsSetPtr->paramSetPtr.SetPtr(newParamsSetPtr);
				paramsSetPtr->name = GetNewSetName();
				paramsSetPtr->parentPtr = this;

				imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
				if (paramsModelPtr != NULL){
					paramsModelPtr->AttachObserver(paramsSetPtr.GetPtr());
					paramsModelPtr->AttachObserver(this);
				}

				m_paramSets[i].TakeOver(paramsSetPtr);
			}
		}
	}

	return true;
}


// reimplemented (iprm::IParamsManager)

int CParamsManagerComp::GetIndexOperationFlags(int index) const
{
	int retVal = 0;

	if (m_paramSetsFactPtr.IsValid()){
		retVal |= MF_SUPPORT_INSERT | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME;

		if (!m_paramSets.isEmpty() && (index < CParamsManagerComp::GetParamsSetsCount())){
			retVal |= MF_SUPPORT_DELETE;
		}
	}
	else{
		retVal |= MF_COUNT_FIXED;
	}

	if (index >= 0){
		if (index < m_fixedParamSetsCompPtr.GetCount()){
			retVal &= ~(MF_SUPPORT_INSERT | MF_SUPPORT_DELETE | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME);
		}
	}

	return retVal;
}


int CParamsManagerComp::GetParamsSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + m_paramSets.size();
}


const IOptionsList* CParamsManagerComp::GetParamsTypeConstraints() const
{
	return NULL;
}


int CParamsManagerComp::InsertParamsSet(int /*typeIndex*/, int index)
{
	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (		!m_paramSetsFactPtr.IsValid() ||
				((index >= 0) && (index < fixedParamsCount))){
		return -1;
	}

	IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
	if (newParamsSetPtr == NULL){
		return -1;
	}

	istd::CChangeNotifier notifier(this, CF_SET_INSERTED | CF_OPTIONS_CHANGED | CF_MODEL);

	ParamSetPtr paramsSetPtr(new imod::TModelWrap<ParamSet>());

	paramsSetPtr->paramSetPtr.SetPtr(newParamsSetPtr);
	paramsSetPtr->name = GetNewSetName();
	paramsSetPtr->parentPtr = this;

	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
	if (paramsModelPtr != NULL){
		paramsModelPtr->AttachObserver(paramsSetPtr.GetPtr());
		paramsModelPtr->AttachObserver(this);
	}

	m_paramSets.push_back(ParamSetPtr());
	if (index >= 0){
		int insertIndex = index - fixedParamsCount;

		for (int i = m_paramSets.size() - 1; i > insertIndex; --i){
			m_paramSets[i].TakeOver(m_paramSets[i - 1]);
		}
		m_paramSets[insertIndex].TakeOver(paramsSetPtr);

		return index;
	}
	else{
		m_paramSets.last().TakeOver(paramsSetPtr);

		return m_paramSets.size() - 1;
	}
}


bool CParamsManagerComp::RemoveParamsSet(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < CParamsManagerComp::GetParamsSetsCount());

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if (index < fixedParamsCount){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_CHANGED | CF_SELECTION_CHANGED | CF_MODEL);
	
	int removeIndex = index - fixedParamsCount;

	for (int i = removeIndex; i < m_paramSets.size() - 1; ++i){
		m_paramSets[i].TakeOver(m_paramSets[i + 1]);
	}
	m_paramSets.pop_back();

	m_selectedIndex = index - 1;

	return true;
}


bool CParamsManagerComp::SwapParamsSet(int index1, int index2)
{
	Q_ASSERT(index1 >= 0);
	Q_ASSERT(index1 < CParamsManagerComp::GetParamsSetsCount());
	Q_ASSERT(index2 >= 0);
	Q_ASSERT(index2 < CParamsManagerComp::GetParamsSetsCount());

	int fixedParamsCount = m_fixedParamSetsCompPtr.GetCount();

	if ((index1 < fixedParamsCount) || (index2 < fixedParamsCount)){
		return false;
	}

	istd::CChangeNotifier notifier(this, CF_SET_REMOVED | CF_OPTIONS_CHANGED | CF_SELECTION_CHANGED | CF_MODEL);

	ParamSet& paramsSet1 = *m_paramSets[index1 - fixedParamsCount];
	ParamSet& paramsSet2 = *m_paramSets[index2 - fixedParamsCount];

	paramsSet1.paramSetPtr.Swap(paramsSet2.paramSetPtr);
	qSwap(paramsSet1.name, paramsSet2.name);

	istd::CChangeNotifier notifier1(&paramsSet1);
	istd::CChangeNotifier notifier2(&paramsSet2);

	return true;
}


IParamsSet* CParamsManagerComp::GetParamsSet(int index) const
{
	Q_ASSERT((index >= 0) && (index < CParamsManagerComp::GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return m_fixedParamSetsCompPtr[index];
	}

	if (m_elementIndexParamId.IsValid() || m_elementNameParamId.IsValid()){
		return const_cast<ParamSet*>(m_paramSets[index - fixedSetsCount].GetPtr());
	}
	else{
		return const_cast<IParamsSet*>(m_paramSets[index - fixedSetsCount]->paramSetPtr.GetPtr());
	}
}


QString CParamsManagerComp::GetParamsSetName(int index) const
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		int namesCount = m_fixedSetNamesAttrPtr.GetCount();

		if (index < namesCount){
			return m_fixedSetNamesAttrPtr[index];
		}
		else{
			return (*m_defaultSetNameAttrPtr).arg(index - namesCount + 1);
		}
	}

	return m_paramSets[index - fixedSetsCount]->name;
}


bool CParamsManagerComp::SetParamsSetName(int index, const QString& name)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if (index < fixedSetsCount){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount]->name != name){
		istd::CChangeNotifier notifier(this, CF_SET_NAME_CHANGED | CF_OPTION_RENAMED | CF_MODEL);

		m_paramSets[index - fixedSetsCount]->name = name;
	}

	return true;
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CParamsManagerComp::GetSelectionConstraints() const
{
	return this;
}


int CParamsManagerComp::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CParamsManagerComp::SetSelectedOptionIndex(int index)
{
	if (index < GetOptionsCount()){
		if (index != m_selectedIndex){
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED | CF_OPTIONS_CHANGED | CF_MODEL);

			m_selectedIndex = index;
		}

		return true;
	}

	return false;
}


ISelectionParam* CParamsManagerComp::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CParamsManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag paramsSetListTag("ParamsSetList", "List of parameter set");
	static iser::CArchiveTag paramsSetTag("ParamsSet", "Single parameter set", true);
	static iser::CArchiveTag nameTag("Name", "Name of set");
	static iser::CArchiveTag valueTag("Value", "Value of set", true);

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

		retVal = retVal && archive.BeginTag(valueTag);
		retVal = retVal && paramsSetPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(valueTag);

		retVal = retVal && archive.EndTag(paramsSetTag);
	}

	retVal = retVal && archive.EndTag(paramsSetListTag);

	static iser::CArchiveTag selectedIndexTag("Selected", "Selected index");
	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(m_selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	return retVal;
}


// reimplemented (iprm::IOptionsList)

int CParamsManagerComp::GetOptionsFlags() const
{
	return SCF_NONE;
}


int CParamsManagerComp::GetOptionsCount() const
{
	return GetParamsSetsCount();
}


QString CParamsManagerComp::GetOptionName(int index) const
{
	return GetParamsSetName(index);
}


QString CParamsManagerComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CParamsManagerComp::GetOptionId(int index) const
{
	return GetParamsSetName(index).toLocal8Bit();
}


bool CParamsManagerComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// protected methods

QString CParamsManagerComp::GetNewSetName() const
{
	if ((*m_defaultSetNameAttrPtr).contains("%1")){			
		for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
			QString name = (*m_defaultSetNameAttrPtr).arg(suffixIndex);
			if (FindParamSetIndex(name) < 0){
				return name;
			}
		}
	}

	return *m_defaultSetNameAttrPtr;
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

// private methods

int CParamsManagerComp::FindParamSetIndex(const QString& name) const
{
	int paramsCount = m_paramSets.size();
	for (int i = 0; i < paramsCount; ++i){
		const ParamSetPtr& paramSetPtr = m_paramSets[i];
		if (paramSetPtr.IsValid() && (paramSetPtr->name == name)){
			return i;
		}
	}

	return -1;
}


// public methods of embedded class

CParamsManagerComp::ParamSet::ParamSet()
{
	parentPtr = NULL;
}


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CParamsManagerComp::ParamSet::GetParamIds(bool editableOnly) const
{
	Q_ASSERT(paramSetPtr.IsValid());
	Q_ASSERT(parentPtr != NULL);

	IParamsSet::Ids ids = paramSetPtr->GetParamIds(editableOnly);

	if (!editableOnly){
		if (parentPtr->m_elementIndexParamId.IsValid()){
			ids += *(parentPtr->m_elementIndexParamId);
		}

		if (parentPtr->m_elementNameParamId.IsValid()){
			ids += *(parentPtr->m_elementNameParamId);
		}
	}

	return ids;
}


const iser::ISerializable* CParamsManagerComp::ParamSet::GetParameter(const QByteArray& id) const
{
	Q_ASSERT(paramSetPtr.IsValid());
	Q_ASSERT(parentPtr != NULL);

	if (parentPtr->m_elementIndexParamId.IsValid() && (id == *(parentPtr->m_elementIndexParamId))){
		return this;
	}

	if (parentPtr->m_elementNameParamId.IsValid() && (id == *(parentPtr->m_elementNameParamId))){
		return this;
	}

	return paramSetPtr->GetParameter(id);
}


iser::ISerializable* CParamsManagerComp::ParamSet::GetEditableParameter(const QByteArray& id)
{
	Q_ASSERT(paramSetPtr.IsValid());

	return paramSetPtr->GetEditableParameter(id);
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CParamsManagerComp::ParamSet::GetSelectionConstraints() const
{
	return parentPtr;
}


int CParamsManagerComp::ParamSet::GetSelectedOptionIndex() const
{
	Q_ASSERT(parentPtr != NULL);

	int retIndex = 0;
	for (		ParamSets::ConstIterator iter = parentPtr->m_paramSets.constBegin();
				iter != parentPtr->m_paramSets.constEnd();
				++iter, ++retIndex){
		if (iter->GetPtr() == this){
			return retIndex + parentPtr->m_fixedParamSetsCompPtr.GetCount();
		}
	}

	return -1;
}


bool CParamsManagerComp::ParamSet::SetSelectedOptionIndex(int /*index*/)
{
	return false;
}


ISelectionParam* CParamsManagerComp::ParamSet::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::INameParam)

const QString& CParamsManagerComp::ParamSet::GetName() const
{
	return name;
}


void CParamsManagerComp::ParamSet::SetName(const QString& /*name*/)
{	// it is read only interface imeplementation
}


bool CParamsManagerComp::ParamSet::IsNameFixed() const
{
	return true;
}


// reimplemented (iser::ISerializable)

bool CParamsManagerComp::ParamSet::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace iprm


