#ifndef icmpstr_CComponentNoteEditorComp_included
#define icmpstr_CComponentNoteEditorComp_included


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "icmpstr/IElementSelectionInfo.h"

#include "Generated/ui_CComponentNoteEditorComp.h"


namespace icmpstr
{


class CComponentNoteEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CComponentNoteEditorComp, IElementSelectionInfo>,
			protected imod::CMultiModelDispatcherBase
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
	virtual void UpdateModel() const;
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

protected Q_SLOTS:
	void on_NoteEditor_textChanged();

private:
	QTextCursor m_lastCursorPosition;
	bool m_textWasChanged;
};


} // namespace icmpstr


#endif // !icmpstr_CComponentNoteEditorComp_included

