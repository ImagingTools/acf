#ifndef iqtgui_CSplitterDelegate_included
#define iqtgui_CSplitterDelegate_included


// ACF includes
#include <iqtgui/IMultiPageWidgetDelegate.h>


namespace iqtgui
{


/**
	Implementation of the IMultiPageWidgetDelegate interface for QSplitter.
*/
class CSplitterDelegate: virtual public IMultiPageWidgetDelegate
{
public:
	// reimplemented (IMultiPageWidgetDelegate)
	virtual QWidget* CreateContainerWidget(QWidget* parentWidgetPtr, int orientation = Qt::Horizontal);
	virtual bool SetPageHeaderPosition(QWidget& containerWidget, PageHeaderPosition pageHeaderPosition);
	virtual int InsertPage(
		QWidget& containerWidget,
		QWidget* pageWidgetPtr,
		const QString& pageTitle,
		int pageIndex = -1);
	virtual void RemovePage(QWidget& containerWidget, int pageIndex);
	virtual int GetPagesCount(const QWidget& containerWidget) const; 
	virtual QWidget* GetPageWidgetPtr(const QWidget& containerWidget, int pageIndex) const; 
	virtual int GetCurrentPage(const QWidget& containerWidget) const;
	virtual bool SetCurrentPage(QWidget& containerWidget, int pageIndex);
	virtual QString GetPageTitle(const QWidget& containerWidget, int pageIndex) const;
	virtual void SetPageTitle(QWidget& containerWidget, int pageIndex, const QString& pageTitle);
	virtual QIcon GetPageIcon(const QWidget& containerWidget, int pageIndex) const;
	virtual void SetPageIcon(QWidget& containerWidget, int pageIndex, const QIcon& pageIcon);
	virtual QString GetPageToolTip(const QWidget& containerWidget, int pageIndex) const;
	virtual void SetPageToolTip(QWidget& containerWidget, int pageIndex, const QString& pageToolTip);
	virtual bool IsPageEnabled(const QWidget& containerWidget, int pageIndex) const;
	virtual bool SetPageEnabled(QWidget& containerWidget, int pageIndex, bool isPageEnabled = true);
	virtual bool IsPageVisible(const QWidget& containerWidget, int pageIndex) const;
	virtual bool SetPageVisible(QWidget& containerWidget, int pageIndex, bool isPageVisible = true);
	virtual QSize GetPageIconSize(const QWidget& containerWidget) const;
	virtual bool SetPageIconSize(QWidget& containerWidget, const QSize& pageIconSize);

private:
	QSize m_iconSize;
};


} // namespace iqtgui


#endif // !iqtgui_CSplitterDelegate_included


