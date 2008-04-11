#include "CTextEditorComp.h"


#include "iqt/CSignalBlocker.h"


// reimplemented (iqt::TGuiObserverWrap)

void CTextEditorComp::UpdateModel() const
{
	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	IText* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		objectPtr->SetText(iqt::GetCString(textEditPtr->toPlainText()));
	}
}


void CTextEditorComp::UpdateEditor()
{
	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	IText* objectPtr = GetObjectPtr();

	if ((objectPtr != NULL) && !textEditPtr->signalsBlocked()){
		textEditPtr->setText(iqt::GetQString(objectPtr->GetText()));
	}
}


// protected slots

void CTextEditorComp::OnTextChanged()
{
	UpdateModel();
}


// reimplemented (iqt::CGuiComponentBase)

void CTextEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	connect(textEditPtr, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
}


