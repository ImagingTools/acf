#include "iqtproc/CProcessorCommandComp.h"


// ACF includes
#include "iprm/ISelectionConstraints.h"


namespace iqtproc
{


// public methods

CProcessorCommandComp::CProcessorCommandComp()
	:m_processingCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR)
{
}


// reimpemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CProcessorCommandComp::GetCommands() const
{
	return &m_rootMenuCommand;
}


// reimpemented (icomp::IComponent)

void CProcessorCommandComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_mainMenuCommand.SetName(*m_rootMenuNameAttrPtr);
	m_processingCommand.SetVisuals(*m_menuNameAttrPtr, *m_menuNameAttrPtr, *m_menuDescriptionAttrPtr, QIcon(*m_actionIconAttrPtr));

	m_rootMenuCommand.InsertChild(&m_mainMenuCommand);
	m_mainMenuCommand.InsertChild(&m_processingCommand);

	m_processingCommand.setVisible(m_processorCompPtr.IsValid());

	connect(&m_processingCommand, SIGNAL(triggered()), this, SLOT(OnCommandActivated()));
}


// protected methods

// private slots

void CProcessorCommandComp::OnCommandActivated()
{
	I_ASSERT(m_processorCompPtr.IsValid());

	m_processorCompPtr->DoProcessing(m_processorParamsCompPtr.GetPtr(), m_processorInputCompPtr.GetPtr(), m_processorOutputCompPtr.GetPtr());
}


} // namespace iqtproc


