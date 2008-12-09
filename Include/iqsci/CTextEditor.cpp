#include "CTextEditor.h"


// Qt includes
#include <QTextEdit>


// ACF includes
#include "iqt/CSignalBlocker.h"


// QScinitlla includes
#include <Qsci/qscilexercpp.h>


namespace iqsci
{


CTextEditor::CTextEditor(QWidget* parentWidget/* = NULL*/)
:	BaseClass(parentWidget),
	m_editorCommand("&Text"),
	m_lowercaseCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR),
	m_uppercaseCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR)
{
	m_lowercaseCommand.SetEnabled(false);
	m_uppercaseCommand.SetEnabled(false);
	m_editorCommand.InsertChild(&m_lowercaseCommand, false);
	m_editorCommand.InsertChild(&m_uppercaseCommand, false);
	m_rootCommand.InsertChild(&m_editorCommand, false);

	connect(&m_lowercaseCommand, SIGNAL(activated()), this, SLOT(OnToLowercase()));
	connect(&m_uppercaseCommand, SIGNAL(activated()), this, SLOT(OnToUppercase()));
	connect(this, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
	
	setLexer(new QsciLexerCPP(this));
	setFolding(BoxedFoldStyle); 
	setAutoIndent(true);
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CTextEditor::GetCommands() const
{
	return &m_rootCommand;
}


// protected slots

void CTextEditor::OnSelectionChanged()
{
	bool isTextSelected = hasSelectedText();

	m_lowercaseCommand.SetEnabled(isTextSelected);
	m_uppercaseCommand.SetEnabled(isTextSelected);
}


void CTextEditor::OnTextChanged()
{
	emit DataChanged();
}


void CTextEditor::OnToLowercase()
{
	QString selectedText = this->selectedText();

	int line;
	int index;
	getSelection(&line, &index, NULL, NULL);

	removeSelectedText();

	insertAt(selectedText.toLower(), line, index);

	OnSelectionChanged();
	OnTextChanged();
}


void CTextEditor::OnToUppercase()
{
	QString selectedText = this->selectedText();

	int line;
	int index;
	getSelection(&line, &index, NULL, NULL);

	removeSelectedText();

	insertAt(selectedText.toUpper(), line, index);

	OnSelectionChanged();
	OnTextChanged();
}


}


