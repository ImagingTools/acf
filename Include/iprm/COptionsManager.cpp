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


void COptionsManager::SetSlaveSelectionConstraints(const iprm::IOptionsList* slaveSelectionConstraintsPtr)
{
	m_slaveSelectionConstraintsPtr = slaveSelectionConstraintsPtr;
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

		if (fixedOptionsCount - index - 1 >= 0){
			return m_slaveSelectionConstraintsPtr->GetOptionName(index);
		}
		else{
			return m_options[index - fixedOptionsCount].optionName;
		}
	}

	return m_options[index].optionName;
}


QString COptionsManager::GetOptionDescription(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (fixedOptionsCount - index - 1 >= 0){
			return m_slaveSelectionConstraintsPtr->GetOptionDescription(index);
		}
		else{
			return m_options[index - fixedOptionsCount].optionDescription;
		}
	}

	return m_options[index].optionDescription;
}


QByteArray COptionsManager::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_slaveSelectionConstraintsPtr != NULL){
		int fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (fixedOptionsCount - index - 1 >= 0){
			return m_slaveSelectionConstraintsPtr->GetOptionId(index);
		}
		else{
			return m_options[index - fixedOptionsCount].optionId;
		}
	}

	return m_options[index].optionId;
}


bool COptionsManager::IsOptionEnabled(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	return m_options[index].isEnabled;
}


void COptionsManager::SetOptionEnabled(int index, bool isEnabled)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	if (m_options[index].isEnabled != isEnabled){
		istd::CChangeNotifier changePtr(this, CF_OPTIONS_CHANGED | CF_MODEL);

		m_options[index].isEnabled = isEnabled;
	}
}


// reimplemented (iprm::IOptionsManager)

bool COptionsManager::RemoveOption(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_options.size()));

	istd::CChangeNotifier changePtr(this, CF_OPTION_REMOVED | CF_OPTIONS_CHANGED | CF_MODEL);

	m_options.erase(m_options.begin() + index);

	return true;
}


bool COptionsManager::InsertOption(
					QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription, 
					int index)
{
	OptionInfo optionInfo(optionName, optionId, optionDescription);

	istd::CChangeNotifier changePtr(this, CF_OPTION_ADDED | CF_OPTIONS_CHANGED | CF_MODEL);

	if (index < 0 || index >= int(m_options.size())){
		m_options.push_back(optionInfo);
	}
	else{
		m_options.insert(m_options.begin() + index, optionInfo);
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

		if (!isStoring && retVal){
			m_options.push_back(option);
		}
	}

	retVal = retVal && archive.EndTag(optionsTag);

	return retVal;
}


} // namespace iprm


