#include "iqsci/CTextEditorComp.h"


// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqsci
{


// reimplemented (imod::IModelEditor)

void CTextEditorComp::UpdateModel() const
{
	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	ibase::ITextDocument* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		objectPtr->SetText(iqt::GetCString(textEditPtr->text()));
	}
}


void CTextEditorComp::UpdateEditor()
{
	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	ibase::ITextDocument* objectPtr = GetObjectPtr();

	if ((objectPtr != NULL) && !textEditPtr->signalsBlocked()){
		textEditPtr->setText(iqt::GetQString(objectPtr->GetText()));
	}
}


// protected slots

void CTextEditorComp::OnTextChanged()
{
	iqt::CSignalBlocker block(this);

	UpdateModel();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTextEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	connect(textEditPtr, SIGNAL(TextChanged()), this, SLOT(OnTextChanged()));

	OnRetranslate();
}


void CTextEditorComp::OnRetranslate()
{
}


} // namespace iqsci


