#pragma once


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <ibase/ICommandsProvider.h>
#include <ibase/ICommand.h>
#include <icomp/CComponentBase.h>
#include <imod/CSingleModelObserverBase.h>
#include <idoc/IUndoManager.h>
#include <imod/IModel.h>
#include <iqtgui/TMakeIconProviderCompWrap.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace iqtdoc
{


class CUndoManagerCommandsProviderComp:
			public QObject,
			public iqtgui::TMakeIconProviderCompWrap<icomp::CComponentBase>,
			protected imod::CSingleModelObserverBase,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TMakeIconProviderCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CUndoManagerCommandsProviderComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_undoManagerCompPtr, "UndoManager", "Undo/Redo-manager", true, "UndoManager");
		I_ASSIGN_TO(m_undoManagerModelCompPtr, m_undoManagerCompPtr, true);
	I_END_COMPONENT;

	CUndoManagerCommandsProviderComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const override;

protected:
	// reimplemented from (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private Q_SLOTS:
	void OnUndoCommand();
	void OnRedoCommand();

private:
	I_REF(idoc::IUndoManager, m_undoManagerCompPtr);
	I_REF(imod::IModel, m_undoManagerModelCompPtr);

	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_editCommands;
	iqtgui::CHierarchicalCommand m_undoCommand;
	iqtgui::CHierarchicalCommand m_redoCommand;
};


} // namespace iqtdoc


