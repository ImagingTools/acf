#ifndef iqtgui_CMultiPageGuiCompBase_included
#define iqtgui_CMultiPageGuiCompBase_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"
#include "imod/CMultiModelBridgeBase.h"
#include "iprm/IEnableableParam.h"
#include "iprm/CSelectionParam.h"
#include "iprm/COptionsManager.h"
#include "iqtgui/CGuiComponentBase.h"
#include "iqtgui/CMultiPageWidget.h"


namespace iqtgui
{


class CMultiPageGuiCompBase:
			public iqtgui::CGuiComponentBase,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef iqtgui::CGuiComponentBase BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_BASE_COMPONENT(CMultiPageGuiCompBase);
		I_REGISTER_SUBELEMENT(PageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iprm::ISelectionParam, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iprm::IOptionsList, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, iser::ISerializable, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, imod::IModel, ExtractPageModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(PageModel, istd::IChangeable, ExtractPageModel);
		I_ASSIGN_MULTI_0(m_slaveWidgetsVisualCompPtr, "GuiVisualInfos", "Provide visual information for each GUI", false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsVisualCompPtr, false);
		I_ASSIGN_MULTI_0(m_pageActivatorsCompPtr, "PageActivators", "Optional activators for each page", false);
		I_ASSIGN_TO(m_pageActivatorsModelCompPtr, m_pageActivatorsCompPtr, false);
		I_ASSIGN_MULTI_0(m_pageNamesAttrPtr, "Names", "List of the page titles", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for page icons", false, 16);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout", true, false);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 0 - simple\n* 1 - tool box\n* 2 - tab\n* 3 - widget stack\n* 4 - splitter", true, CMultiPageWidget::DT_SIMPLE);
		I_ASSIGN(m_useSameStretchingFactorAttrPtr, "UseSameStretchingFactor", "Set the same stretching factor for all widgets. Only for group box mode", false, false);
		I_ASSIGN(m_insertSpacerAttrPtr, "InsertSpacer", "If enabled, spacer will be added to fill whole available area", true, false);
	I_END_COMPONENT;

protected:
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
		Add a new page to the widget container.
	*/
	virtual int AddPageToContainerWidget(iqtgui::IGuiObject& pageGui, const QString& pageTitle) const;

	/**
		Get the name corresponding to a page GUI element.
	*/
	QString GetPageGuiName(const iqtgui::IGuiObject& pageGui) const;

	/**
		Create a page in the page container widget.
	*/
	bool CreatePage(int guiIndex);

	/**
		Update visual status of the page elements.
	*/
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	class PageModel:
				public iprm::CSelectionParam,
				public imod::CMultiModelBridgeBase,
				public iprm::IOptionsList
	{
	public:
		typedef iprm::CSelectionParam BaseClass;

		PageModel();

		void SetParent(CMultiPageGuiCompBase* parentPtr);
		void UpdatePageState();

		// reimplemented (iprm::ISelectionParam)
		virtual bool SetSelectedOptionIndex(int index);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

	protected:
		// reimplemented (istd::IChangeable)
		virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	private:
		CMultiPageGuiCompBase* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractPageModel(CMultiPageGuiCompBase& component)
	{
		return &component.m_pageModel;
	}

protected:
	I_MULTIREF(IVisualStatusProvider, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_MULTIREF(iprm::IEnableableParam, m_pageActivatorsCompPtr);
	I_MULTIREF(imod::IModel, m_pageActivatorsModelCompPtr);
	I_MULTIATTR(QString, m_pageNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(bool, m_useSameStretchingFactorAttrPtr);
	I_ATTR(bool, m_insertSpacerAttrPtr);

private:
	typedef QMap<int /*page index in the widget container*/, int /*logical GUI element index*/> PageToGuiIndexMap;
	PageToGuiIndexMap m_pageToGuiIndexMap;

	QMap<const iqtgui::IGuiObject*, QString> m_guiNamesMap;

	imod::TModelWrap<PageModel> m_pageModel;
};


} // namespace iqtgui


#endif // !iqtgui_CMultiPageGuiCompBase_included


