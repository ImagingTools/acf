#include "iprm/COptionsManager.h"


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QUuid>

// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


const istd::IChangeable::ChangeSet s_enableChangeSet(IOptionsList::CF_OPTIONS_CHANGED, QObject::tr("Enable option"));
const istd::IChangeable::ChangeSet s_insertOptionChangeSet(IOptionsList::CF_OPTIONS_CHANGED, IOptionsManager::CF_OPTION_ADDED, QObject::tr("Insert option"));
const istd::IChangeable::ChangeSet s_removeOptionChangeSet(IOptionsList::CF_OPTIONS_CHANGED, IOptionsManager::CF_OPTION_REMOVED, QObject::tr("Remove option"));
const istd::IChangeable::ChangeSet s_moveOptionChangeSet(IOptionsList::CF_OPTIONS_CHANGED, QObject::tr("Move option"));
const istd::IChangeable::ChangeSet s_renameOptionChangeSet(IOptionsList::CF_OPTION_RENAMED, QObject::tr("Rename option"));
const istd::IChangeable::ChangeSet s_setDescriptionChangeSet(IOptionsList::CF_OPTIONS_CHANGED, QObject::tr("Change description"));


// public methods

COptionsManager::COptionsManager()
:	m_slaveSelectionConstraintsPtr(NULL),
	m_isSelectionSerialized(false)
{
	SetSelectionConstraints(this);
}


void COptionsManager::SetFixedOptionsList(const iprm::IOptionsList* slaveSelectionConstraintsPtr)
{
	m_slaveSelectionConstraintsPtr = slaveSelectionConstraintsPtr;
}


void COptionsManager::ResetOptions()
{
	m_options.clear();
}


int COptionsManager::GetOptionIndexById(const QByteArray& optionId) const
{
	if (optionId.isEmpty()){
		return NO_SELECTION;
	}

	int optionsCount = COptionsManager::GetOptionsCount();

	for (int optionIndex = 0; optionIndex < optionsCount; ++optionIndex){
		if (COptionsManager::GetOptionId(optionIndex) == optionId){
			return optionIndex;
		}
	}

	return NO_SELECTION;
}


int COptionsManager::GetOptionIndexByName(const QString& optionName) const
{
	int optionsCount = COptionsManager::GetOptionsCount();

	for (int optionIndex = 0; optionIndex < optionsCount; ++optionIndex){
		if (COptionsManager::GetOptionName(optionIndex) == optionName){
			return optionIndex;
		}
	}

	return -1;
}


bool COptionsManager::IsSelectionSerialized() const
{
	return m_isSelectionSerialized;
}


void COptionsManager::SetSelectionSerialized(bool  state)
{
	m_isSelectionSerialized = state;
}


// reimplemented (iprm::IOptionsList)

int COptionsManager::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID | SFC_DISABLE_ALLOWED;
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

int COptionsManager::GetOptionOperationFlags(int index) const
{
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	int retVal = OOF_SUPPORT_INSERT;

	int fixedOptionsCount = 0;
	if (m_slaveSelectionConstraintsPtr != NULL){
		fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();
	}

	if (index >= fixedOptionsCount){
		retVal = retVal | OOF_SUPPORT_DELETE | OOF_SUPPORT_RENAME | OOF_SUPPORT_SWAP | OOF_DISABLE_ALLOWED | OOF_SUPPORT_ENABLING;
	}
	return retVal;
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
		istd::CChangeNotifier notifier(this, &s_enableChangeSet);
		Q_UNUSED(notifier);

		m_options[index].isEnabled = isEnabled;
	}

	return true;
}


bool COptionsManager::RemoveOption(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < COptionsManager::GetOptionsCount());

	int fixedOptionsCount = 0;
	if (m_slaveSelectionConstraintsPtr != NULL){
		fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if (index < fixedOptionsCount){
			return false;
		}
	}

	istd::CChangeNotifier notifier(this, &s_removeOptionChangeSet);
	Q_UNUSED(notifier);

	Q_ASSERT(index - fixedOptionsCount < m_options.size());
	m_options.erase(m_options.begin() + index - fixedOptionsCount);

	if (index == BaseClass::GetSelectedOptionIndex()){
		BaseClass::SetSelectedOptionIndex(index - 1);
	}

	return true;
}


