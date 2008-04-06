#include "CTextEditor.h"

#include "iqt/CSignalBlocker.h"


// public methods

CTextEditor::CTextEditor()
{
	connect(this, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
}



// reimplemented (imod::IModelEditor)

void CTextEditor::UpdateModel()
{
	iqt::CSignalBlocker block(this);

	CTextModel* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		objectPtr->SetText(iqt::GetCString(toPlainText()));
	}
}


void CTextEditor::UpdateEditor()
{
	CTextModel* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		setText(iqt::GetQString(objectPtr->GetText()));
	}
}


// protected slots

void CTextEditor::OnTextChanged()
{
	UpdateModel();
}
