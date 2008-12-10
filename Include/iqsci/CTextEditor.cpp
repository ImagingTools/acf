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
	m_editorCommand("&Edit"),
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
	connect(this, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));
	
	setLexer(new QsciLexerCPP(this));
	setFolding(BoxedFoldStyle); 
	setAutoIndent(true);
	setIndentationGuides(true);
}


void CTextEditor::OnRetranslate()
{
	m_lowercaseCommand.SetVisuals(tr("To &Lowercase"), tr("Lowercase"), tr("Convert selected block to lowercase characters"), QIcon(":/Icons/down"));
	m_uppercaseCommand.SetVisuals(tr("To &Uppercase"), tr("Uppercase"), tr("Convert selected block to uppercase characters"), QIcon(":/Icons/up"));
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
	int line2;
	int index2;
	getSelection(&line, &index, &line2, &index2);

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
	int line2;
	int index2;
	getSelection(&line, &index, &line2, &index2);

	removeSelectedText();

	insertAt(selectedText.toUpper(), line, index);

	OnSelectionChanged();
	OnTextChanged();
}


}


