#include "iprm/COptionsManagerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// public methods

// reimplemented (iprm::ISelectionConstraints)

int COptionsManagerComp::GetConstraintsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int COptionsManagerComp::GetOptionsCount() const
{
	if (m_slaveSelectionConstraintsCompPtr.IsValid()){
		return m_options.size() + m_slaveSelectionConstraintsCompPtr->GetOptionsCount();
	}

	return m_options.size();
}


QString COptionsManagerComp::GetOptionName(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < COptionsManagerComp::GetOptionsCount());

	if (m_slaveSelectionConstraintsCompPtr.IsValid()){
		int fixedOptionsCount = m_slaveSelectionConstraintsCompPtr->GetOptionsCount();

		if (fixedOptionsCount - index - 1 >= 0){
			return m_slaveSelectionConstraintsCompPtr->GetOptionName(index);
		}
		else{
			return m_options[index - fixedOptionsCount].optionName;
		}
	}

	return m_options[index].optionName;
}


QString COptionsManagerComp::GetOptionDescription(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < COptionsManagerComp::GetOptionsCount());

	if (m_slaveSelectionConstraintsCompPtr.IsValid()){
		int fixedOptionsCount = m_slaveSelectionConstraintsCompPtr->GetOptionsCount();

		if (fixedOptionsCount - index - 1 >= 0){
			return m_slaveSelectionConstraintsCompPtr->GetOptionDescription(index);
		}
		else{
			return m_options[index - fixedOptionsCount].optionDescription;
		}
	}

	return m_options[index].optionDescription;
}


std::string COptionsManagerComp::GetOptionId(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < COptionsManagerComp::GetOptionsCount());

	if (m_slaveSelectionConstraintsCompPtr.IsValid()){
		int fixedOptionsCount = m_slaveSelectionConstraintsCompPtr->GetOptionsCount();

		if (fixedOptionsCount - index - 1 >= 0){
			return m_slaveSelectionConstraintsCompPtr->GetOptionId(index);
		}
		else{
			return m_options[index - fixedOptionsCount].optionId;
		}
	}

	return m_options[index].optionId;
}


// reimplemented (iprm::IOptionsManager)

void COptionsManagerComp::RemoveOption(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_options.size()));

	istd::CChangeNotifier changePtr(this, CF_OPTION_REMOVED | iprm::ISelectionConstraints::CF_OPTIONS_CHANGED);

	m_options.erase(m_options.begin() + index);
}


bool COptionsManagerComp::InsertOption(
					QString& optionName,
					const std::string& optionId,
					const QString& optionDescription, 
					int index)
{
	OptionInfo optionInfo(optionName, optionId, optionDescription);

	istd::CChangeNotifier changePtr(this, CF_OPTION_ADDED | iprm::ISelectionConstraints::CF_OPTIONS_CHANGED);

	if (index < 0 || index >= int(m_options.size())){
		m_options.push_back(optionInfo);
	}
	else{
		m_options.insert(m_options.begin() + index, optionInfo);
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool COptionsManagerComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	if (!archive.IsStoring()){
		m_options.clear();
	}

	int optionsCount = int(m_options.size());

	static iser::CArchiveTag optionsTag("Options", "List of dynamic options");
	static iser::CArchiveTag optionTag("OptionInfo", "Meta-information about an option");

	retVal = retVal && archive.BeginMultiTag(optionsTag, optionTag, optionsCount);

	for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
		OptionInfo option;

		if (archive.IsStoring()){
			option = m_options[optionIndex];
		}

		static iser::CArchiveTag optionNameTag("Name", "Name of the option");
		retVal = retVal && archive.BeginTag(optionNameTag);
		retVal = retVal && archive.Process(option.optionName);
		retVal = retVal && archive.EndTag(optionNameTag);

		static iser::CArchiveTag optionDescriptionTag("Description", "Description of the option");
		retVal = retVal && archive.BeginTag(optionDescriptionTag);
		retVal = retVal && archive.Process(option.optionDescription);
		retVal = retVal && archive.EndTag(optionDescriptionTag);

		static iser::CArchiveTag optionIdTag("ID", "ID of the option");
		retVal = retVal && archive.BeginTag(optionIdTag);
		retVal = retVal && archive.Process(option.optionId);
		retVal = retVal && archive.EndTag(optionIdTag);

		if (!archive.IsStoring() && retVal){
			m_options.push_back(option);
		}
	}

	retVal = retVal && archive.EndTag(optionsTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void COptionsManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


} // namespace iprm


