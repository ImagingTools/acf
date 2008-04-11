#ifndef CTextEditorComp_included
#define CTextEditorComp_included


// Qt includes
#include <QTextEdit>

#include "imod/TSingleModelObserverBase.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiObserverWrap.h"

#include "IText.h"


class CTextEditorComp: public iqt::TGuiObserverWrap<
			iqt::TGuiComponentBase<QTextEdit>,
			imod::TSingleModelObserverBase<IText> >
{

	Q_OBJECT

public:
	typedef iqt::TGuiObserverWrap<
				iqt::TGuiComponentBase<QTextEdit>,
				imod::TSingleModelObserverBase<IText> > BaseClass;

	I_BEGIN_COMPONENT(CTextEditorComp)
	I_END_COMPONENT

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	 void OnTextChanged();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


#endif // !CTextEditorComp_included