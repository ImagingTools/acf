#ifndef iqtgui_CMultiPageWidget_included
#define iqtgui_CMultiPageWidget_included


// Qt includes
#include <QtCore/QSize>
#include <QtCore/QMap>
#include <QtGui/QWidget>

// ACF includes
#include <istd/TSmartPtr.h>
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
	typedef QObject BaseClass;

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
		DT_SPLITTER
	};

	CMultiPageWidget(int designMode = DT_SIMPLE, bool useHorizontalLayout = true);

	/**
		Add a new page to the container. If \c pageIndex is negative, the new page will be inserted after the last page.
		The container overtakes the control about \c widgetPtr.
		\return Position of the inserted page.
	*/
	virtual int InsertPage(
				QWidget* widgetPtr,
				const QString& pageTitle,
				int pageIndex = -1);

	/**
		Remove the page from the container. The page widget will be destroyed.
	*/
	virtual void RemovePage(int pageIndex);

	/**
		Set the page with the given index enabled/disabled.
	*/
	virtual void SetPageEnabled(int pageIndex, bool isEnabled = true);

	/**
		Set the page with the given index visible/hidden.
	*/
	virtual void SetPageVisible(int pageIndex, bool isVisible = true);

	/**
		Get number of the page in the container.
	*/
	virtual int GetPagesCount() const;

	/**
		Get the page widget.
	*/
	virtual QWidget* GetPageWidgetPtr(int pageIndex) const;

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

	/**
		Get page container widget.
	*/
	virtual QWidget* GetGuiContainerPtr() const;

	/**
		Create container GUI in some parent widget.
	*/
	virtual bool CreateContainerGui();

private:
	typedef istd::TSmartPtr<IMultiPageWidgetDelegate> MultiPageWidgetDelegatePtr;
	typedef QMap<int, MultiPageWidgetDelegatePtr> ContainerWidgetDelegateMap;
		
	ContainerWidgetDelegateMap m_containerWidgetDelegateMap;

	/**
		A container that depends on \c DesignType, i.e. QWidget, QToolBox or QTabWidget
	*/
	QWidget* m_guiContainerPtr;

	int m_designMode;

	bool m_useHorizontalLayout;

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


