#include "iprm/CFilteredOptionsListComp.h"


namespace iprm
{


// reimplemented (iprm::IOptionsList)

int CFilteredOptionsListComp::GetOptionsFlags() const
{
	if (m_inputOptionsCompPtr.IsValid()){
		return m_inputOptionsCompPtr->GetOptionsFlags();
	}

	return SCF_NONE;
}


int CFilteredOptionsListComp::GetOptionsCount() const
{
	return m_options.count();
}


QString CFilteredOptionsListComp::GetOptionName(int index) const
{
	Q_ASSERT(m_options.count() > index);
	Q_ASSERT(index >= 0);

	return m_options[index].name;
}


QString CFilteredOptionsListComp::GetOptionDescription(int index) const
{
	Q_ASSERT(index >= 0);

	if (index < m_options.count()){
		return m_options[index].description;
	}

	return QString();
}


QByteArray CFilteredOptionsListComp::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);

	if (index < m_options.count()){
		return m_options[index].id;
	}

	return QByteArray();
}


bool CFilteredOptionsListComp::IsOptionEnabled(int index) const
{
	Q_ASSERT(index >= 0);

	if (index < m_options.count()){
		return m_options[index].isEnabled;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CFilteredOptionsListComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	UpdateOptions();
}


// private methods

void CFilteredOptionsListComp::UpdateOptions()
{
	if (!m_inputOptionsCompPtr.IsValid()){
		return;
	}

	static const istd::IChangeable::ChangeSet changeSet(CF_OPTIONS_CHANGED);
	istd::CChangeNotifier notifier(this, &changeSet);
	Q_UNUSED(notifier);

	m_options.clear();

	for (int i = 0; i < m_includeIdsAttrPtr.GetCount(); ++i){
		QByteArray id = m_includeIdsAttrPtr[i];

		int index = GetOptionById(*m_inputOptionsCompPtr.GetPtr(), id);
		if (index >= 0){
			Option option;

			option.id = id;
			option.name = m_inputOptionsCompPtr->GetOptionName(index);
			option.description = m_inputOptionsCompPtr->GetOptionDescription(index);
			option.isEnabled = m_inputOptionsCompPtr->IsOptionEnabled(index);

			m_options.push_back(option);
		}
	}
}


// private static methods
	
int CFilteredOptionsListComp::GetOptionById(const iprm::IOptionsList& options, const QByteArray& optionId)
{
	int optionsCount = options.GetOptionsCount();

	for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
		QByteArray id = options.GetOptionId(optionIndex);

		if (optionId == id){
			return optionIndex;
		}
	}

	return -1;
}


} // namespace iprm


