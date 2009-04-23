#include "CTextEditor.h"


// QScinitlla includes
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpostscript.h>


// Qt includes
#include <QTextEdit>
#include <QApplication>


// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqsci
{


CTextEditor::CTextEditor(QWidget* parentWidget/* = NULL*/)
:	QWidget(parentWidget),
	m_editorCommand("&Edit"),
	m_viewCommand("&View"),
	m_languageCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU),
	m_lowercaseCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR, MF_EDIT),
	m_uppercaseCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_TOOLBAR, MF_EDIT),
	m_useIdentGuideCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF, MF_VIEW),
	m_useFoldingCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF, MF_VIEW),
	m_showLineNumberCommand("", 100, idoc::ICommand::CF_GLOBAL_MENU | idoc::ICommand::CF_ONOFF, MF_VIEW)
{
	Ui_CTextEditor::setupUi(this);

	m_scintilla = new QsciScintilla(EditorFrame);
	QLayout* editorLayout = EditorFrame->layout();
	if (editorLayout == NULL){
		editorLayout = new QVBoxLayout(EditorFrame);

		editorLayout->addWidget(m_scintilla);
		editorLayout->setMargin(0);
		m_scintilla->setFrameStyle(QFrame::NoFrame);
	}

	m_scintilla->setUtf8(true);
	m_scintilla->setMarginWidth(1, 0);

	m_lowercaseCommand.SetEnabled(false);
	m_uppercaseCommand.SetEnabled(false);
	m_editorCommand.InsertChild(&m_lowercaseCommand, false);
	m_editorCommand.InsertChild(&m_uppercaseCommand, false);
	m_rootCommand.InsertChild(&m_editorCommand, false);
	m_rootCommand.InsertChild(&m_viewCommand, false);
	m_rootCommand.InsertChild(&m_languageCommand, false);
	
	connect(&m_lowercaseCommand, SIGNAL(activated()), this, SLOT(OnToLowercase()));
	connect(&m_uppercaseCommand, SIGNAL(activated()), this, SLOT(OnToUppercase()));
	connect(m_scintilla, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
	connect(m_scintilla, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));
	
	RegisterLexers();

	// add view commands
	m_viewCommand.InsertChild(&m_useIdentGuideCommand, false);
	m_viewCommand.InsertChild(&m_useFoldingCommand, false);
	m_viewCommand.InsertChild(&m_showLineNumberCommand, false);

	connect(&m_useIdentGuideCommand, SIGNAL(toggled(bool)), m_scintilla, SLOT(setIndentationGuides(bool)));
	connect(&m_useFoldingCommand, SIGNAL(toggled(bool)), this, SLOT(SetFoldingEnabled(bool)));
	connect(&m_showLineNumberCommand, SIGNAL(toggled(bool)), this, SLOT(SetLineNumberEnabled(bool)));

	// some additional settings:
	m_scintilla->setBraceMatching(QsciScintilla::StrictBraceMatch);
}


QString CTextEditor::GetText() const
{
	I_ASSERT(m_scintilla != NULL);

	return m_scintilla->text();
}


void CTextEditor::SetText(const QString& text)
{
	I_ASSERT(m_scintilla != NULL);

	m_scintilla->setText(text);
}



void CTextEditor::OnRetranslate()
{
	m_lowercaseCommand.SetVisuals(tr("To &Lowercase"), tr("Lowercase"), tr("Convert selected block to lowercase characters"), QIcon(":/Icons/down"));
	m_uppercaseCommand.SetVisuals(tr("To &Uppercase"), tr("Uppercase"), tr("Convert selected block to uppercase characters"), QIcon(":/Icons/up"));
	m_languageCommand.SetVisuals(tr("&Language"), tr("Language"), tr("Select language for syntax highlighting"), QIcon(":/Icons/"));
	m_useIdentGuideCommand.SetVisuals(tr("Show &Indentation Guides"), tr("Show Indentation Guides"), tr("Show indentation guides"));
	m_useFoldingCommand.SetVisuals(tr("Use &Folding"), tr("Use Folding"), tr("Use folding"));
	m_showLineNumberCommand.SetVisuals(tr("Show &Line Number"), tr("Show Line Number"), tr("Show current line number"));
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CTextEditor::GetCommands() const
{
	return &m_rootCommand;
}


// public slots

void CTextEditor::SetFoldingEnabled(bool useFoldingEnabled)
{
	I_ASSERT(m_scintilla != NULL);

	if (useFoldingEnabled){
		m_scintilla->setFolding(QsciScintilla::BoxedTreeFoldStyle); 
	}
	else{
		m_scintilla->setFolding(QsciScintilla::NoFoldStyle); 
	}
}


void CTextEditor::SetLineNumberEnabled(bool showLineNumber)
{
	I_ASSERT(m_scintilla != NULL);

	m_scintilla->setMarginLineNumbers(1, showLineNumber);
}


void CTextEditor::SetLanguage(const QString& language)
{
	I_ASSERT(m_scintilla != NULL);

	LexerMap::iterator foundIt = m_languages.find(language.toUpper());
	if (foundIt != m_languages.end()){
		m_scintilla->setLexer(foundIt->second);
	}
}


// protected slots

void CTextEditor::OnSelectionChanged()
{
	bool isTextSelected = m_scintilla->hasSelectedText();

	m_lowercaseCommand.SetEnabled(isTextSelected);
	m_uppercaseCommand.SetEnabled(isTextSelected);
}


void CTextEditor::OnTextChanged()
{
	emit DataChanged();

	QString linesString = QString("%1").arg(m_scintilla->lines());
	I_ASSERT(m_scintilla != NULL);

	if (m_scintilla->marginLineNumbers(1)){
		m_scintilla->setMarginWidth(1, linesString);
		m_scintilla->setMarginWidth(1, istd::Max(20, m_scintilla->marginWidth(1)));
	}
}


void CTextEditor::OnToLowercase()
{
	I_ASSERT(m_scintilla != NULL);

	QString selectedText = m_scintilla->selectedText();

	int line;
	int index;
	int line2;
	int index2;
	m_scintilla->getSelection(&line, &index, &line2, &index2);

	m_scintilla->removeSelectedText();

	m_scintilla->insertAt(selectedText.toLower(), line, index);

	OnSelectionChanged();
	OnTextChanged();
}


void CTextEditor::OnToUppercase()
{
	I_ASSERT(m_scintilla != NULL);

	QString selectedText = m_scintilla->selectedText();

	int line;
	int index;
	int line2;
	int index2;
	m_scintilla->getSelection(&line, &index, &line2, &index2);

	m_scintilla->removeSelectedText();

	m_scintilla->insertAt(selectedText.toUpper(), line, index);

	OnSelectionChanged();

	OnTextChanged();
}


// private methods

void CTextEditor::RegisterLexers()
{
	QsciLexerCPP* cppLexerPtr = new QsciLexerCPP(this);
	cppLexerPtr->setDefaultFont(QFont("Courier", 10));
	cppLexerPtr->setFoldCompact(true);
	m_languages[QString(cppLexerPtr->language()).toUpper()] = cppLexerPtr;

	QsciLexerPostScript* psLexerPtr = new QsciLexerPostScript(this);
	m_languages[QString(psLexerPtr->language()).toUpper()] = psLexerPtr;
}


}


