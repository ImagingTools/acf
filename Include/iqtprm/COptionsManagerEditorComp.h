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
#include <ifile/IFilePersistence.h>
#include <iprm/IOptionsManager.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtprm/ui_COptionsManagerEditorComp.h>


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
		I_ASSIGN(m_defaultOptionNameAttrPtr, "DefaultOptionName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "Option %1");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loader for the parameter set", false, "ParamsLoader");
	I_END_COMPONENT;

	COptionsManagerEditorComp();

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_UpButton_clicked();
	void on_DownButton_clicked();
	void on_OptionsList_itemSelectionChanged();
	void on_OptionsList_itemChanged(QListWidgetItem* item);
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	void UpdateActions();
	void UpdateList();
	int GetSelectedIndex() const;
	void EnsureSelectedIndexUpdated() const;
	QByteArray GetSelectedParamsSetTypeId() const;

	QString CalculateNewDefaultName() const;

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
	I_TEXTATTR(m_defaultOptionNameAttrPtr);
	I_REF(ifile::IFilePersistence, m_paramsLoaderCompPtr);

	mutable int m_lastSelectedIndex;
};


} // namespace iqtprm


#endif // !iqtprm_COptionsManagerEditorComp_included


