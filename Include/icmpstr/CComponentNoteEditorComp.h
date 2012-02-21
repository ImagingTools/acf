#ifndef icmpstr_CComponentNoteEditorComp_included
#define icmpstr_CComponentNoteEditorComp_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "icmpstr/IElementSelectionInfo.h"

#include "Generated/ui_CComponentNoteEditorComp.h"


namespace icmpstr
{


class CComponentNoteEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CComponentNoteEditorComp, IElementSelectionInfo>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CComponentNoteEditorComp, IElementSelectionInfo> BaseClass;

	I_BEGIN_COMPONENT(CComponentNoteEditorComp);
	I_END_COMPONENT;

	CComponentNoteEditorComp();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);
	virtual void UpdateModel();

protected Q_SLOTS:
	void on_NoteEditor_textChanged();
	void on_CommitButton_clicked();
};


} // namespace icmpstr


#endif // !icmpstr_CComponentNoteEditorComp_included

