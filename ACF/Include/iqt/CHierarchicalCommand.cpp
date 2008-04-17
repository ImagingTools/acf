#include "iqt/CHierarchicalCommand.h"


namespace iqt
{


CHierarchicalCommand::CHierarchicalCommand(const istd::CString& name, int priority, int staticFlags)
:	BaseClass(iqt::GetQString(name), NULL),
	m_priority(priority),
	m_staticFlags(staticFlags)
{
	BaseClass2::SetName(name);
}


void CHierarchicalCommand::SetPriority(int priority)
{
	m_priority = priority;
}


void CHierarchicalCommand::SetStaticFlags(int flags)
{
	m_staticFlags = flags;
}


void CHierarchicalCommand::ResetChilds()
{
	m_childs.Reset();
}


void CHierarchicalCommand::InsertChild(CHierarchicalCommand* commandPtr, bool releaseFlag)
{
	m_childs.PushBack(commandPtr, releaseFlag);
}


void CHierarchicalCommand::JoinLinkFrom(const idoc::IHierarchicalCommand* rootPtr)
{
	I_ASSERT(rootPtr != NULL);

	int commandsCount = rootPtr->GetChildsCount();
	for (int commandIndex = 0; commandIndex < commandsCount; ++commandIndex){
		CHierarchicalCommand* childCommandPtr =  dynamic_cast<CHierarchicalCommand*>(rootPtr->GetChild(commandIndex));
		if (childCommandPtr != NULL){
			int sameCommandIndex = FindTheSameCommand(*childCommandPtr);
			if (sameCommandIndex >= 0){
				const Childs::ElementType& element = m_childs.GetElementAt(sameCommandIndex);
				I_ASSERT(element.pointer != NULL);

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


// reimplemented (idoc::ICommand)

int CHierarchicalCommand::GetPriority() const
{
	return m_priority;
}


int CHierarchicalCommand::GetStaticFlags() const
{
	return m_staticFlags;
}


bool CHierarchicalCommand::Execute(istd::IPolymorphic* /*contextPtr*/)
{
	if (IsEnabled()){
		emit trigger();
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


idoc::ICommand* CHierarchicalCommand::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_childs.GetCount());

	return m_childs.GetAt(index);
}


// reimplemented (istd::INamed)

void CHierarchicalCommand::SetName(const istd::CString& name)
{
	BaseClass2::SetName(name);
	setText(iqt::GetQString(name));
}


// protected methods

int CHierarchicalCommand::FindTheSameCommand(const idoc::IHierarchicalCommand& command) const
{
	const istd::CString& elementName = command.GetName();

	int childsCount = m_childs.GetCount();
	for (int i = 0; i < childsCount; ++i){
		CHierarchicalCommand* childPtr = m_childs.GetAt(i);
		I_ASSERT(childPtr != NULL);

		if (childPtr->GetName().IsEqual(elementName, istd::CString::CF_CASE_INSENSITIVE)){
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
		I_ASSERT(childPtr != NULL);

		if (priority < childPtr->GetPriority()){
			return i;
		}
	}

	return childsCount;
}


} // namespace iqt


