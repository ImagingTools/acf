#ifndef iqtgui_CMultiPageWidget_included
#define iqtgui_CMultiPageWidget_included


// Qt includes
#include <QtCore/QSize>
#include <QtCore/QMap>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

// ACF includes
#include <istd/TSmartPtr.h>
#include <istd/TDelPtr.h>
#include <iqtgui/IMultiPageWidgetDelegate.h>


namespace iqtgui
{


/**
	Implementation of the widget, which is represented as a set of pages.
	According to a selected UI presentation mode, it can be a tab widget, stacked layout, tool box, or simple a set of lay-outed widgets.
*/
class CMultiPageWidget: public QWidget
{
public:
	typedef QWidget BaseClass;
	typedef IMultiPageWidgetDelegate::PageHeaderPosition PageHeaderPosition; 

	enum DesignType
	{
		/**
			The pages will be placed simple beside each other.
		*/
		DT_SIMPLE,

		/**
			The pages are grouped in a tool box.
		*/
		DT_TOOL_BOX,

		/**
			The pages are grouped in a tab widget.
		*/
		DT_TAB_WIDGET,

		/**
			The pages are grouped in a stack layout.
		*/
		DT_STACK,

		/**
			The same as DT_SIMPLE but a splitter manager is used for lay outing of the pages.
		*/
		DT_SPLITTER,

		/**
			User defined container implementation.
		*/
		DT_USER = 1024
	};

	/**
		Construct the multi page widget.
		\param designMode Specify the type of the underlying page container widget. \sa DesignType
		\param useHorizontalLayout	If set, the container will try
	*/
	CMultiPageWidget(
		QWidget* parentWidgetPtr = NULL,
		int designMode = DT_SIMPLE,
		Qt::Orientation orientation = Qt::Vertical);

	/**
		Get page container widget.
	*/
	virtual QWidget* GetContainerWidgetPtr() const;

	/**
		Set design mode. Existing pages will be re-integrated into the new container widget.
	*/
	void SetDesignMode(int designMode);

	/**
		Remove all pages from the container widget.
	*/
	virtual void ResetPages();

	/**
		Set the position of the page header on the container UI.
	*/
	virtual void SetPageHeaderPosition(PageHeaderPosition pageHeaderPosition);

	/**
		Add a new page to the container. If \c pageIndex is negative, the new page will be inserted after the last page.
		The container overtakes the control about \c widgetPtr.
		\return Position of the inserted page.
	*/
	virtual int InsertPage(
				QWidget* pageWidgetPtr,
				const QString& pageTitle,
				int pageIndex = -1);

	/**
		Remove the page from the container. The page widget will be destroyed.
	*/
	virtual void RemovePage(int pageIndex);

	/**
		Get the number of the pages in the container.
	*/
	virtual int GetPagesCount() const; 

	/**
		Get page's widget from the container.
	*/
	virtual QWidget* GetPageWidgetPtr(int pageIndex) const; 

	/**
		Get currently active page in the container.
	*/
	virtual int GetCurrentPage() const;

	/**
		Set active page in the container.
	*/
	virtual bool SetCurrentPage(int pageIndex);

	/**
		Get the title of the given page.
	*/
	virtual QString GetPageTitle(int pageIndex) const;

	/**
		Set the page title.
	*/
	virtual void SetPageTitle(int pageIndex, const QString& pageTitle);

	/**
		Get the page icon.
	*/
	virtual QIcon GetPageIcon(int pageIndex) const;

	/**
		Set the page icon.
	*/
	virtual void SetPageIcon(int pageIndex, const QIcon& pageIcon);

	/**
		Get the page tool tip.
	*/
	virtual QString GetPageToolTip(int pageIndex) const;

	/**
		Set the page tool tip.
	*/
	virtual void SetPageToolTip(int pageIndex, const QString& pageToolTip);

	/**
		Get if the page enabled/disabled.
	*/
	virtual bool IsPageEnabled(int pageIndex) const;

	/**
		Set page enabled/disabled.
	*/
	virtual bool SetPageEnabled(int pageIndex, bool isPageEnabled = true);

	/**
		Get if the page visible/hidden.
	*/
	virtual bool IsPageVisible(int pageIndex) const;

	/**
		Set page visible/hidden.
	*/
	virtual bool SetPageVisible(int pageIndex, bool isPageVisible = true);

	/**
		Get the size page icon.
	*/
	virtual QSize GetPageIconSize() const;

	/**
		Set the size of the icon for all pages in the container.
	*/
	virtual bool SetPageIconSize(const QSize& pageIconSize);

	/**
		Register your own page widget delegate objects for the given UI mode.
	*/
	template<class DelegateImpl>
	void RegisterMultiPageWidgetDelegate(int uiMode);

protected:
	/**
		Get the index of the currently active page in the container widget.
	*/
	int GetCurrentPageIndex() const;

private:
	typedef istd::TSmartPtr<IMultiPageWidgetDelegate> MultiPageWidgetDelegatePtr;

	/**
		Create container GUI in some parent widget.
	*/
	bool CreateContainerGui();

	/**
		Get container delegate for the currently selected UI mode.
	*/
	MultiPageWidgetDelegatePtr GetCurrentDelegate() const;

private:
	typedef QMap<int, MultiPageWidgetDelegatePtr> ContainerWidgetDelegateMap;
		
	ContainerWidgetDelegateMap m_containerWidgetDelegateMap;

	/**
		A container that depends on \c DesignType, i.e. QWidget, QToolBox or QTabWidget
	*/
	istd::TDelPtr<QWidget> m_guiContainerPtr;

	int m_designMode;

	Qt::Orientation m_orientation;

	QSize m_pageIconSize;
};


template<class DelegateImpl>
void CMultiPageWidget::RegisterMultiPageWidgetDelegate(int uiMode)
{
	MultiPageWidgetDelegatePtr delegatePtr = MultiPageWidgetDelegatePtr(new DelegateImpl);

	m_containerWidgetDelegateMap[uiMode] = delegatePtr;
}


} // namespace iqtgui


#endif // !iqtgui_CMultiPageWidget_included


