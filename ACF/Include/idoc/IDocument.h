#ifndef idoc_IDocument_included
#define idoc_IDocument_included


#include "idoc/idoc.h"


#include "istd/IChangeable.h"
#include "istd/TIFactory.h"
#include "istd/CString.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"
#include "imod/IUndoManager.h"


namespace idoc
{		


class IDocumentTemplate;

/**
	This class represents a document in the Document/View architecture.
*/
class IDocument: virtual public istd::IChangeable
{
public:
	typedef istd::TIFactory<idoc::IDocument> IDocumentFactory;

	enum ChangeFlags
	{
		TitleChanged = 1
	};

	virtual istd::CString GetFileName() const = 0;
	virtual bool Load(const istd::CString& fileName) = 0;
	virtual bool Save(const istd::CString& fileName) = 0;
	virtual std::string GetDocumentId() const = 0;
	virtual istd::CString GetDocumentTitle() const = 0;
	virtual void SetDocumentTitle(const istd::CString& documentTitle) = 0;
	virtual bool IsModified() const = 0;
	virtual imod::IModel* GetContent() const = 0;
	virtual bool AddView(imod::IObserver* viewPtr) = 0;
	virtual void RemoveView(imod::IObserver* viewPtr) = 0;
	virtual int GetViewCount() = 0;
	virtual imod::IObserver* GetView(int viewIndex) = 0;
	virtual bool HasView(const imod::IObserver* viewPtr) = 0;
	virtual imod::IUndoManager* GetUndoManager() const = 0;
};


} // namespace idoc


#endif // !idoc_IDocument_included