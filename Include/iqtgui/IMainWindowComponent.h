#ifndef iqtgui_IMainWindowComponent_included
#define iqtgui_IMainWindowComponent_included


// ACF includes
#include "iqtgui/iqtgui.h"
#include "istd/IPolymorphic.h"


class QMainWindow;


namespace iqtgui
{


/**
	A common interface for main window component such as dock widget, tool bar and so on.
*/
class IMainWindowComponent: virtual public istd::IPolymorphic
{
public:
	/**
		Flags describing behaviour of the main window component.
	*/
	enum WindowComponentFlags
	{
		WCF_NONE = 0,

		/**
			Element can be closed.
		*/
		WCF_CLOSABLE = 1,
		/**
			Element can be moved.
		*/
		WCF_MOVEABLE = 2,
		/**
			Element can be float from the main window to be standalone window.
		*/
		WCF_FLOATABLE = 4
	};

	/**
		Add this component to main window.
	*/
	virtual bool AddToMainWindow(QMainWindow& mainWindow) = 0;

	/**
		Remove this component from main window.
	*/
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow) = 0;

	/**
		Get the title of the main window component.
	*/
	virtual QString GetTitle() const = 0;

	/**
		Get properties of of the window component.
		\sa WindowComponentFlags
	*/
	virtual int GetFlags() const = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IMainWindowComponent_included


