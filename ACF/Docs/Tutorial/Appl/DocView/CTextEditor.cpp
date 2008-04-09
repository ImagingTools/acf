#include "CTextEditor.h"


#include "iqt/CSignalBlocker.h"


// reimplemented (iqt::TGuiModelJoinerWrap)

void CTextEditor::UpdateModel()
{
	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	CTextModel* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		objectPtr->SetText(iqt::GetCString(textEditPtr->toPlainText()));
	}
}


void CTextEditor::UpdateGui()
{
	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

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


