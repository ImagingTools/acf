#ifndef CTextEditorComp_included
#define CTextEditorComp_included


// Qt includes
#include <QTextEdit>

#include "imod/TSingleModelObserverBase.h"

#include "idoc/ICommandsProvider.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "IText.h"


class CTextEditorComp:
			public iqtgui::TGuiObserverWrap<iqtgui::TGuiComponentBase<QTextEdit>, imod::TSingleModelObserverBase<IText> >,
			public idoc::ICommandsProvider
{

	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QTextEdit>,
				imod::TSingleModelObserverBase<IText> > BaseClass;

	I_BEGIN_COMPONENT(CTextEditorComp)
		I_REGISTER_INTERFACE(imod::IObserver)
	I_END_COMPONENT

	CTextEditorComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

protected slots:
	void OnSelectionChanged();
	void OnTextChanged();
	void OnToLowercase();
	void OnToUppercase();

protected:
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


