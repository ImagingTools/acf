#include "iqsci/CTextEditorGuiComp.h"


// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqsci
{


// reimplemented (imod::IModelEditor)

void CTextEditorGuiComp::UpdateModel() const
{
	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	ibase::ITextDocument* objectPtr = GetObjectPtr();

	if (objectPtr != NULL ){
		objectPtr->SetText(iqt::GetCString(textEditPtr->GetText()));
	}
}


void CTextEditorGuiComp::UpdateEditor(int /*updateFlags*/)
{
	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	ibase::ITextDocument* objectPtr = GetObjectPtr();

	if ((objectPtr != NULL) && !textEditPtr->signalsBlocked()){
		textEditPtr->SetText(iqt::GetQString(objectPtr->GetText()));
	}
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CTextEditorGuiComp::GetCommands() const
{
	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		I_ASSERT(textEditPtr != NULL);
		
		return textEditPtr->GetCommands();
	}

	return NULL;
}



// protected slots

void CTextEditorGuiComp::OnTextChanged()
{
	iqt::CSignalBlocker block(this);

	UpdateModel();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTextEditorGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CTextEditor* textEditorPtr = GetQtWidget();
	I_ASSERT(textEditorPtr != NULL);
	if (textEditorPtr != NULL){
		connect(textEditorPtr, SIGNAL(DataChanged()), this, SLOT(OnTextChanged()));

		if (m_useFoldingAttrPtr.IsValid()){
			textEditorPtr->SetFoldingEnabled(*m_useFoldingAttrPtr); 
		}

		if (m_languageAttrPtr.IsValid()){
			textEditorPtr->SetLanguage(iqt::GetQString(*m_languageAttrPtr));
		}

		OnRetranslate();
	}
}


void CTextEditorGuiComp::OnRetranslate()
{
	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		I_ASSERT(textEditPtr != NULL);
	
		textEditPtr->OnRetranslate();
	}
}


} // namespace iqsci


