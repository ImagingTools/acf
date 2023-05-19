#ifndef iqtgui_CCommandTools_included
#define iqtgui_CCommandTools_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#endif

#if QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#else
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#endif

// ACF includes
#include <ibase/ICommandsProvider.h>

#include <iqtgui/CHierarchicalCommand.h>


namespace iqtgui
{


/**
	Helper class to manage menu and toolbar using CHierarchicalCommand.
*/
class CCommandTools
{
public:
	/**
		Create menu according to the given commands.
	*/
	template <class MenuType>
	static void CreateMenu(const iqtgui::CHierarchicalCommand& command, MenuType& result);

	/**
		Fill a toolbar with the commands.
	*/
	static int SetupToolbar(
				const iqtgui::CHierarchicalCommand& command,
				QToolBar& result,
				int prevGroupId = ibase::ICommand::GI_NONE);

	/**
		Fill a context menu of a widget with the commands.
	*/
	static int SetupContextMenu(
				const iqtgui::CHierarchicalCommand& command,
				QWidget& menuOwner,
				int prevGroupId = ibase::ICommand::GI_NONE);
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


