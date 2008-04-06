#ifndef idoc_CDocumentBase_included
#define idoc_CDocumentBase_included


#include <map>


#include "idoc/IDocument.h"

#include "imod/IModel.h"

#include "istd/IFileEngine.h"
#include "istd/CString.h"
#include "istd/TDelPtr.h"
#include "istd/TPointerVector.h"


namespace idoc
{		


class IDocumentTemplate;


class CDocumentBase: virtual public idoc::IDocument
{
public:
	CDocumentBase();
	virtual ~CDocumentBase();

	virtual void SetDocumentId(const std::string& documentId);
	virtual void SetContent(imod::IModel* modelPtr);
	virtual void SetFileName(const istd::CString& fileName);
	virtual void SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr);
	virtual void RegisterFileEngine(const istd::IFileEngine* fileEnginePtr);

	// reimplemented (idoc::IDocument)
	virtual istd::CString GetFileName() const;
	virtual bool Load(const istd::CString& fileName);
	virtual bool Save(const istd::CString& fileName);
	virtual std::string GetDocumentId() const;
	virtual istd::CString GetDocumentTitle() const;
	virtual void SetDocumentTitle(const istd::CString& documentTitle);
	virtual imod::IModel* GetContent() const;
	virtual bool AddView(imod::IObserver* viewPtr);
	virtual void RemoveView(imod::IObserver* viewPtr);
	virtual int GetViewCount();
	virtual imod::IObserver* GetView(int viewIndex);
	virtual bool HasView(const imod::IObserver* viewPtr);

protected:
	typedef istd::TDelPtr<imod::IModel> ModelPtr;
	typedef imod::IObserver View;
	typedef istd::TPointerVector<View> ViewList;
	ModelPtr m_modelPtr;

	ViewList m_viewList;
	istd::CString m_fileName;
	std::string m_documentId;
	istd::CString m_title;
	const istd::IFileEngine* m_fileEnginePtr;

private:
	const idoc::IDocumentTemplate* m_documentTemplatePtr;
};


} // namespace idoc


#endif // !idoc_CDocumentBase_included

