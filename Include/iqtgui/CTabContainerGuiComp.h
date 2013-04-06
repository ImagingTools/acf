#ifndef iqtgui_CTabContainerGuiComp_included
#define iqtgui_CTabContainerGuiComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtGui/QTabWidget>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"
#include "imod/CMultiModelBridgeBase.h"
#include "iprm/IEnableableParam.h"
#include "iprm/CSelectionParam.h"
#include "iprm/COptionsManager.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Component to create the tab widget, that shows the specified sub-UI-component in its tab page.
	You can control some settings of the tabs such icons, title, orientation or tab visualization.
	Optionally, you can specify the corner widget, which will be placed beside of the last tab.
*/
class CTabContainerGuiComp:
			public iqtgui::TGuiComponentBase<QTabWidget>,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef iqtgui::TGuiComponentBase<QTabWidget> BaseClass;

	I_BEGIN_COMPONENT(CTabContainerGuiComp);
		I_REGISTER_SUBELEMENT(TabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::ISelectionParam, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::IOptionsList, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iser::ISerializable, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, imod::IModel, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, istd::IChangeable, ExtractTabModel);
		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "Guis", "Slave widgets for tab window", true);
		I_ASSIGN_MULTI_0(m_slaveWidgetsVisualCompPtr, "GuiVisualInfos", "Provide visual information for each GUI", false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsVisualCompPtr, false);
		I_ASSIGN_MULTI_0(m_tabActivatorsCompPtr, "TabActivators", "Optional activators for each tab", false);
		I_ASSIGN_TO(m_tabActivatorsModelCompPtr, m_tabActivatorsCompPtr, false);
		I_ASSIGN(m_cornerGuiCompPtr, "CornerWidget", "Optional corner widget", false, "CornerWidget");
		I_ASSIGN_MULTI_0(m_tabNamesAttrPtr, "Names", "Titles for the tab", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for tab icons", false, 16);
		I_ASSIGN(m_useTriangularTabsAttrPtr, "UseTriangularTabs", "Using triangular tab form", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_flatStyleAttrPtr, "FlatTabs", "Flat style of the tabs", true, false);
	I_END_COMPONENT;

protected:
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
    virtual void OnComponentCreated();
    virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	class TabModel:
				public iprm::CSelectionParam,
				public imod::CMultiModelBridgeBase,
				public iprm::IOptionsList
	{
	public:
		typedef iprm::CSelectionParam BaseClass;

		TabModel();

		void SetParent(CTabContainerGuiComp* parentPtr);
		void UpdateTabsState();

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
		CTabContainerGuiComp* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractTabModel(CTabContainerGuiComp& component)
	{
		return &component.m_tabModel;
	}

private:
	I_MULTIREF(IGuiObject, m_slaveWidgetsCompPtr);
	I_MULTIREF(IVisualStatusProvider, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_MULTIREF(iprm::IEnableableParam, m_tabActivatorsCompPtr);
	I_MULTIREF(imod::IModel, m_tabActivatorsModelCompPtr);
	I_REF(iqtgui::IGuiObject, m_cornerGuiCompPtr);
	I_MULTIATTR(QString, m_tabNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useTriangularTabsAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(bool, m_flatStyleAttrPtr);

	typedef QMap<int, int> TabToGuiIndexMap;
	TabToGuiIndexMap m_tabToGuiIndexMap;

	imod::TModelWrap<TabModel> m_tabModel;
};


} // namespace iqtgui


#endif // !iqtgui_CTabContainerGuiComp_included

