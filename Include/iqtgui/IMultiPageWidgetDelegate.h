#ifndef iqtgui_IMultiPageWidgetDelegate_included
#define iqtgui_IMultiPageWidgetDelegate_included


// Qt includes
#include <QtCore/QSize>
#include <QtGui/QWidget>

// ACF includes
#include <istd/IPolymorphic.h>


namespace iqtgui
{


class IMultiPageWidgetDelegate: virtual public istd::IPolymorphic
{
public:
	/**
		Create container widget.
	*/
	virtual QWidget* CreateContainerWidget(QWidget* parentWidgetPtr) = 0;

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
	virtual int GetCurrentPage(QWidget& containerWidget) const = 0;

	/**
		Set active page in the container.
	*/
	virtual bool SetCurrentPage(QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Get the title of the given page.
	*/
	virtual QString GetPageTitle(QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set the page title.
	*/
	virtual void SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle) const = 0;

	/**
		Get the page icon.
	*/
	virtual QIcon GetPageIcon(QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set the page icon.
	*/
	virtual void SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon) const = 0;

	/**
		Get the page tool tip.
	*/
	virtual QString GetPageToolTip(QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set the page tool tip.
	*/
	virtual void SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip) const = 0;

	/**
		Get if the page enabled/disabled.
	*/
	virtual bool IsPageEnabled(QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set page enabled/disabled.
	*/
	virtual bool SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled = true) const = 0;

	/**
		Get if the page visible/hidden.
	*/
	virtual bool IsPageVisible(QWidget& containerWidget, int pageIndex) const = 0;

	/**
		Set page visible/hidden.
	*/
	virtual bool SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible = true) const = 0;

	/**
		Get the size page icon.
	*/
	virtual QSize GetPageIconSize(QWidget& containerWidget) const = 0;

	/**
		Set the size page icon.
	*/
	virtual bool SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize) const = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IMultiPageWidgetDelegate_included


