#ifndef CTextEditor_included
#define CTextEditor_included


// Qt includes
#include <QTextEdit>

#include "imod/TModelEditorBase.h"

#include "iqt/TGuiComponentBase.h"

#include "CTextModel.h"


class CTextEditor:	public iqt::TGuiComponentBase<QTextEdit>, 
					public imod::TModelEditorBase<CTextModel>
{

	Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QTextEdit> BaseClass;
	typedef imod::TModelEditorBase<CTextModel> BaseClass2;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel();
	virtual void UpdateEditor();

protected slots:
	 void OnTextChanged();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


#endif // !CTextEditor_included