#ifndef CTextEditor_included
#define CTextEditor_included




#include <QTextEdit>

#include "imod/TModelEditorBase.h"


#include "CTextModel.h"


class CTextEditor:	public QTextEdit, 
					public imod::TModelEditorBase<CTextModel>
{

	Q_OBJECT

public:
	CTextEditor();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel();
	virtual void UpdateEditor();

protected slots:
	 void OnTextChanged();
};


#endif // !CTextEditor_included