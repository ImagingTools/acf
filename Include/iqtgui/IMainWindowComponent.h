#ifndef iqtgui_IMainWindowComponent_included
#define iqtgui_IMainWindowComponent_included


#include "iqtgui/iqtgui.h"


#include "istd/IPolymorphic.h"


class QMainWindow;


namespace iqtgui
{


/**
	A common interface for main window component such as dock widget,tool bar and so on.
*/
class IMainWindowComponent: virtual public istd::IPolymorphic
{
public:
	/**
		Add this component to main window.
	*/
	virtual bool AddToMainWindow(QMainWindow& mainWindow) = 0;
	/**
		Remove this component from main window.
	*/
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IMainWindowComponent_included

