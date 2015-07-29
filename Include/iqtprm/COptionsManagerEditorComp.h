#ifndef iqtprm_COptionsManagerEditorComp_included
#define iqtprm_COptionsManagerEditorComp_included


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

#include "GeneratedFiles/iqtprm/ui_COptionsManagerEditorComp.h"


namespace iqtprm
{


class COptionsManagerEditorComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::COptionsManagerEditorComp,
			iprm::IOptionsManager>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsManagerEditorComp,
				iprm::IOptionsManager> BaseClass;

	I_BEGIN_COMPONENT(COptionsManagerEditorComp);
		I_ASSIGN(m_allowAddRemoveAttrPtr, "AllowAddRemove", "If it is false, 'Add' and 'Remove' buttons will be always hidden", true, true);
		I_ASSIGN(m_allowUpDownAttrPtr, "AllowUpDown", "If it is false, 'Up' and 'Down' buttons will be always hidden", true, true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for page icons", false, 32);
	I_END_COMPONENT;

	COptionsManagerEditorComp();

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_UpButton_clicked();
	void on_DownButton_clicked();
	void on_OptionsList_itemSelectionChanged();
	void on_OptionsList_itemChanged(QListWidgetItem* item);

protected:
	void UpdateActions();
	void UpdateList();
	int GetSelectedIndex() const;
	void EnsureSelectedIndexUpdated() const;
	QByteArray GetSelectedParamsSetTypeId() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();

private:
	I_ATTR(bool, m_allowAddRemoveAttrPtr);
	I_ATTR(bool, m_allowUpDownAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);

	mutable int m_lastSelectedIndex;
};


} // namespace iqtprm


#endif // !iqtprm_COptionsManagerEditorComp_included


