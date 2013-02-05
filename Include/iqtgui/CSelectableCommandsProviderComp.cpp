#include "iqtgui/CSelectableCommandsProviderComp.h"


// ACF includes
#include "iprm/IOptionsList.h"


namespace iqtgui
{


// public methods

CSelectableCommandsProviderComp::CSelectableCommandsProviderComp()
	:m_commandsList("", 100, ibase::ICommand::CF_GLOBAL_MENU)
{
}


// reimpemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CSelectableCommandsProviderComp::GetCommands() const
{
	return &m_rootMenuCommand;
}


// reimpemented (icomp::IComponent)

void CSelectableCommandsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_commandSelectionModelCompPtr.IsValid()){
		m_commandSelectionModelCompPtr->AttachObserver(this);
	}

	m_mainMenuCommand.SetName(*m_rootMenuNameAttrPtr);
	m_commandsList.SetVisuals(*m_menuNameAttrPtr, *m_menuNameAttrPtr, *m_menuDescriptionAttrPtr);

	m_rootMenuCommand.InsertChild(&m_mainMenuCommand);
	m_mainMenuCommand.InsertChild(&m_commandsList);
}


void CSelectableCommandsProviderComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// protected methods

// reimpemented (imod::CSingleModelObserverBase)

void CSelectableCommandsProviderComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	Q_ASSERT(selectionPtr != NULL);

	istd::CChangeNotifier changePtr(this, CF_COMMANDS | CF_MODEL);

	m_commandsList.ResetChilds();
	m_commandsList.setVisible(false);

	int currentSelection = selectionPtr->GetSelectedOptionIndex();

	const iprm::IOptionsList* selectionConstrainsPtr = selectionPtr->GetSelectionConstraints();
	if (selectionConstrainsPtr != NULL){
		int optionsCount = selectionConstrainsPtr->GetOptionsCount();
		int indexOffset = m_noSelectionCommandAttrPtr.IsValid() ? 1 : 0;

		int commandFlags = 
			ibase::ICommand::CF_GLOBAL_MENU | 
			ibase::ICommand::CF_EXCLUSIVE | 
			ibase::ICommand::CF_ONOFF;
		if (optionsCount > 0){
			m_commandsList.setVisible(true);

			if (m_noSelectionCommandAttrPtr.IsValid())
			{
				iqtgui::CHierarchicalCommand* resetAoiCommandPtr = new iqtgui::CHierarchicalCommand(
					*m_noSelectionCommandAttrPtr,
					100,
					commandFlags);

				connect(resetAoiCommandPtr, SIGNAL(triggered()), this, SLOT(OnCommandActivated()));
				resetAoiCommandPtr->setData(iprm::ISelectionParam::NO_SELECTION);
				resetAoiCommandPtr->setChecked(currentSelection == -1);

				m_commandsList.InsertChild(resetAoiCommandPtr, true, 0);
			}
		}

		for (int optionIndex = 0; optionIndex < optionsCount; optionIndex++){
			QString commandName = selectionConstrainsPtr->GetOptionName(optionIndex);

			iqtgui::CHierarchicalCommand* aoiCommandPtr = new iqtgui::CHierarchicalCommand(
				commandName,
				100,
				commandFlags);

			aoiCommandPtr->setData(optionIndex);

			connect(aoiCommandPtr, SIGNAL(triggered()), this, SLOT(OnCommandActivated()));

			aoiCommandPtr->setChecked(currentSelection == optionIndex);
			aoiCommandPtr->setCheckable(true);

			m_commandsList.InsertChild(aoiCommandPtr, true, optionIndex + indexOffset);				
		}

		if (m_actionIconsProviderCompPtr.IsValid()){
			int aoiCommandsCount = qMin(m_commandsList.GetChildsCount(), m_actionIconsProviderCompPtr->GetIconCount());
			for (int commandIndex = 0; commandIndex < aoiCommandsCount; commandIndex++){
				iqtgui::CHierarchicalCommand* commandPtr = dynamic_cast<iqtgui::CHierarchicalCommand*>(m_commandsList.GetChild(commandIndex));
				Q_ASSERT(commandPtr != NULL);

				commandPtr->setIcon(m_actionIconsProviderCompPtr->GetIcon(commandIndex));
			}
		}
	}
}


// private slots

void CSelectableCommandsProviderComp::OnCommandActivated()
{
	QAction* actionPtr = dynamic_cast<QAction*>(sender());
	Q_ASSERT(actionPtr != NULL);

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	Q_ASSERT(selectionPtr != NULL);

	int optionIndex = actionPtr->data().toInt();
	Q_ASSERT(optionIndex < selectionPtr->GetSelectionConstraints()->GetOptionsCount());

	selectionPtr->SetSelectedOptionIndex(optionIndex);
}


} // namespace iqtgui


