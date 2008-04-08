#include "CTextEditor.h"


#include "iqt/CSignalBlocker.h"


// reimplemented (imod::IModelEditor)

void CTextEditor::UpdateModel()
{
	QTextEdit* textEditPtr = GetQtWidget();
	if (textEditPtr == NULL){
		return;
	}

	iqt::CSignalBlocker block(textEditPtr);

	CTextModel* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		objectPtr->SetText(iqt::GetCString(textEditPtr->toPlainText()));
	}
}


void CTextEditor::UpdateEditor()
{
	QTextEdit* textEditPtr = GetQtWidget();
	if (textEditPtr == NULL){
		return;
	}

	CTextModel* objectPtr = GetObjectPtr();

	if ((objectPtr != NULL) && !textEditPtr->signalsBlocked()){
		textEditPtr->setText(iqt::GetQString(objectPtr->GetText()));
	}
}


// protected slots

void CTextEditor::OnTextChanged()
{
	UpdateModel();
}


// reimplemented (iqt::CGuiComponentBase)

void CTextEditor::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	connect(textEditPtr, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
}


