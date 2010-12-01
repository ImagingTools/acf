#include "CTextEditorComp.h"


#include "iqt/CSignalBlocker.h"


CTextEditorComp::CTextEditorComp()
:	m_editorCommand("&Text"),
	m_lowercaseCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_uppercaseCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR)
{
	m_lowercaseCommand.SetEnabled(false);
	m_uppercaseCommand.SetEnabled(false);
	m_editorCommand.InsertChild(&m_lowercaseCommand, false);
	m_editorCommand.InsertChild(&m_uppercaseCommand, false);
	m_rootCommand.InsertChild(&m_editorCommand, false);

	connect(&m_lowercaseCommand, SIGNAL(activated()), this, SLOT(OnToLowercase()));
	connect(&m_uppercaseCommand, SIGNAL(activated()), this, SLOT(OnToUppercase()));
}


// reimplemented (imod::IModelEditor)

void CTextEditorComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	ibase::ITextDocument* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	objectPtr->SetText(iqt::GetCString(textEditPtr->toPlainText()));
}


void CTextEditorComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	ibase::ITextDocument* objectPtr = GetObjectPtr();

	if ((objectPtr != NULL) && !textEditPtr->signalsBlocked()){
		textEditPtr->setText(iqt::GetQString(objectPtr->GetText()));
	}
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CTextEditorComp::GetCommands() const
{
	return &m_rootCommand;
}


// protected slots

void CTextEditorComp::OnSelectionChanged()
{
	bool isTextSelected = false;

	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	QTextCursor cursor = textEditPtr->textCursor();
	isTextSelected = cursor.hasSelection();

	m_lowercaseCommand.SetEnabled(isTextSelected);
	m_uppercaseCommand.SetEnabled(isTextSelected);
}


void CTextEditorComp::OnTextChanged()
{
	UpdateModel();
}


void CTextEditorComp::OnToLowercase()
{
	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	QTextCursor cursor = textEditPtr->textCursor();
	QString selectedText = cursor.selectedText();
	cursor.removeSelectedText();
	cursor.insertText(selectedText.toLower());

	OnSelectionChanged();
	UpdateModel();
}


void CTextEditorComp::OnToUppercase()
{
	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	iqt::CSignalBlocker block(textEditPtr);

	QTextCursor cursor = textEditPtr->textCursor();
	QString selectedText = cursor.selectedText();
	cursor.removeSelectedText();
	cursor.insertText(selectedText.toUpper());

	OnSelectionChanged();
	UpdateModel();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTextEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QTextEdit* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	connect(textEditPtr, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
	connect(textEditPtr, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));

	OnRetranslate();
}


void CTextEditorComp::OnRetranslate()
{
	m_lowercaseCommand.SetVisuals(tr("To &Lowercase"), tr("Lowercase"), tr("Convert selected block to lowercase characters"), QIcon(":/Icons/Down"));
	m_uppercaseCommand.SetVisuals(tr("To &Uppercase"), tr("Uppercase"), tr("Convert selected block to uppercase characters"), QIcon(":/Icons/Up"));
}


