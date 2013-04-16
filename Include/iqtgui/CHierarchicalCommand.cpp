#include "iqtgui/CHierarchicalCommand.h"


namespace iqtgui
{


CHierarchicalCommand::CHierarchicalCommand(const QString& name, int priority, int staticFlags, int groupId)
:	BaseClass(name, NULL),
	m_priority(priority),
	m_staticFlags(0),
	m_groupId(groupId)
{
	BaseClass3::SetName(name);
	SetStaticFlags(staticFlags);

	connect(this, SIGNAL(triggered()), SLOT(OnTriggered()), Qt::QueuedConnection);
}


void CHierarchicalCommand::SetPriority(int priority)
{
	m_priority = priority;
}


void CHierarchicalCommand::SetGroupId(int groupId)
{
	m_groupId = groupId;
}


void CHierarchicalCommand::SetStaticFlags(int flags)
{
	m_staticFlags = flags;
	setCheckable((flags & CF_ONOFF) != 0);
}


void CHierarchicalCommand::ResetChilds()
{
	m_childs.Reset();
}


void CHierarchicalCommand::InsertChild(CHierarchicalCommand* commandPtr, bool releaseFlag, int index)
{
	if ((index >= 0) && (index < m_childs.GetCount())){
		m_childs.InsertAt(index, commandPtr, releaseFlag);
	}
	else{
		m_childs.PushBack(commandPtr, releaseFlag);
	}
}


void CHierarchicalCommand::RemoveChild(int index)
{
	m_childs.RemoveAt(index);
}


void CHierarchicalCommand::JoinLinkFrom(const ibase::IHierarchicalCommand* rootPtr)
{
	Q_ASSERT(rootPtr != NULL);

	int commandsCount = rootPtr->GetChildsCount();
	for (int commandIndex = 0; commandIndex < commandsCount; ++commandIndex){
		CHierarchicalCommand* childCommandPtr =  dynamic_cast<CHierarchicalCommand*>(rootPtr->GetChild(commandIndex));
		if (childCommandPtr != NULL){
			int sameCommandIndex = FindTheSameCommand(*childCommandPtr);
			if (sameCommandIndex >= 0){
				const Childs::ElementType& element = m_childs.GetElementAt(sameCommandIndex);
				Q_ASSERT(element.pointer != NULL);

				if (element.pointer != childCommandPtr){
					if (element.releaseFlag){
						element.pointer->JoinLinkFrom(childCommandPtr);
					}
					else{
						CHierarchicalCommand* newCommandPtr = new CHierarchicalCommand(
									childCommandPtr->GetName(),
									childCommandPtr->GetPriority(),
									childCommandPtr->GetStaticFlags());

						if (newCommandPtr != NULL){
							newCommandPtr->JoinLinkFrom(element.pointer);
							newCommandPtr->JoinLinkFrom(childCommandPtr);

							m_childs.SetAt(sameCommandIndex, newCommandPtr, true);
						}
					}
				}
			}
			else{
				int position = FindInsertingIndex(childCommandPtr->GetPriority());
				m_childs.InsertAt(position, childCommandPtr, false);
			}
		}
	}
}


void CHierarchicalCommand::SetVisuals(const QString& name, const QString& shortName, const QString& description, const QIcon& icon)
{
	SetName(name);
	BaseClass::setIconText(shortName);
	BaseClass::setStatusTip(description);
	BaseClass::setToolTip(description);
	BaseClass::setIcon(icon);
}


// reimplemented (ibase::ICommand)

int CHierarchicalCommand::GetPriority() const
{
	return m_priority;
}


int CHierarchicalCommand::GetGroupId() const
{
	return m_groupId;
}


int CHierarchicalCommand::GetStaticFlags() const
{
	return m_staticFlags;
}


bool CHierarchicalCommand::Execute(istd::IPolymorphic* contextPtr)
{
	if (IsEnabled() && (contextPtr != this)){
		Q_EMIT trigger();

		return true;
	}
	else{
		return false;
	}
}


// reimplemented (istd::TIHierarchical<Interface>)

int CHierarchicalCommand::GetChildsCount() const
{
	return m_childs.GetCount();
}


ibase::ICommand* CHierarchicalCommand::GetChild(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < m_childs.GetCount());

	return m_childs.GetAt(index);
}


// reimplemented (iprm::INameParam)

void CHierarchicalCommand::SetName(const QString& name)
{
	BaseClass::setText(name);
	BaseClass3::SetName(name);
}


// reimplemented (iprm::IEnableableParam)

bool CHierarchicalCommand::SetEnabled(bool isEnabled)
{
	BaseClass::setEnabled(isEnabled);

	return BaseClass2::SetEnabled(isEnabled);
}


// reimplemented (iser::ISerializable)

bool CHierarchicalCommand::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// protected slots

void CHierarchicalCommand::OnTriggered()
{
	Execute(this);
}


// protected methods

int CHierarchicalCommand::FindTheSameCommand(const ibase::IHierarchicalCommand& command) const
{
	const QString& elementName = command.GetName();

	int childsCount = m_childs.GetCount();
	for (int i = 0; i < childsCount; ++i){
		CHierarchicalCommand* childPtr = m_childs.GetAt(i);
		Q_ASSERT(childPtr != NULL);

		if (childPtr->GetName().compare(elementName, Qt::CaseInsensitive) == 0){
			return i;
		}
	}

	return -1;
}


int CHierarchicalCommand::FindInsertingIndex(int priority) const
{
	int childsCount = m_childs.GetCount();

	for (int i = 0; i < childsCount; ++i){
		CHierarchicalCommand* childPtr = m_childs.GetAt(i);
		Q_ASSERT(childPtr != NULL);

		if (priority < childPtr->GetPriority()){
			return i;
		}
	}

	return childsCount;
}


} // namespace iqtgui


