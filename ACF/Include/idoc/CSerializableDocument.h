#ifndef CSerializableDocument_included
#define CSerializableDocument_included


#include "imod/CSerializedUndoManager.h"

#include "idoc/CDocumentBase.h"


namespace idoc
{		


class CSerializableDocument: public idoc::CDocumentBase
{
public:
	typedef CDocumentBase BaseClass;
	
	// reimplemented (idoc::CDocument)
	virtual void SetContent(imod::IModel* modelPtr);

	// reimplemented (idoc::IDocument)
	virtual bool IsModified() const;
	virtual imod::IUndoManager* GetUndoManager() const;

private:
	imod::CSerializedUndoManager m_undoManager;
};


} // namespace idoc


#endif // !CSerializableDocument_included

