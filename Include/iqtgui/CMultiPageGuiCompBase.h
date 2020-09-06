#ifndef iqtgui_CMultiPageGuiCompBase_included
#define iqtgui_CMultiPageGuiCompBase_included


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QVector>

// ACF includes
#include <imod/CMultiModelDispatcherBase.h>
#include <imod/CModelUpdateBridge.h>
#include <iprm/IEnableableParam.h>
#include <iprm/CSelectionParam.h>
#include <iprm/COptionsManager.h>
#include <iqtgui/IMultiVisualStatusProvider.h>
#include <iqtgui/CGuiComponentBase.h>
#include <iwidgets/CMultiPageWidget.h>


namespace iqtgui
{

class CMultiPageGuiCompAttr: public iqtgui::CGuiComponentBase
{
public:
	typedef iqtgui::CGuiComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CMultiPageGuiCompAttr);
		I_ASSIGN_MULTI_0(m_pageNamesAttrPtr, "Names", "List of the page titles", true);
		I_ASSIGN_MULTI_0(m_pageIdsAttrPtr, "PageIds", "List of unique page IDs", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for page icons", false, 16);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout", true, false);
		I_ASSIGN(m_useSameStretchingFactorAttrPtr, "UseSameStretchingFactor", "Set the same stretching factor for all widgets. Only for group box mode", false, false);
		I_ASSIGN(m_insertSpacerAttrPtr, "InsertSpacer", "If enabled, insert spacer after the last page widget", false, false);
		I_ASSIGN(m_lazyPagesInitializationAttrPtr, "LazyPagesInitialization", "If enabled, CreateGui for a page will be called only when this page will be selected", true, false);
		I_ASSIGN(m_supressPageTitleAttrPtr, "SupressPageTitle", "Supress page title on the container widget", true, false);
		I_ASSIGN(m_defaultPageIndexAttrPtr, "DefaultStartPageIndex", "Index of the start page", true, 0);
	I_END_COMPONENT;

protected:
	I_MULTITEXTATTR(m_pageNamesAttrPtr);
	I_MULTIATTR(QByteArray, m_pageIdsAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(bool, m_useSameStretchingFactorAttrPtr);
	I_ATTR(bool, m_insertSpacerAttrPtr);
	I_ATTR(bool, m_lazyPagesInitializationAttrPtr);
	I_ATTR(bool, m_supressPageTitleAttrPtr);
	I_ATTR(int, m_defaultPageIndexAttrPtr);
};


class CMultiPageGuiCompBase:
			public CMultiPageGuiCompAttr,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef CMultiPageGuiCompAttr BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	enum ModelId
	{
		/**
			Minimal number which should be used for user-specific data models.
		*/
		MI_USER = 1000
	};

	I_BEGIN_BASE_COMPONENT(CMultiPageGuiCompBase);
		I_REGISTER_SUBELEMENT(PageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iprm::ISelectionParam, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iprm::IOptionsList, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iser::ISerializable, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, imod::IModel, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, istd::IChangeable, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, IMultiVisualStatusProvider, ExtractPageModel);
		I_ASSIGN_MULTI_0(m_slaveWidgetsVisualCompPtr, "GuiVisualInfos", "Provide visual information for each GUI", false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsVisualCompPtr, false);
		I_ASSIGN_MULTI_0(m_pageActivatorsCompPtr, "PageActivators", "Optional activators for each page (enable/disable)", false);
		I_ASSIGN_TO(m_pageActivatorsModelCompPtr, m_pageActivatorsCompPtr, false);
		I_ASSIGN_MULTI_0(m_pageVisibilityActivatorsCompPtr, "PageVisibilityActivators", "Optional visibility activators for each page (show/hide)", false);
		I_ASSIGN_TO(m_pageVisibilityActivatorsModelCompPtr, m_pageVisibilityActivatorsCompPtr, false);
	I_END_COMPONENT;

	CMultiPageGuiCompBase();

	iprm::ISelectionParam& GetPageModelRef();

	/**
		Get the number of the pages in the UI container.
	*/
	virtual int GetPagesCount() const = 0;

	/**
		Get the UI component for a given page.
	*/
	virtual iqtgui::IGuiObject* GetPageGuiComponent(int pageIndex) const = 0;

protected:
	/**
		Get the container UI mode.
		\sa iqtgui::CMultiPageWidget::DesignType
	*/
	virtual int GetDesignType() const = 0;

	/**
		Get logical page index from a given widget index.
	*/
	virtual int GetLogicalPageIndex(int physicalWidgetIndex) const;

	/**
		Create a page in the page container widget.
	*/
	virtual bool CreatePage(int pageIndex);

	/**
		Remove a page from the container widget.
	*/
	virtual void RemovePage(int pageIndex);

	/**
		Set current page.
	*/
	virtual void SetCurrentPage(int pageIndex);

	/**
		Update visual status of the page elements.
	*/
	virtual void UpdateVisualElements();

	/**
		Create all container pages.
	*/
	virtual void CreatePages();

	/**
		Clear all container pages.
	*/
	virtual void ResetPages();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual QWidget* CreateQtWidget(QWidget* parentPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	virtual void OnPageChanged(int widgetIndex);

private:
	class PageModel:
				public iprm::CSelectionParam,
				public iprm::IOptionsList,
				public IMultiVisualStatusProvider
	{
	public:
		typedef iprm::CSelectionParam BaseClass;

		PageModel();

		void SetParent(CMultiPageGuiCompBase* parentPtr);
		void UpdatePageState();

		// reimplemented (iprm::ISelectionParam)
		virtual bool SetSelectedOptionIndex(int index);
		virtual ISelectionParam* GetSubselection(int index) const;

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

		// reimplemented (iqtgui::IMultiVisualStatusProvider)
		virtual int GetStatusesCount() const;
		virtual const IVisualStatus* GetVisualStatus(int statusIndex) const;

	protected:
		// reimplemented (istd::IChangeable)
		virtual void OnEndChanges(const ChangeSet& changeSet);

	private:
		CMultiPageGuiCompBase* m_parentPtr;

		imod::CModelUpdateBridge m_updateBridge;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractPageModel(CMultiPageGuiCompBase& component)
	{
		return &component.m_pageModel;
	}

protected:
	bool EnsurePageInitialized(int pageIndex);

	I_MULTIREF(IVisualStatus, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_MULTIREF(iprm::IEnableableParam, m_pageActivatorsCompPtr);
	I_MULTIREF(imod::IModel, m_pageActivatorsModelCompPtr);
	I_MULTIREF(iprm::IEnableableParam, m_pageVisibilityActivatorsCompPtr);
	I_MULTIREF(imod::IModel, m_pageVisibilityActivatorsModelCompPtr);

	imod::TModelWrap<PageModel> m_pageModel;

private:
	struct PageInfo
	{
		PageInfo(): isCreated(false), widgetIndex(-1), widgetPtr(NULL){}

		bool isCreated;		// true, if page was created
		QString pageTitle;	// title of this page
		QByteArray pageId;
		int widgetIndex;	// index of widget in widget container
		QWidget* widgetPtr;	// widget whera page contents will be placed
	};
	typedef QMap<int, PageInfo> PageIndexToInfoMap;
	PageIndexToInfoMap m_pageIndexToInfoMap;
};


} // namespace iqtgui


#endif // !iqtgui_CMultiPageGuiCompBase_included


