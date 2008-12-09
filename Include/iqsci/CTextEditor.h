#ifndef iqsci_CTextEditor_included
#define iqsci_CTextEditor_included


// QScinitlla includes
#include <Qsci/QsciScintilla.h>


// ACF includes
#include "idoc/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"


namespace iqsci
{


class CTextEditor: public QsciScintilla, virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef QsciScintilla BaseClass;

	CTextEditor(QWidget* parentWidget = NULL);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

protected slots:
	virtual void OnSelectionChanged();
	virtual void OnTextChanged();
	virtual void OnToLowercase();
	virtual void OnToUppercase();

signals:
	void DataChanged();

private:
	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_editorCommand;
	iqtgui::CHierarchicalCommand m_lowercaseCommand;
	iqtgui::CHierarchicalCommand m_uppercaseCommand;
};


} // namespace iqsci



#endif // !iqsci_CTextEditor_included




