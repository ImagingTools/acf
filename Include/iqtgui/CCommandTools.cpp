#include <iqtgui/CCommandTools.h>


namespace iqtgui
{


// public static methods

int CCommandTools::SetupToolbar(const iqtgui::CHierarchicalCommand& command, QToolBar& result, int prevGroupId)
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

				if (groupId != ibase::ICommand::GI_NONE){
					prevGroupId = groupId;
				}
			}
			else if (hierarchicalPtr->GetChildsCount() > 0){
				prevGroupId = SetupToolbar(*hierarchicalPtr, result, prevGroupId);
			}
		}
	}

	return prevGroupId;
}


} // namespace iqtgui


