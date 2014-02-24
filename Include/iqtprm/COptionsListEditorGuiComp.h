#ifndef iqtprm_COptionsListEditorGuiComp_included
#define iqtprm_COptionsListEditorGuiComp_included


//Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif

// ACF includes
#include "iprm/IOptionsManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "GeneratedFiles/iqtprm/ui_COptionsListEditorGuiComp.h"


namespace iqtprm
{


class COptionsListEditorGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::COptionsListEditorGuiComp,
			iprm::IOptionsList>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsListEditorGuiComp,
				iprm::IOptionsList> BaseClass;

	I_BEGIN_COMPONENT(COptionsListEditorGuiComp);
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true);
		I_ASSIGN(m_allowUpDownAttrPtr, "AllowUpDown", "If it is false, 'Up' and 'Down' buttons will be always hidden", true, true);
		I_ASSIGN(m_showDescriptionAttrPtr, "ShowDescription", "If it is true, a desciption of each value is available and editable", true, false);
		I_ASSIGN(m_showListHeaderAttrPtr, "ShowHeader", "If it is true, the headers for the option list is shown", true, false);
		I_ASSIGN_MULTI_0(m_headerListAttrPtr, "HeaderItems", "Header labels for the columns", true);
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
	void EnsureSelectedIndexUpdated() const;
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
	I_ATTR(bool, m_showListHeaderAttrPtr);
	I_MULTIATTR(QString, m_headerListAttrPtr);

	QMenu m_startVariableMenus;

	mutable int m_lastSelectedIndex;
};


} // namespace iqtprm


#endif // !iqtprm_COptionsListEditorGuiComp_included


