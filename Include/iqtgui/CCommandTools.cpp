#include <iqtgui/CCommandTools.h>


namespace iqtgui
{


// public static methods

int CCommandTools::SetupToolbar(
			const iqtgui::CHierarchicalCommand& command,
			QToolBar& result,
			int prevGroupId,
			const QVector<int>& includedGroups,
			const QVector<int>& excludedGroups)
{
	int childsCount = command.GetChildsCount();

	QMap<int, QActionGroup*> groups;

	for (int i = 0; i < childsCount; ++i){
		iqtgui::CHierarchicalCommand* hierarchicalPtr = const_cast<iqtgui::CHierarchicalCommand*>(
					dynamic_cast<const iqtgui::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
			int groupId = hierarchicalPtr->GetGroupId();
			int flags = hierarchicalPtr->GetStaticFlags();

			if ((flags & ibase::ICommand::CF_TOOLBAR) != 0){
				if (hierarchicalPtr->GetChildsCount() > 0){
					QMenu* newMenuPtr = new QMenu(&result);
					if (newMenuPtr != NULL){
						newMenuPtr->setTitle(hierarchicalPtr->GetName());

						CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

						newMenuPtr->setIcon(hierarchicalPtr->icon());
						result.addAction(newMenuPtr->menuAction());
					}
				}
				else{
					if (IsCommandIncluded(*hierarchicalPtr, includedGroups, excludedGroups)){
						if ((groupId != prevGroupId) && (prevGroupId != ibase::ICommand::GI_NONE)){
							result.addSeparator();
						}

						QString actionName = hierarchicalPtr->GetName();

						result.addAction(hierarchicalPtr);

						if ((flags & ibase::ICommand::CF_EXCLUSIVE) != 0){
							QActionGroup*& groupPtr = groups[hierarchicalPtr->GetGroupId()];
							if (groupPtr == NULL){
								groupPtr = new QActionGroup(&result);
								groupPtr->setExclusive(true);
							}

							groupPtr->addAction(hierarchicalPtr);
							hierarchicalPtr->setCheckable(true);
						}
					}
				}

				if (groupId != ibase::ICommand::GI_NONE){
					prevGroupId = groupId;
				}
			}
			else if (hierarchicalPtr->GetChildsCount() > 0){
				prevGroupId = SetupToolbar(*hierarchicalPtr, result, prevGroupId, includedGroups, excludedGroups);
			}
		}
	}

	return prevGroupId;
}


int CCommandTools::SetupContextMenu(
			const iqtgui::CHierarchicalCommand& command,
			QWidget& menuOwner,
			int prevGroupId,
			const QVector<int>& includedGroups,
			const QVector<int>& excludedGroups)
{
	QMap<int, QActionGroup*> groups;

	int childsCount = command.GetChildsCount();

	menuOwner.setContextMenuPolicy(Qt::ActionsContextMenu);

	for (int i = 0; i < childsCount; ++i){
		iqtgui::CHierarchicalCommand* hierarchicalPtr = const_cast<iqtgui::CHierarchicalCommand*>(
					dynamic_cast<const iqtgui::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
			int groupId = hierarchicalPtr->GetGroupId();
			int flags = hierarchicalPtr->GetStaticFlags();

			if ((flags & ibase::ICommand::CF_CONTEXT_MENU) != 0){
				if (hierarchicalPtr->GetChildsCount() > 0){
					QMenu* newMenuPtr = new QMenu(&menuOwner);
					if (newMenuPtr != NULL){
						newMenuPtr->setTitle(hierarchicalPtr->GetName());

						CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

						newMenuPtr->setIcon(hierarchicalPtr->icon());
						menuOwner.addAction(newMenuPtr->menuAction());
					}
				}
				else{
					if (IsCommandIncluded(*hierarchicalPtr, includedGroups, excludedGroups)){
						if ((groupId != prevGroupId) && (prevGroupId != ibase::ICommand::GI_NONE)){
							QAction* separator = new QAction(&menuOwner);
							separator->setSeparator(true);
							menuOwner.addAction(separator);
						}

						QString actionName = hierarchicalPtr->GetName();

						menuOwner.addAction(hierarchicalPtr);

						if ((flags & ibase::ICommand::CF_EXCLUSIVE) != 0){
							QActionGroup*& groupPtr = groups[hierarchicalPtr->GetGroupId()];
							if (groupPtr == NULL){
								groupPtr = new QActionGroup(&menuOwner);
								groupPtr->setExclusive(true);
							}

							groupPtr->addAction(hierarchicalPtr);
							hierarchicalPtr->setCheckable(true);
						}
					}
				}

				if (groupId != ibase::ICommand::GI_NONE){
					prevGroupId = groupId;
				}
			}
			else if (hierarchicalPtr->GetChildsCount() > 0){
				prevGroupId = SetupContextMenu(*hierarchicalPtr, menuOwner, prevGroupId, includedGroups, excludedGroups);
			}
		}
	}

	return prevGroupId;
}


// private methods

bool CCommandTools::IsCommandIncluded(
			const iqtgui::CHierarchicalCommand& command,
			const QVector<int>& includedGroups,
			const QVector<int>& excludedGroups)
{
	bool retVal = true;

	int groupId = command.GetGroupId();

	if (!includedGroups.isEmpty() && !includedGroups.contains(groupId)){
		retVal = false;
	}

	if (!excludedGroups.isEmpty() && excludedGroups.contains(groupId)){
		retVal = false;
	}

	return retVal;
}


} // namespace iqtgui


