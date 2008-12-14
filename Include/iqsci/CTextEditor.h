#ifndef iqsci_CTextEditor_included
#define iqsci_CTextEditor_included


// ACF includes
#include "idoc/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"


// Project includes

#include "iqsci/generated/ui_CTextEditor.h"


class QsciScintilla;


namespace iqsci
{


class CTextEditor: public QWidget, public Ui::CTextEditor, virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef QWidget BaseClass;

	CTextEditor(QWidget* parentWidget = NULL);

	virtual QString GetText() const;
	virtual void SetText(const QString& text);

	virtual void OnRetranslate();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

public slots:
	virtual void SetFoldingEnabled(bool useFoldingEnabled);
	virtual void SetLineNumberEnabled(bool useFoldingEnabled);

protected slots:
	virtual void OnSelectionChanged();
	virtual void OnTextChanged();
	virtual void OnToLowercase();
	virtual void OnToUppercase();

signals:
	void DataChanged();

private:
	enum MenuFlags
	{
		MF_VIEW = 0x9977,
		MF_EDIT
	};

	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_editorCommand;
	iqtgui::CHierarchicalCommand m_viewCommand;
	iqtgui::CHierarchicalCommand m_languageCommand;
	iqtgui::CHierarchicalCommand m_lowercaseCommand;
	iqtgui::CHierarchicalCommand m_uppercaseCommand;

	// view commands
	iqtgui::CHierarchicalCommand m_useIdentGuideCommand;
	iqtgui::CHierarchicalCommand m_useFoldingCommand;
	iqtgui::CHierarchicalCommand m_showLineNumberCommand;

	QsciScintilla* m_scintilla;
};


} // namespace iqsci


#endif // !iqsci_CTextEditor_included



