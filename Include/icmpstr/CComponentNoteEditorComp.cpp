#include "icmpstr/CComponentNoteEditorComp.h"


// Qt includes
#include <QTextDocument>


// ACF includes
#include "iqt/CSignalBlocker.h"

#include "icmpstr/IComponentNoteController.h"


namespace icmpstr
{


CComponentNoteEditorComp::CComponentNoteEditorComp()
{
}


// protected methods

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

			QString noteText = iqt::GetQString(componentNoteControllerPtr->GetComponentNote(index->first));
			if (!noteText.isEmpty()){
				notes.push_back(noteText);
			}
		}

		NoteEditor->setReadOnly(elements.size() != 1);
		NoteEditor->setPlainText(notes.join("\n"));

		return;
	}
	else{
		NoteEditor->clear();
	}
}


void CComponentNoteEditorComp::UpdateModel()
{
	IElementSelectionInfo* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	icmpstr::IComponentNoteController* componentNoteControllerPtr = dynamic_cast<icmpstr::IComponentNoteController*>(objectPtr->GetSelectedRegistry());
	if (componentNoteControllerPtr != NULL){
		IElementSelectionInfo::Elements elements = objectPtr->GetSelectedElements();
		if (elements.size() == 1){
			istd::CString currentNote = iqt::GetCString(NoteEditor->toPlainText());

			componentNoteControllerPtr->SetComponentNote(elements.begin()->first, currentNote);
		}
	}
}


// protected slots

void CComponentNoteEditorComp::on_NoteEditor_textChanged()
{
	CommitButton->setVisible(!NoteEditor->isReadOnly());
}


void CComponentNoteEditorComp::on_CommitButton_clicked()
{
	if (!IsUpdateBlocked() && IsModelAttached(NULL)){
		UpdateBlocker updateBlocker(this);

		UpdateModel();

		CommitButton->setVisible(false);
	}
}


} // namespace icmpstr