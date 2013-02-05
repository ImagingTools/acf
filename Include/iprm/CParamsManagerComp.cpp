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
	int actualSetsCount = GetParamsSetsCount();

	if (count < actualSetsCount){
		int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
		if (count < fixedSetsCount){
			return false;
		}

		istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED | CF_MODEL);

		while (m_paramSets.size() > count - fixedSetsCount){
			m_paramSets.removeLast();
		}
	}
	else if (count > actualSetsCount){
		if (!m_paramSetsFactPtr.IsValid()){
			return false;
		}

		int setsToAdd = count - GetParamsSetsCount();
		for (int i = 0; i < setsToAdd; ++i){
			IParamsSet* newParamsSetPtr = m_paramSetsFactPtr.CreateInstance();
			if (newParamsSetPtr == NULL){
				return false;
			}

			istd::CChangeNotifier notifier(this, CF_MODEL | CF_OPTIONS_CHANGED);

			m_paramSets.push_back(ParamSet());
			ParamSet& paramsSet = m_paramSets.back();

			paramsSet.paramSetPtr.SetPtr(newParamsSetPtr);
			paramsSet.name = m_defaultSetNameAttrPtr.IsValid() ? *m_defaultSetNameAttrPtr : "unnamed";

			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
			if (modelPtr != NULL){
				modelPtr->AttachObserver(this);
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

	QString defaultSetName;
	if (m_defaultSetNameAttrPtr.IsValid()){
		defaultSetName = *m_defaultSetNameAttrPtr;
		if (defaultSetName.contains("%1")){			
			QString tmpName;
			for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
				tmpName = defaultSetName;
				tmpName.replace(QString("%1"), QString::number(suffixIndex));
				if (FindParamSetIndex(tmpName) < 0){
					defaultSetName = tmpName;
					break;
				}
			}
		}
	}
	else {
		for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
			defaultSetName = QObject::tr("unnamed%1").arg(suffixIndex);
			if (FindParamSetIndex(defaultSetName) < 0){
				break;
			}
		}		
	}


	ParamSet paramSet;
	
	paramSet.paramSetPtr.SetPtr(newParamsSetPtr);
	paramSet.name = defaultSetName;

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(newParamsSetPtr);
	if (modelPtr != NULL){
		modelPtr->AttachObserver(this);
	}

	if (index >= 0){
		int insertIndex = index - fixedParamsCount;

		m_paramSets.insert(m_paramSets.begin() + insertIndex, paramSet);

		return index;
	}
	else{
		m_paramSets.push_back(paramSet);

		return int(m_paramSets.size()) - 1;
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

	m_paramSets.removeAt(removeIndex);

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

	ParamSet& paramsSet1 = m_paramSets[index1 - fixedParamsCount];
	ParamSet& paramsSet2 = m_paramSets[index2 - fixedParamsCount];

	paramsSet1.paramSetPtr.Swap(paramsSet2.paramSetPtr);
	qSwap(paramsSet1.name, paramsSet2.name);

	return true;
}


IParamsSet* CParamsManagerComp::GetParamsSet(int index) const
{
	Q_ASSERT((index >= 0) && (index < CParamsManagerComp::GetParamsSetsCount()));

	int fixedSetsCount = m_fixedParamSetsCompPtr.GetCount();
	if (index < fixedSetsCount){
		return m_fixedParamSetsCompPtr[index];
	}

	return const_cast<IParamsSet*>(m_paramSets[index - fixedSetsCount].paramSetPtr.GetPtr());
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
			return QObject::tr("%1_%2").arg(m_defaultSetNameAttrPtr.IsValid() ? *m_defaultSetNameAttrPtr : "unnamed").arg(index - namesCount + 1);
		}
	}

	return m_paramSets[index - fixedSetsCount].name;
}


bool CParamsManagerComp::SetParamsSetName(int index, const QString& name)
{
	Q_ASSERT((index >= 0) && (index < GetParamsSetsCount()));

	int fixedSetsCount = m_fixedSetNamesAttrPtr.GetCount();
	if (index < fixedSetsCount){
		return false;
	}

	if (m_paramSets[index - fixedSetsCount].name != name){
		istd::CChangeNotifier notifier(this, CF_SET_NAME_CHANGED | CF_OPTION_RENAMED | CF_MODEL);

		m_paramSets[index - fixedSetsCount].name = name;
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
		if (m_paramSets.at(i).name == name){
			return i;
		}
	}

	return -1;
}


} // namespace iprm


