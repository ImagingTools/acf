#ifndef iqtgui_CCommandTools_included
#define iqtgui_CCommandTools_included


// Qt includes
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

// ACF includes
#include "ibase/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"


namespace iqtgui
{


/**
	Helper class to manage menu and toolbar using CHierarchicalCommand.
*/
class CCommandTools
{
public:
	template <class MenuType>
	static void CreateMenu(const iqtgui::CHierarchicalCommand& command, MenuType& result);

	static int SetupToolbar(const iqtgui::CHierarchicalCommand& command, QToolBar& result, int prevGroupId = ibase::ICommand::GI_NONE);
};


// protected template methods

template <class MenuType>
void CCommandTools::CreateMenu(const iqtgui::CHierarchicalCommand& command, MenuType& result)
{
	int prevGroupId = ibase::ICommand::GI_NONE;

	int childsCount = command.GetChildsCount();

	QMap<int, QActionGroup*> groups;

	for (int i = 0; i < childsCount; ++i){
		QString text = command.text();
		iqtgui::CHierarchicalCommand* hierarchicalPtr = const_cast<iqtgui::CHierarchicalCommand*>(
					dynamic_cast<const iqtgui::CHierarchicalCommand*>(command.GetChild(i)));

		if (hierarchicalPtr != NULL){
			QString text2 = hierarchicalPtr->text();
			int groupId = hierarchicalPtr->GetGroupId();
			int flags = hierarchicalPtr->GetStaticFlags();

			if ((groupId != prevGroupId) && (prevGroupId != ibase::ICommand::GI_NONE)){
				result.addSeparator();
			}

			if (groupId != ibase::ICommand::GI_NONE){
				prevGroupId = groupId;
			}

			if (hierarchicalPtr->GetChildsCount() > 0){
				QMenu* newMenuPtr = new QMenu(&result);
				newMenuPtr->setTitle(hierarchicalPtr->GetName());

				CreateMenu<QMenu>(*hierarchicalPtr, *newMenuPtr);

				result.addMenu(newMenuPtr);
			}
			else if ((flags & ibase::ICommand::CF_GLOBAL_MENU) != 0){
				if ((flags & ibase::ICommand::CF_EXCLUSIVE) != 0){
					QActionGroup*& groupPtr = groups[hierarchicalPtr->GetGroupId()];
					if (groupPtr == NULL){
						groupPtr = new QActionGroup(&result);
						groupPtr->setExclusive(true);
					}

					groupPtr->addAction(hierarchicalPtr);
					hierarchicalPtr->setCheckable(true);
				}

				result.addAction(hierarchicalPtr);
			}
		}
	}
}


} // namespace iqtgui


#endif // !iqtgui_CCommandTools_included


