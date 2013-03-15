#ifndef iqtprm_COptionsListEditorGuiComp_included
#define iqtprm_COptionsListEditorGuiComp_included


//Qt includes
#include <QtGui/QMenu>

// ACF includes
#include "iprm/IOptionsManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_COptionsListEditorGuiComp.h"


namespace iqtprm
{


class COptionsListEditorGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::COptionsListEditorGuiComp,
			iprm::IOptionsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsListEditorGuiComp,
				iprm::IOptionsManager> BaseClass;

	I_BEGIN_COMPONENT(COptionsListEditorGuiComp);
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true);
		I_ASSIGN(m_allowUpDownAttrPtr, "AllowUpDown", "If it is false, 'Up' and 'Down' buttons will be always hidden", true, true);
		I_ASSIGN(m_showDescriptionAttrPtr, "ShowDescription", "If it is true, a desciption of each value is available and editable", true, false);
	I_END_COMPONENT;

	COptionsListEditorGuiComp();

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
	I_ATTR(bool, m_allowAddRemoveAttrPtr);
	I_ATTR(bool, m_allowUpDownAttrPtr);
	I_ATTR(bool, m_showDescriptionAttrPtr);

	imod::IModel* m_lastConnectedModelPtr;
	imod::IObserver* m_lastObserverPtr;
	QMenu m_startVariableMenus;

	int m_lastSelectedIndex;
};


} // namespace iqtprm


#endif // !iqtprm_COptionsListEditorGuiComp_included


