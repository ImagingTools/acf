#ifndef CTextEditor_included
#define CTextEditor_included


// Qt includes
#include <QTextEdit>

#include "imod/TSingleModelObserverBase.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiModelJoinerWrap.h"

#include "CTextModel.h"


class CTextEditor: public iqt::TGuiModelJoinerWrap<
			iqt::TGuiComponentBase<QTextEdit>,
			imod::TSingleModelObserverBase<CTextModel> >
{

	Q_OBJECT

public:
	typedef iqt::TGuiModelJoinerWrap<
				iqt::TGuiComponentBase<QTextEdit>,
				imod::TSingleModelObserverBase<CTextModel> > BaseClass;

	// reimplemented (iqt::TGuiModelJoinerWrap)
	virtual void UpdateModel();
	virtual void UpdateGui();

protected slots:
	 void OnTextChanged();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


#endif // !CTextEditor_included