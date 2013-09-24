#include "iprm/COptionsManager.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// public methods

COptionsManager::COptionsManager()
	:m_slaveSelectionConstraintsPtr(NULL)
{
}


void COptionsManager::SetFixedOptionsList(const iprm::IOptionsList* slaveSelectionConstraintsPtr)
{
	m_slaveSelectionConstraintsPtr = slaveSelectionConstraintsPtr;
}


void COptionsManager::ResetOptions()
{
	m_options.clear();
}


// reimplemented (iprm::IOptionsList)

int COptionsManager::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID | SCF_SUPPORT_DISABLED;
}


int COptionsManager::GetOptionsCount() const
{
	if (m_slaveSelectionConstraintsPtr != NULL){
		return m_options.size() + m_slaveSelectionConstraintsPtr->GetOptionsCount();
	}

	return m_options.size();
}


QString COptionsManager::GetOptionName(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return m_slaveSelectionConstraintsPtr->GetOptionName(index);
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	return m_options[index].optionName;
}


QString COptionsManager::GetOptionDescription(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return m_slaveSelectionConstraintsPtr->GetOptionDescription(index);
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	return m_options[index].optionDescription;
}


QByteArray COptionsManager::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return m_slaveSelectionConstraintsPtr->GetOptionId(index);
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	return m_options[index].optionId;
}


bool COptionsManager::IsOptionEnabled(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return m_slaveSelectionConstraintsPtr->IsOptionEnabled(index);
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	return m_options[index].isEnabled;
}


// reimplemented (iprm::IOptionsManager)

int COptionsManager::GetOptionOperationFlags(int /*index*/) const
{
	return OOF_SUPPORT_INSERT | OOF_SUPPORT_DELETE;
}


bool COptionsManager::SetOptionEnabled(int index, bool isEnabled)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return false;
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	if (m_options[index].isEnabled != isEnabled){
		istd::CChangeNotifier changePtr(this, CF_OPTIONS_CHANGED | CF_MODEL);

		m_options[index].isEnabled = isEnabled;
	}

	return true;
}


bool COptionsManager::RemoveOption(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return false;
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	istd::CChangeNotifier changePtr(this, CF_OPTION_REMOVED | CF_OPTIONS_CHANGED | CF_MODEL);

	m_options.erase(m_options.begin() + index);

	return true;
}


bool COptionsManager::InsertOption(
					const QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription,
					int index)
{
	OptionInfo optionInfo(optionName, optionId, optionDescription);

	if (index < 0 || index >= int(m_options.size())){
		istd::CChangeNotifier changePtr(this, CF_OPTION_ADDED | CF_OPTIONS_CHANGED | CF_MODEL);

		m_options.push_back(optionInfo);
	}
	else{
		if (m_slaveSelectionConstraintsPtr != NULL){
			int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

			if (index < fixedOptionsCount){
				return false;
			}

			index -= fixedOptionsCount;
		}

		istd::CChangeNotifier changePtr(this, CF_OPTION_ADDED | CF_OPTIONS_CHANGED | CF_MODEL);

		m_options.insert(m_options.begin() + index, optionInfo);
	}

	return true;
}


bool COptionsManager::SwapOptions(int index1, int index2)
{
	Q_ASSERT(index1 >= 0);
	Q_ASSERT(index1 < COptionsManager::GetOptionsCount());
	Q_ASSERT(index2 >= 0);
	Q_ASSERT(index2 < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if ((index1 < fixedOptionsCount) || (index2 < fixedOptionsCount)){
			return false;
		}

		index1 -= fixedOptionsCount;
		index2 -= fixedOptionsCount;
	}

	Q_ASSERT(index1 < m_options.size());
	Q_ASSERT(index2 < m_options.size());

	if (index1 != index2){
		istd::CChangeNotifier changePtr(this, CF_OPTIONS_CHANGED | CF_MODEL);

		OptionInfo tempInfo = m_options[index1];
		m_options[index1] = m_options[index2];
		m_options[index2] = tempInfo;
	}

	return true;
}


bool COptionsManager::SetOptionName(int index, const QString& optionName)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return false;
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	if (m_options[index].optionName != optionName){
		istd::CChangeNotifier changePtr(this, CF_OPTION_RENAMED | CF_MODEL);

		m_options[index].optionName = optionName;
	}

	return true;
}


bool COptionsManager::SetOptionDescription(int index, const QString& optionDescription)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return false;
		}

		index -= fixedOptionsCount;
	}

	Q_ASSERT(index < m_options.size());
	if (m_options[index].optionDescription != optionDescription){
		istd::CChangeNotifier changePtr(this, CF_OPTIONS_CHANGED | CF_MODEL);

		m_options[index].optionDescription = optionDescription;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool COptionsManager::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag optionsTag("Options", "List of dynamic options");
	static iser::CArchiveTag optionTag("OptionInfo", "Meta-information about an option");
	static iser::CArchiveTag optionNameTag("Name", "Name of the option");
	static iser::CArchiveTag optionDescriptionTag("Description", "Description of the option");
	static iser::CArchiveTag optionIdTag("ID", "ID of the option");
	static iser::CArchiveTag enabledTag("Enabled", "Is option enabled");

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

	if (!isStoring){
		m_options.clear();
	}

	int optionsCount = int(m_options.size());

	retVal = retVal && archive.BeginMultiTag(optionsTag, optionTag, optionsCount);

	for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
		OptionInfo option;

		if (isStoring){
			option = m_options[optionIndex];
		}

		retVal = retVal && archive.BeginTag(optionNameTag);
		retVal = retVal && archive.Process(option.optionName);
		retVal = retVal && archive.EndTag(optionNameTag);

		retVal = retVal && archive.BeginTag(optionDescriptionTag);
		retVal = retVal && archive.Process(option.optionDescription);
		retVal = retVal && archive.EndTag(optionDescriptionTag);

		retVal = retVal && archive.BeginTag(optionIdTag);
		retVal = retVal && archive.Process(option.optionId);
		retVal = retVal && archive.EndTag(optionIdTag);

		retVal = retVal && archive.BeginTag(enabledTag);
		retVal = retVal && archive.Process(option.isEnabled);
		retVal = retVal && archive.EndTag(enabledTag);

		if (!isStoring && retVal){
			m_options.push_back(option);
		}
	}

	retVal = retVal && archive.EndTag(optionsTag);

	return retVal;
}


} // namespace iprm


