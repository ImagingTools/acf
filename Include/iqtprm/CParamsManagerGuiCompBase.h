#ifndef iqtprm_CParamsManagerGuiCompBase_included
#define iqtprm_CParamsManagerGuiCompBase_included


//Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif

// ACF includes
#include "iprm/IParamsManager.h"
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqt2d/IViewExtender.h"
#include "GeneratedFiles/iqtprm/ui_CParamsManagerGuiCompBase.h"


namespace iqtprm
{


class CParamsManagerGuiCompBase:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CParamsManagerGuiCompBase, iprm::IParamsManager>,
			virtual public iqt2d::IViewExtender


{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CParamsManagerGuiCompBase,
				iprm::IParamsManager> BaseClass;

	I_BEGIN_BASE_COMPONENT(CParamsManagerGuiCompBase);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true);
		I_ASSIGN(m_allowUpDownAttrPtr, "AllowUpDown", "If it is false, 'Up' and 'Down' buttons will be always hidden", true, true);
		I_ASSIGN(m_supportEnablingAttrPtr, "AllowEnable", "If this option is activated, the single parameter sets can be enabled/disabled in the list", true, false);
		I_ASSIGN(m_comboBoxViewAttrPtr, "CompactView", "Shows parameters list as a combo box", true, false);
		I_ASSIGN(m_iconProviderCompPtr, "IconProvider", "Icons for drop-down types menu", false, "IconProvider");
	I_END_COMPONENT;

	CParamsManagerGuiCompBase();

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_UpButton_clicked();
	void on_DownButton_clicked();
	void on_ParamsTree_itemSelectionChanged();
	void on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_ParamsComboBox_currentIndexChanged(int index);
	void on_ParamsComboBox_editTextChanged(const QString& text);
	void OnAddMenuOptionClicked(QAction* action);

protected:
	// abstract methods

	/**
		Get the observer for a given parameter set.
	*/
	virtual imod::IObserver* GetObserverPtr(const iprm::IParamsSet* paramsSetPtr) const = 0;

	/**
		Get the editor GUI for a given parameter set.
	*/
	virtual iqtgui::IGuiObject* GetEditorGuiPtr(const iprm::IParamsSet* paramsSetPtr) const = 0;

	/**
		Get the currently active view extender.
	*/
	virtual iqt2d::IViewExtender* GetCurrentViewExtenderPtr() const;

	void UpdateActions();
	void UpdateTree();
	void UpdateComboBox();
	virtual void UpdateParamsView(int selectedIndex);
	int GetSelectedIndex() const;
	void EnsureParamsGuiDetached();
	QByteArray GetSelectedParamsSetTypeId() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	void AttachCurrentExtender();
	void DetachCurrentExtender();

private:
	I_REF(iqtgui::IIconProvider, m_iconProviderCompPtr);
	I_ATTR(bool, m_allowAddRemoveAttrPtr);
	I_ATTR(bool, m_allowUpDownAttrPtr);
	I_ATTR(bool, m_supportEnablingAttrPtr);
	I_ATTR(bool, m_comboBoxViewAttrPtr);
	I_REF(iqt2d::IViewExtender, m_extenderCompPtr);

	typedef QMap<int, QIcon> StateIconsMap;

	imod::IModel* m_lastConnectedModelPtr;
	imod::IObserver* m_lastObserverPtr;
	QMenu m_startVariableMenus;
	QMap<QByteArray, int> m_factoryIconIndexMap;
	StateIconsMap m_stateIconsMap;

	typedef QMap<iqt2d::IViewProvider*, int> ConnectedSceneFlags; // maps connected scene provider to connection flags
	ConnectedSceneFlags m_connectedSceneFlags;
};


} // namespace iqtprm


#endif // !iqtprm_CParamsManagerGuiCompBase_included


