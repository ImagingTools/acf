#ifndef CTextEditorComp_included
#define CTextEditorComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QTextEdit>
#include <QtPrintSupport/QPrinter>
#else
#include <QtGui/QTextEdit>
#include <QtGui/QPrinter>
#endif

#include <imod/TSingleModelObserverBase.h>

#include <ibase/ICommandsProvider.h>

#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TGuiObserverWrap.h>
#include <iqtgui/CHierarchicalCommand.h>

#include <idoc/ITextDocument.h>


class CTextEditorComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QTextEdit>,
						imod::TSingleModelObserverBase<idoc::ITextDocument> >,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QTextEdit>,
				imod::TSingleModelObserverBase<idoc::ITextDocument> > BaseClass;

	I_BEGIN_COMPONENT(CTextEditorComp);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_REGISTER_INTERFACE(imod::IObserver);
	I_END_COMPONENT;

	CTextEditorComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected Q_SLOTS:
	void OnSelectionChanged();
	void OnTextChanged();
	void OnToLowercase();
	void OnToUppercase();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

private:
	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_editorCommand;
	iqtgui::CHierarchicalCommand m_lowercaseCommand;
	iqtgui::CHierarchicalCommand m_uppercaseCommand;
};


#endif // !CTextEditorComp_included