bool COptionsManager::InsertOption(
					const QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription,
					int index)
{
	QByteArray realOptionId = optionId;
	if (realOptionId.isEmpty()){
		realOptionId = QUuid::createUuid().toByteArray();
	}

	OptionInfo optionInfo(optionName, realOptionId, optionDescription);

	int fixedOptionsCount = 0;
	if (m_slaveSelectionConstraintsPtr != NULL){
		fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();
	}

	if ((index < 0) || (index >= m_options.size() + fixedOptionsCount)){
		istd::CChangeNotifier notifier(this, &s_insertOptionChangeSet);
		Q_UNUSED(notifier);

		m_options.push_back(optionInfo);
	}
	else{
		if (index < fixedOptionsCount){
			return false;
		}

		istd::CChangeNotifier notifier(this, &s_insertOptionChangeSet);
		Q_UNUSED(notifier);

		m_options.insert(m_options.begin() + index - fixedOptionsCount, optionInfo);

		BaseClass::SetSelectedOptionIndex(index);
	}

	return true;
}


bool COptionsManager::SwapOptions(int index1, int index2)
{
	Q_ASSERT(index1 >= 0);
	Q_ASSERT(index1 < COptionsManager::GetOptionsCount());
	Q_ASSERT(index2 >= 0);
	Q_ASSERT(index2 < COptionsManager::GetOptionsCount());

	int fixedOptionsCount = 0;
	if (m_slaveSelectionConstraintsPtr != NULL){
		fixedOptionsCount = m_slaveSelectionConstraintsPtr->GetOptionsCount();

		if ((index1 < fixedOptionsCount) || (index2 < fixedOptionsCount)){
			return false;
		}
	}

	Q_ASSERT(index1 < m_options.size() + fixedOptionsCount);
	Q_ASSERT(index2 < m_options.size() + fixedOptionsCount);

	if (index1 != index2){
		istd::CChangeNotifier notifier(this, &s_moveOptionChangeSet);
		Q_UNUSED(notifier);

		OptionInfo tempInfo = m_options[index1 - fixedOptionsCount];
		m_options[index1 - fixedOptionsCount] = m_options[index2 - fixedOptionsCount];
		m_options[index2 - fixedOptionsCount] = tempInfo;

		int selectedOptionIndex = BaseClass::GetSelectedOptionIndex();
		if (selectedOptionIndex == index1){
			BaseClass::SetSelectedOptionIndex(index2);
		}
		else if (selectedOptionIndex == index2){
			BaseClass::SetSelectedOptionIndex(index1);
		}
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
		istd::CChangeNotifier notifier(this, &s_renameOptionChangeSet);
		Q_UNUSED(notifier);

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
		istd::CChangeNotifier notifier(this, &s_setDescriptionChangeSet);
		Q_UNUSED(notifier);

		m_options[index].optionDescription = optionDescription;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool COptionsManager::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag optionsTag("Options", "List of dynamic options", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag optionTag("OptionInfo", "Meta-information about an option", iser::CArchiveTag::TT_GROUP, &optionsTag);
	static iser::CArchiveTag optionNameTag("Name", "Name of the option", iser::CArchiveTag::TT_LEAF, &optionTag);
	static iser::CArchiveTag optionDescriptionTag("Description", "Description of the option", iser::CArchiveTag::TT_LEAF, &optionTag);
	static iser::CArchiveTag optionIdTag("ID", "ID of the option", iser::CArchiveTag::TT_LEAF, &optionTag);
	static iser::CArchiveTag enabledTag("Enabled", "Is option enabled", iser::CArchiveTag::TT_LEAF, &optionTag);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

	if (m_isSelectionSerialized){
		retVal = retVal && BaseClass::Serialize(archive);
	}

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

		retVal = retVal && archive.BeginTag(optionTag);

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

		retVal = retVal && archive.EndTag(optionTag);

		if (!isStoring && retVal){
			m_options.push_back(option);
		}
	}

	retVal = retVal && archive.EndTag(optionsTag);

	return retVal;
}


} // namespace iprm


