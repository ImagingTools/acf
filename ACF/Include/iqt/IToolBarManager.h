#ifndef iqt_IToolBarManager_included
#define iqt_IToolBarManager_included


#include "iqt/iqt.h"


#include "istd/IPolymorphic.h"


class QWidget;
class QToolBar;


namespace iqt
{


/**
	Common interface for a toolbar manager.
*/
class IToolBarManager: virtual public istd::IPolymorphic
{
public:
	/**
		Set visibility of all tool bars to \s isVisible
	*/
	virtual void SetToolBarsVisible(bool isVisible = true) = 0;

	/**
		Get number of toolbars.
	*/
	virtual int GetToolBarCount() const = 0;

	/**
		Add a toolbar.
	*/
	virtual	void AddToolBar(QToolBar* widgetPtr) = 0;

	/**
		Remove the given toolbar.
	*/
	virtual void RemoveToolBar(QToolBar* widgetPtr) = 0;
};


} // namespace iqt


#endif // !iqt_IToolBarManager_h_included

