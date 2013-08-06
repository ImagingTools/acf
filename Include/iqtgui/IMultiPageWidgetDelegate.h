#ifndef iqtgui_IMultiPageWidgetDelegate_included
#define iqtgui_IMultiPageWidgetDelegate_included


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QSize>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <istd/IPolymorphic.h>


namespace iqtgui
{


/**
	Interface of a delegate object used for operating on concrete UI-container implementation.
	The delegate itself is no widget, but it provides all necessary management functionalities for a page container widget.
	\sa CTabWidgetDelegate, CSplitterDelegate, CToolBoxDelegate and CSimpleGuiContainerDelegate
*/
class IMultiPageWidgetDelegate: virtual public istd::IPolymorphic
{
public:
	/**
		Enum for describing of page header bar position on the container widget.
	*/
	enum PageHeaderPosition
	{
		/**
			Default page header position (decided by container implementation)
		*/
		PHP_AUTO,

		/**
			The page header will be placed on the left side of the container if possible.
		*/
		PHP_LEFT,

		/**
			The page header will be placed on the top side of the container if possible.
		*/
		PHP_TOP,

		/**
			The page header will be placed on the right side of the container if possible.
		*/
		PHP_RIGHT,

		/**
			The page header will be placed on the bottom side of the container if possible.
		*/
		PHP_BOTTOM
	};

	/**
		Create container for the page widgets.
	*/
	virtual QWidget* CreateContainerWidget(QWidget* parentWidgetPtr, int orientation = Qt::Horizontal) = 0;

	/**
		Connect object, which will be notified about changing of the current selected page index.
		\param containerWidget	Container widget which provides initiate switching of the page
		\param receiverPtr	Receiver of the page changing notification
		\param receiverSlotName	Name of the slot implemented in the receiver implementation, which will be triggered on page switching.
	*/
	virtual bool ConnectPageIndexListener(
				QWidget& containerWidget,
				QObject* receiverPtr,
				const char* receiverSlotName) = 0;

	/**
		Set the position of the page header bar on the container UI.
	*/
	virtual bool SetPageHeaderPosition(QWidget& containerWidget, PageHeaderPosition pageHeaderPosition) = 0;

	/**
		Add a new page to the container. If \c pageIndex is negative, the new page will be inserted after the last page.
		The container overtakes the control about \c widgetPtr.
		\return Position of the inserted page.
	*/
	virtual int InsertPage(
				QWidget& containerWidget,
				QWidget* pageWidgetPtr,
				const QString& pageTitle,
				int pageIndex = -1) = 0;

	/**
		Remove the page from the container. The page widget will be destroyed.
	*/
	virtual void RemovePage(QWidget& containerWidget, int pageIndex) = 0;

	/**
		Get the number of the pages in the container.
	*/
	virtual int GetPagesCount(const QWidget& containerWidget) const = 0; 

	/**
		Get page's widget from the container.
	*/
	virtual QWidget* GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const = 0; 

	/**
		Get currently active page in the container.
	*/
	virtual int GetCurrentPage(const QWidget& containerWidget) const = 0;

	/**
		Set active page in the container.
	*/
	virtual bool SetCurrentPage(QWidget& containerWidget, int pageIndex) = 0;

	/**
		Get the title of the given page.
	*/
	virtual QString GetPageTitle(const QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set the page title.
	*/
	virtual void SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle) = 0;

	/**
		Get the page icon.
	*/
	virtual QIcon GetPageIcon(const QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set the page icon.
	*/
	virtual void SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon) = 0;

	/**
		Get the page tool tip.
	*/
	virtual QString GetPageToolTip(const QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set the page tool tip.
	*/
	virtual void SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip) = 0;

	/**
		Get if the page enabled/disabled.
	*/
	virtual bool IsPageEnabled(const QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set page enabled/disabled.
	*/
	virtual bool SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled = true) = 0;

	/**
		Get if the page visible/hidden.
	*/
	virtual bool IsPageVisible(const QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set page visible/hidden.
	*/
	virtual bool SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible = true) = 0;

	/**
		Get the size page icon.
	*/
	virtual QSize GetPageIconSize(const QWidget& containerWidget) const = 0;

	/**
		Set the size of the icon for all pages in the container.
	*/
	virtual bool SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IMultiPageWidgetDelegate_included


