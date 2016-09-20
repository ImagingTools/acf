#ifndef iqtprm_COptionsListEditorComp_included
#define iqtprm_COptionsListEditorComp_included


//Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif

// ACF includes
#include <iprm/IOptionsManager.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtprm/ui_COptionsListEditorComp.h>


namespace iqtprm
{


class COptionsListEditorComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::COptionsListEditorComp,
			iprm::IOptionsList>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsListEditorComp,
				iprm::IOptionsList> BaseClass;

	I_BEGIN_COMPONENT(COptionsListEditorComp);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for page icons", false, 32);
	I_END_COMPONENT;

	COptionsListEditorComp();

protected Q_SLOTS:
	void on_OptionsList_itemSelectionChanged();

protected:
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
	I_ATTR(int, m_iconSizeAttrPtr);

	mutable int m_lastSelectedIndex;
};


} // namespace iqtprm


#endif // !iqtprm_COptionsListEditorComp_included


