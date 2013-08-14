#ifndef iqtgui_CContainerGuiBase_included
#define iqtgui_CContainerGuiBase_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"
#include "iprm/CSelectionParam.h"
#include "iprm/COptionsManager.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	A basic class for selectable GUI containers.
*/
class CContainerGuiBase: 
			virtual public CGuiComponentBase,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef CGuiComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CContainerGuiBase);
		I_REGISTER_SUBELEMENT(TabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::ISelectionParam, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::IOptionsList, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iprm::IOptionsManager, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, iser::ISerializable, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, imod::IModel, ExtractTabModel);
		I_REGISTER_SUBELEMENT_INTERFACE_T(TabModel, istd::IChangeable, ExtractTabModel);

		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "Guis", "Slave widgets for tab window", true);
		I_ASSIGN_MULTI_0(m_slaveWidgetsVisualCompPtr, "GuiVisualInfos", "Provide visual information for each GUI", false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsVisualCompPtr, false);
		I_ASSIGN_MULTI_0(m_tabNamesAttrPtr, "Names", "Titles for the buttons", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for button icons", true, 16);
	I_END_COMPONENT;

protected:
	virtual bool CreateMainGui(QSize iconSize) = 0;
	virtual int CreatePageGui(const QString& name, QSize iconSize, iqtgui::IGuiObject* guiPtr) = 0;
	virtual void UpdateItem(int index, const QIcon& icon, const QString& toolTip) = 0;
	virtual bool SetSelectedIndex(int index) = 0;
	virtual bool SetEnabled(int index, bool isEnabled) = 0;

	virtual void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	class TabModel:
		public iprm::CSelectionParam,
		public iprm::COptionsManager
	{
	public:
		typedef iprm::CSelectionParam BaseClass;
		typedef iprm::COptionsManager BaseClass2;

		TabModel();

		void SetParent(CContainerGuiBase* parentPtr);

		// reimplemented (iprm::ISelectionParam)
		virtual bool SetSelectedOptionIndex(int index);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		// reimplemented (iprm::IOptionsManager)
		virtual bool SetOptionEnabled(int index, bool isEnabled = true);
		virtual bool RemoveOption(int index);
		virtual bool InsertOption(
			const QString& optionName,
			const QByteArray& optionId,
			const QString& optionDescription = QString(), 
			int index = -1);

	private:
		CContainerGuiBase* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractTabModel(CContainerGuiBase& component)
	{
		return &component.m_tabModel;
	}

private:
	I_MULTIREF(IGuiObject, m_slaveWidgetsCompPtr);
	I_MULTIREF(IVisualStatusProvider, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_MULTIATTR(QString, m_tabNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);

	typedef QMap<int, int> TabToGuiIndexMap;
	TabToGuiIndexMap m_tabToGuiIndexMap;

	imod::TModelWrap<TabModel> m_tabModel;
};


template <class WidgetType>
class TContainerGuiBase: public CContainerGuiBase
{
public:
	typedef CContainerGuiBase BaseClass;

	TContainerGuiBase()
	{}

	virtual WidgetType* GetQtWidget() const
	{
		return dynamic_cast<WidgetType*>(GetWidget());
	}

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr)
	{
		Q_ASSERT(!IsGuiCreated());

		WidgetType* widgetPtr = new WidgetType(parentPtr);

		return widgetPtr;
	}
};


} // namespace iqtgui


#endif // !iqtgui_CContainerGuiBase_included
