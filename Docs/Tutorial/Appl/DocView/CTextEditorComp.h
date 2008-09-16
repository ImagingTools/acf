#ifndef CTextEditorComp_included
#define CTextEditorComp_included


// Qt includes
#include <QTextEdit>

#include "imod/TSingleModelObserverBase.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiObserverWrap.h"
#include "iqt/CHierarchicalCommand.h"

#include "IText.h"


class CTextEditorComp:
			public iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QTextEdit>, imod::TSingleModelObserverBase<IText> >,
			public idoc::ICommandsProvider
{

	Q_OBJECT

public:
	typedef iqt::TGuiObserverWrap<
				iqt::TGuiComponentBase<QTextEdit>,
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
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

private:
	iqt::CHierarchicalCommand m_rootCommand;
	iqt::CHierarchicalCommand m_editorCommand;
	iqt::CHierarchicalCommand m_lowercaseCommand;
	iqt::CHierarchicalCommand m_uppercaseCommand;
};


#endif // !CTextEditorComp_included


