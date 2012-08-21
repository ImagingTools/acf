#ifndef iqtprm_CParamsManagerGuiComp_included
#define iqtprm_CParamsManagerGuiComp_included


//Qt includes
#include <QtGui/QMenu>

// ACF includes
#include "iprm/IParamsManager.h"
#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CParamsManagerGuiComp.h"


namespace iqtprm
{


class CParamsManagerGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CParamsManagerGuiComp,
			iprm::IParamsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CParamsManagerGuiComp,
				iprm::IParamsManager> BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerGuiComp);
		I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
		I_ASSIGN(m_paramsObserverCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true);
		I_ASSIGN(m_allowUpDownAttrPtr, "AllowUpDown", "If it is false, 'Up' and 'Down' buttons will be always hidden", true, true);
	I_END_COMPONENT;

	CParamsManagerGuiComp();

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_UpButton_clicked();
	void on_DownButton_clicked();
	void on_ParamsTree_itemSelectionChanged();
	void on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column);
	void OnAddMenuOptionClicked(QAction* action);

protected:
	void UpdateActions();
	void UpdateTree();
	void UpdateParamsView(int selectedIndex);
	int GetSelectedIndex() const;
	void EnsureParamsGuiDetached();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_REF(imod::IObserver, m_paramsObserverCompPtr);
	I_ATTR(bool, m_allowAddRemoveAttrPtr);
	I_ATTR(bool, m_allowUpDownAttrPtr);

	imod::IModel* m_lastConnectedModelPtr;
	QMenu m_startVariableMenus;
};


} // namespace iqtprm


#endif // !iqtprm_CParamsManagerGuiComp_included


