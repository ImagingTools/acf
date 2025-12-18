#include <iqtdoc/CUndoManagerCommandsProviderComp.h>


namespace iqtdoc
{


// public methods

CUndoManagerCommandsProviderComp::CUndoManagerCommandsProviderComp()
	:m_undoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	 m_redoCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR)
{
	m_rootCommands.InsertChild(&m_editCommands, false);
	m_editCommands.InsertChild(&m_undoCommand, false);
	m_editCommands.InsertChild(&m_redoCommand, false);

	connect(&m_undoCommand, SIGNAL(triggered()), this, SLOT(OnUndoCommand()));
	connect(&m_redoCommand, SIGNAL(triggered()), this, SLOT(OnRedoCommand()));

	m_undoCommand.SetVisuals(tr("&Undo"), tr("Undo"), tr("Undo last document changes"), GetIcon(":/Icons/Undo"));
	m_redoCommand.SetVisuals(tr("&Redo"), tr("Redo"), tr("Redo last document changes"), GetIcon(":/Icons/Redo"));

	m_undoCommand.setShortcut(Qt::CTRL + Qt::Key_Z);
	m_redoCommand.setShortcut(int(Qt::CTRL) + int(Qt::SHIFT) + Qt::Key_Z);

	m_undoCommand.setVisible(false);
	m_redoCommand.setVisible(false);

}


const ibase::IHierarchicalCommand* CUndoManagerCommandsProviderComp::GetCommands() const
{
	return &m_rootCommands;
}


// private slots

void CUndoManagerCommandsProviderComp::OnUndoCommand()
{
	if (m_undoManagerModelCompPtr.IsValid() && m_undoManagerCompPtr.IsValid()){
		m_undoManagerCompPtr->DoUndo();
	}
}


void CUndoManagerCommandsProviderComp::OnRedoCommand()
{
	if (m_undoManagerModelCompPtr.IsValid() && m_undoManagerCompPtr.IsValid()){
		m_undoManagerCompPtr->DoRedo();
	}
}


//protected methods

// reimplemented from (imod::CSingleModelObserverBase)

void CUndoManagerCommandsProviderComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (m_undoManagerCompPtr.IsValid() && m_undoManagerModelCompPtr.IsValid()){
		const bool undoPossible = (m_undoManagerCompPtr->GetAvailableUndoSteps() > 0);
		const bool redoPossible = (m_undoManagerCompPtr->GetAvailableRedoSteps() > 0);
		
		m_undoCommand.SetEnabled(undoPossible);
		m_redoCommand.SetEnabled(redoPossible);
	}
}


// reimplemented icomp::CComponentBase

void CUndoManagerCommandsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_undoManagerModelCompPtr.IsValid()){
		if (m_undoManagerModelCompPtr->AttachObserver(this)){
			m_undoCommand.setVisible(true);
			m_redoCommand.setVisible(true);
		}
	}
}


void CUndoManagerCommandsProviderComp::OnComponentDestroyed()
{
	if (m_undoManagerModelCompPtr.IsValid()){
		m_undoManagerModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iqtdoc


