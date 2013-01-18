#ifndef iqtprm_CParamsManagerGuiCompBase_included
#define iqtprm_CParamsManagerGuiCompBase_included


//Qt includes
#include <QtGui/QMenu>

// ACF includes
#include "iprm/IParamsManager.h"
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CParamsManagerGuiCompBase.h"


namespace iqtprm
{


class CParamsManagerGuiCompBase: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CParamsManagerGuiCompBase,
			iprm::IParamsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CParamsManagerGuiCompBase,
				iprm::IParamsManager> BaseClass;

	I_BEGIN_BASE_COMPONENT(CParamsManagerGuiCompBase);
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true);
		I_ASSIGN(m_allowUpDownAttrPtr, "AllowUpDown", "If it is false, 'Up' and 'Down' buttons will be always hidden", true, true);
		I_ASSIGN(m_iconProviderCompPtr, "IconProvider", "Icons for drop-down types menu", false, "IconProvider");
	I_END_COMPONENT;

	CParamsManagerGuiCompBase();

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_UpButton_clicked();
	void on_DownButton_clicked();
	void on_ParamsTree_itemSelectionChanged();
	void on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column);
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

	void UpdateActions();
	void UpdateTree();
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

private:
	I_REF(iqtgui::IIconProvider, m_iconProviderCompPtr);
	I_ATTR(bool, m_allowAddRemoveAttrPtr);
	I_ATTR(bool, m_allowUpDownAttrPtr);

	imod::IModel* m_lastConnectedModelPtr;
	imod::IObserver* m_lastObserverPtr;
	QMenu m_startVariableMenus;
	QMap<QByteArray, int> m_factoryIconIndexMap;
};


} // namespace iqtprm


#endif // !iqtprm_CParamsManagerGuiCompBase_included


