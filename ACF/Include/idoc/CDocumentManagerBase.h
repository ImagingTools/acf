#ifndef idoc_CDocumentManagerBase_included
#define idoc_CDocumentManagerBase_included


// STL includes
#include <map>
#include <list>

#include "istd/TIFactory.h"
#include "istd/TPointerVector.h"
#include "istd/TDelPtr.h"

#include "imod/TMultiModelObserverBase.h"
#include "imod/IModel.h"

#include "idoc/IDocumentManager.h"


namespace idoc
{


class IDocumentTemplate;


class CDocumentManagerBase: public idoc::IDocumentManager,
							public imod::TMultiModelObserverBase<imod::IModel>
{
public:
	typedef imod::TMultiModelObserverBase<imod::IModel> BaseClass;

	CDocumentManagerBase();

	virtual istd::IChangeable* OpenDocument(const istd::CString& filePath, bool createView, const std::string& viewTypeId);

	void SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr);

	// reimplemented (idoc::IDocumentManager)
	virtual const idoc::IDocumentTemplate* GetDocumentTemplate() const;
	imod::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const;
	virtual int GetDocumentsCount() const;
	virtual istd::IChangeable& GetDocumentFromIndex(int index) const;
	virtual istd::IPolymorphic* GetActiveView() const;
	virtual void SetActiveView(istd::IPolymorphic* viewPtr);
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view) const;
	virtual std::string GetDocumentTypeId(const istd::IChangeable& document) const;
	virtual istd::IChangeable* FileNew(const std::string& documentTypeId, bool createView = true, const std::string& viewTypeId = "");
	virtual bool FileOpen(const std::string* documentTypeIdPtr, bool createView = true, const std::string& viewTypeId = "");
	virtual bool FileSave(bool requestFileName = false);
	virtual bool FileClose();

protected:
	typedef istd::TDelPtr<istd::IChangeable> DocumentPtr;
	typedef istd::TDelPtr<imod::IUndoManager> UndoManagerPtr;
	typedef istd::TDelPtr<istd::IPolymorphic> ViewPtr;
	typedef std::list<ViewPtr> Views;

	struct DocumentInfo
	{
		DocumentInfo(){}
		DocumentInfo(const istd::CString& filePath, istd::IChangeable* documentPtr, imod::IUndoManager* undoManagerPtr)
		{
			this->filePath = filePath;
			this->documentPtr.SetPtr(documentPtr);
			this->undoManagerPtr.SetPtr(undoManagerPtr);
		}

		istd::CString filePath;
		std::string documentTypeId;
		DocumentPtr documentPtr;
		Views views;
		UndoManagerPtr undoManagerPtr;
	};

	void CloseAllDocuments();

	DocumentInfo& GetDocumentInfo(int index) const;

	/**
		Get document info assigned to active view.
	*/
	DocumentInfo* GetActiveDocumentInfo() const;

	/**
		Get document info assigned to specified view.
	*/
	DocumentInfo* GetDocumentInfoFromView(const istd::IPolymorphic& view) const;

	/**
		Get document info assigned to specified file.
	*/
	DocumentInfo* GetDocumentInfoFromPath(const istd::CString& filePath) const;

	/**
		Create instance of specified document without attaching to this manager.
	*/
	DocumentInfo* CreateDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId) const;

	/**
		Register (attach) created document as new working document.
	*/
	bool RegisterDocument(DocumentInfo* documentPtr);

	// abstract methods
	
	/**
		Gets open file names.
	*/
	virtual istd::CStringList GetOpenFileNames(const std::string* documentTypeIdPtr = NULL) const  = 0;

	/**
		Gets save file name.
	*/
	virtual istd::CString GetSaveFileName(const std::string& documentTypeId) const = 0;

	/**
		Called during view is registered.
	*/
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr) = 0;

private:
	typedef istd::TPointerVector<DocumentInfo> DocumentInfos;

	const IDocumentTemplate* m_documentTemplatePtr;
	DocumentInfos m_documentInfos;

	istd::IPolymorphic* m_activeViewPtr;
};


} // namespace idoc


#endif // idoc_CDocumentManagerBase_included