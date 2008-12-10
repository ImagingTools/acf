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


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CTextEditorComp::GetCommands() const
{
	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		I_ASSERT(textEditPtr != NULL);
		
		return textEditPtr->GetCommands();
	}

	return NULL;
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

	connect(textEditPtr, SIGNAL(DataChanged()), this, SLOT(OnTextChanged()));

	if (m_useFoldingAttrPtr.IsValid() && *m_useFoldingAttrPtr){
		textEditPtr->setFolding(CTextEditor::BoxedFoldStyle); 
	}

	OnRetranslate();
}


void CTextEditorComp::OnRetranslate()
{
	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		I_ASSERT(textEditPtr != NULL);
	
		textEditPtr->OnRetranslate();
	}
}


} // namespace iqsci


