#ifndef iqtgui_CTabWidgetDelegate_included
#define iqtgui_CTabWidgetDelegate_included


// ACF includes
#include <iqtgui/IMultiPageWidgetDelegate.h>


namespace iqtgui
{


/**
	Implementation of the IMultiPageWidgetDelegate interface for QTabWidget.
*/
class CTabWidgetDelegate: virtual public IMultiPageWidgetDelegate
{
public:
	// reimplemented (IMultiPageWidgetDelegate)
	virtual QWidget* CreateContainerWidget(QWidget* parentWidgetPtr);
	virtual int InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex = -1);
	virtual void RemovePage(QWidget& containerWidget, int pageIndex);
	virtual int GetPagesCount(const QWidget& containerWidget) const; 
	virtual QWidget* GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const; 
	virtual int GetCurrentPage(QWidget& containerWidget) const;
	virtual bool SetCurrentPage(QWidget& containerWidget, int pageIndex) const;
	virtual QString GetPageTitle(QWidget& containerWidget, int pageIndex) const;
	virtual void SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle) const;
	virtual QIcon GetPageIcon(QWidget& containerWidget, int pageIndex) const;
	virtual void SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon) const;
	virtual QString GetPageToolTip(QWidget& containerWidget, int pageIndex) const;
	virtual void SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip) const;
	virtual bool IsPageEnabled(QWidget& containerWidget, int pageIndex) const;
	virtual bool SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled = true) const;
	virtual bool IsPageVisible(QWidget& containerWidget, int pageIndex) const;
	virtual bool SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible = true) const;
	virtual QSize GetPageIconSize(QWidget& containerWidget) const;
	virtual bool SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize) const;
};


} // namespace iqtgui


#endif // !iqtgui_CTabWidgetDelegate_included


