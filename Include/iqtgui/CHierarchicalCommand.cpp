#include <iqtgui/CHierarchicalCommand.h>


// ACF includes
#include <istd/TDelPtr.h>


namespace iqtgui
{


CHierarchicalCommand::CHierarchicalCommand(const QString& name, int priority, int staticFlags, int groupId, const QByteArray& commandId)
:	BaseClass(name, NULL),
	m_priority(priority),
	m_staticFlags(0),
	m_groupId(groupId),
	m_commandId(commandId)
{
	BaseClass3::SetName(QString(name).replace('&', ""));
	SetStaticFlags(staticFlags);

	connect(this, SIGNAL(triggered()), SLOT(OnTriggered()), Qt::QueuedConnection);
}


void CHierarchicalCommand::SetPriority(int priority)
{
	m_priority = priority;
}


QByteArray CHierarchicalCommand::GetCommandId() const
{
	return m_commandId;
}


void CHierarchicalCommand::SetCommandId(const QByteArray& commandId)
{
	m_commandId = commandId;
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
	BaseClass::setIconVisibleInMenu(icon.isNull() == false);
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
	BaseClass3::SetName(QString(name).replace('&', ""));
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


// reimplemented (istd::IChangeable)

int CHierarchicalCommand::GetSupportedOperations() const
{
	return SO_COPY | SO_COMPARE;
}


bool CHierarchicalCommand::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CHierarchicalCommand* commandPtr = dynamic_cast<const CHierarchicalCommand*>(&object);
	if (commandPtr != NULL){
		*this = *commandPtr;

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CHierarchicalCommand::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonedPtr(new CHierarchicalCommand);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr;
	}

	return istd::IChangeableUniquePtr();
}



bool CHierarchicalCommand::IsEqual(const istd::IChangeable& object) const
{
	const CHierarchicalCommand* commandPtr = dynamic_cast<const CHierarchicalCommand*>(&object);
	if (commandPtr != NULL){
		return (*this == *commandPtr);
	}

	return false;
}


// operators

bool CHierarchicalCommand::operator==(const CHierarchicalCommand& command) const
{
	int childsCount = GetChildsCount();
	if (childsCount != command.GetChildsCount()){
		return false;
	}

	if (		(GetName() != command.GetName()) ||
				(isChecked() != command.isChecked()) ||
				(isVisible() != command.isVisible())){
		return false;
	}

	for (int childIndex = 0; childIndex < childsCount; ++childIndex){
		const CHierarchicalCommand* childPtr = dynamic_cast<const CHierarchicalCommand*>(GetChild(childIndex));
		const CHierarchicalCommand* commandChildPtr = dynamic_cast<const CHierarchicalCommand*>(command.GetChild(childIndex));
		if (childPtr != commandChildPtr){
			if (childPtr == NULL){
				return false;
			}

			if (commandChildPtr == NULL){
				return false;
			}

			if (*childPtr != *commandChildPtr){
				return false;
			}
		}
	}

	return true;
}


bool CHierarchicalCommand::operator!=(const CHierarchicalCommand& command) const
{
	return !operator==(command);
}


CHierarchicalCommand& CHierarchicalCommand::operator=(const CHierarchicalCommand& command)
{
	ResetChilds();

	int childsCount = command.GetChildsCount();

	m_priority = command.m_priority;
	m_staticFlags = command.m_staticFlags;
	m_groupId = command.m_groupId;

	setText(command.text());
	setIcon(command.icon());
	setToolTip(command.toolTip());

	for (int i = 0; i < childsCount; ++i){
		const CHierarchicalCommand* commandChildPtr = dynamic_cast<const CHierarchicalCommand*>(command.GetChild(i));
		if (commandChildPtr != NULL){
			CHierarchicalCommand* childPtr = new CHierarchicalCommand();
			*childPtr = *commandChildPtr;

			InsertChild(childPtr, true);
		}
	}

	return *this;
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


