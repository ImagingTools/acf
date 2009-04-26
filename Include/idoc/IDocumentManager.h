#ifndef idoc_IDocumentManager_included
#define idoc_IDocumentManager_included


#include <string>
#include <map>

#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{


/**
	Provide set of user actions needed to manage documents in MVC (Model View Controller) concept.
*/
class IDocumentManager: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		DocumentRenamed = 0x10,
		DocumentCreated = 0x20,
		DocumentRemoved = 0x40,
		DocumentCountChanged = 0x80,
		ViewActivationChanged = 0x100
	};

	struct DocumentInfo
	{
		istd::CString filePath;
		std::string documentTypeId;
		bool isDirty;
	};

	/**
		Map from file path to ducument type ID's.
	*/
	typedef std::map<istd::CString, std::string> FileToTypeMap;

	/**
		Return main document template used by this manager.
	*/
	virtual const idoc::IDocumentTemplate* GetDocumentTemplate() const = 0;

	/**
		Return undo mananger for document \c documenPtr.
	*/
	virtual imod::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const = 0;

	/**
		Get number of opened documents.
	*/
	virtual int GetDocumentsCount() const = 0;

	/**
		Get document at specified index.
		\param	index			index of document.
		\param	documentInfoPtr	optional return value.
	*/
	virtual istd::IChangeable& GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr) const = 0;

	/**
		Get number of view for specified document.
	*/
	virtual int GetViewsCount(int documentIndex) const = 0;

	/**
		Get single view using its and document indices.
	*/
	virtual istd::IPolymorphic* GetViewFromIndex(int documentIndex, int viewIndex) const = 0;

	/**
		Return the active document. 
		If no document is active, the function return a \c NULL
	*/
	virtual istd::IPolymorphic* GetActiveView() const = 0;

	/**
		Return the document assigned to view. 
		\param	view	view object.
		\return			pointer to assigned document, or \c NULL if no document for this view exists.
	*/
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view) const = 0;

	/**
		Get ID of document type managed by this object.
		\return	ID of document type or empty string if no document is found.
	*/
	virtual std::string GetDocumentTypeId(const istd::IChangeable& document) const = 0;

	/**
		Creates a new document with the document ID \c documentTypeId.
		\param	documentTypeId	ID of document type.
		\param	createView		if true, view will be automatically created.
		\param	viewTypeId		ID of view type, if it will be created.
		\param	newDocumentPtr	optional pointer where new created document will be returned.
		\return	true, if success.
	*/
	virtual bool FileNew(
				const std::string& documentTypeId, 
				bool createView = true, 
				const std::string& viewTypeId = "",
				istd::IChangeable** newDocumentPtr = NULL) = 0;

	/**
		Opens document(s) from the file list. 
		\param	documentTypeIdPtr	optional ID of document type. If it is NULL, document type will be found automatically.
		\param	fileNamePtr			file name.
		\param	createView			if true, view will be automatically created.
		\param	viewTypeId			ID of view type, if it will be created.
		\param	loadedMapPtr		optional list of loaded files and its document type ID's.
	*/
	virtual bool FileOpen(
				const std::string* documentTypeIdPtr = NULL,
				const istd::CString* fileNamePtr = NULL,
				bool createView = true,
				const std::string& viewTypeId = "",
				FileToTypeMap* loadedMapPtr = NULL) = 0;

	/**
		Save active document. 
	*/
	virtual bool FileSave(
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL) = 0;

	/**
		Close current view.
		\param	ignoredPtr	optional return flag indicating that closing was aborted by user.
	*/
	virtual void FileClose(bool* ignoredPtr = NULL) = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentManager_included


