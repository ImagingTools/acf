#include "icmpstr/CComponentNoteEditorComp.h"


// Qt includes
#include <QtGui/QTextDocument>

// ACF includes
#include "iqt/CSignalBlocker.h"

#include "icmpstr/IComponentNoteController.h"


namespace icmpstr
{


CComponentNoteEditorComp::CComponentNoteEditorComp()
	:m_textWasChanged(false)
{
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CComponentNoteEditorComp::UpdateGui(int /*updateFlags*/)
{
	IElementSelectionInfo* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	iqt::CSignalBlocker blockTextEditor(NoteEditor);
	
	icmpstr::IComponentNoteController* componentNoteControllerPtr = dynamic_cast<icmpstr::IComponentNoteController*>(objectPtr->GetSelectedRegistry());
	if (componentNoteControllerPtr != NULL){
		IElementSelectionInfo::Elements elements = objectPtr->GetSelectedElements();

		QStringList notes;

		for (		IElementSelectionInfo::Elements::const_iterator index = elements.begin();
					index != elements.end();
					index++){

			QString noteText = componentNoteControllerPtr->GetComponentNote(index.key());
			if (!noteText.isEmpty()){
				notes.push_back(noteText);
			}
		}

		NoteEditor->setReadOnly(elements.size() != 1);
		NoteEditor->setPlainText(notes.join("\n"));
		NoteEditor->setTextCursor(m_lastCursorPosition);

		return;
	}
	else{
		NoteEditor->clear();
	}
}


void CComponentNoteEditorComp::UpdateModel() const
{
	IElementSelectionInfo* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	icmpstr::IComponentNoteController* componentNoteControllerPtr = dynamic_cast<icmpstr::IComponentNoteController*>(objectPtr->GetSelectedRegistry());
	if (componentNoteControllerPtr != NULL){
		IElementSelectionInfo::Elements elements = objectPtr->GetSelectedElements();
		if (elements.size() == 1){
			QString currentNote = NoteEditor->toPlainText();

			componentNoteControllerPtr->SetComponentNote(elements.begin().key(), currentNote);
		}
	}
}


void CComponentNoteEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	IElementSelectionInfo* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);
	
	imod::IModel* registryModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetSelectedRegistry());
	if (registryModelPtr != NULL){
		RegisterModel(registryModelPtr);
	}
}


void CComponentNoteEditorComp::OnGuiModelDetached()
{
	UnregisterAllModels();

	BaseClass::OnGuiModelDetached();
}



// reimplemented (QObject)

bool CComponentNoteEditorComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (sourcePtr->objectName() == NoteEditor->objectName()){
		if (eventPtr->type() == QEvent::FocusOut){
			if (m_textWasChanged){
				DoUpdateModel();

				m_textWasChanged = false;
			}
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CComponentNoteEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	NoteEditor->setReadOnly(true);
	m_lastCursorPosition = NoteEditor->textCursor();

	NoteEditor->installEventFilter(this);
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CComponentNoteEditorComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	UpdateEditor(istd::IChangeable::CF_MODEL);
}


// protected slots

void CComponentNoteEditorComp::on_NoteEditor_textChanged()
{
	m_textWasChanged = true;
}


} // namespace icmpstr