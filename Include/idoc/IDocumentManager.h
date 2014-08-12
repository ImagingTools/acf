#ifndef idoc_IDocumentManager_included
#define idoc_IDocumentManager_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IChangeable.h"
#include "idoc/IUndoManager.h"
#include "idoc/IDocumentTypesInfo.h"


namespace idoc
{


/**
	Provide set of user actions needed to manage documents in MVC (Model View Controller) concept.
*/
class IDocumentManager:
			virtual public istd::IChangeable,
			virtual public IDocumentTypesInfo
{
public:
	/**
		Possible changes in the mananger data model.
	*/
	enum ChangeFlags
	{
		CF_DOCUMENT_RENAMED = 0x834d670,
		CF_DOCUMENT_CREATED,
		CF_DOCUMENT_REMOVED,
		CF_DOCUMENT_COUNT_CHANGED,
		CF_VIEW_ACTIVATION_CHANGED
	};

	struct DocumentInfo
	{
		QString filePath;
		QByteArray documentTypeId;
		bool isDirty;
	};

	/**
		Flags describing possible operations.
	*/
	enum OperationFlags
	{
		OF_FILE_NEW = 0x0001,
		OF_FILE_OPEN = 0x0002,
		OF_FILE_SAVE = 0x0004,
		OF_FILE_SAVE_AS = 0x0008,
		OF_KNOWN_PATH = 0x0010
	};

	/**
		Map from file path to ducument type ID's.
	*/
	typedef QMap<QString, QByteArray> FileToTypeMap;

	/**
		Get flags of allowed operations.
	*/
	virtual int GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr = NULL) const = 0;

	/**
		Return undo mananger for document \c documenPtr.
	*/
	virtual idoc::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const = 0;

	/**
		Get number of opened documents.
	*/
	virtual int GetDocumentsCount() const = 0;

	/**
		Get document at specified index.
		\param	index			index of document.
		\param	documentInfoPtr	optional return value describing some additional informations about document.
	*/
	virtual istd::IChangeable& GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr = NULL) const = 0;

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
		\param	view			view object.
		\param	documentInfoPtr	optional return value describing some additional informations about document.
		\return					pointer to assigned document, or \c NULL if no document for this view exists.
	*/
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr = NULL) const = 0;

	/**
		Add a new view to the document.
		\param	document		document instance
		\param	viewTypeId		ID of view type, if it will be created.
		\return					Created view object
	*/
	virtual istd::IPolymorphic* AddViewToDocument(const istd::IChangeable& document, const QByteArray& viewTypeId = QByteArray()) = 0;

	/**
		Get ID of document type managed by this object.
		\return	ID of document type or empty string if no document is found.
	*/
	virtual QByteArray GetDocumentTypeId(const istd::IChangeable& document) const = 0;

	/**
		Creates a new document with the document ID \c documentTypeId.
		\param	documentTypeId	ID of document type.
		\param	createView		if true, view will be automatically created.
		\param	viewTypeId		ID of view type, if it will be created.
		\param	newDocumentPtr	optional pointer where new created document will be returned.
		\param	beQuiet			if true, no user interaction will be done.
		\param	ignoredPtr		optional return flag indicating that creating was aborted by user.
		\return	true, if success.
	*/
	virtual bool InsertNewDocument(
				const QByteArray& documentTypeId, 
				bool createView = true, 
				const QByteArray& viewTypeId = "",
				istd::IChangeable** newDocumentPtr = NULL,
				bool beQuiet = false,
				bool* ignoredPtr = NULL) = 0;

	/**
		Opens document(s) from the file list.
		\param	documentTypeIdPtr	optional ID of document type. If it is NULL, document type will be found automatically.
		\param	fileNamePtr			file name.
		\param	createView			if true, view will be automatically created.
		\param	viewTypeId			ID of view type, if it will be created.
		\param	loadedMapPtr		optional list of loaded files and its document type ID's.
		\param	beQuiet			if true, no user interaction will be done.
		\param	ignoredPtr			optional return flag indicating that opening was aborted by user.
	*/
	virtual bool OpenDocument(
				const QByteArray* documentTypeIdPtr = NULL,
				const QString* fileNamePtr = NULL,
				bool createView = true,
				const QByteArray& viewTypeId = "",
				istd::IChangeable** documentPtr = NULL,
				FileToTypeMap* loadedMapPtr = NULL,
				bool beQuiet = false,
				bool* ignoredPtr = NULL) = 0;

	/**
		Save document.
		\param	documentIndex	optional index of document to save, if negative then current active document will be saved.
		\param	requestFileName	if true, file name will be always requested by user.
		\param	savedMapPtr		optional list of files will be extended with currently saved file.
		\param	beQuiet			if true, no user interaction will be done.
		\param	ignoredPtr		optional return flag indicating that saving was aborted by user.
	*/
	virtual bool SaveDocument(
				int documentIndex = -1,
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL,
				bool beQuiet = false,
				bool* ignoredPtr = NULL) = 0;

	/**
		Ask user (optional) and save all dirty (changed) documents.
		\param	beQuiet			if true, no user interaction will be done.
								Typically user will be asked for saving document.
		\param	ignoredPtr		optional return flag indicating that process was aborted by user.
		\return	true if all documents were saved and are not dirty now.
	*/
	virtual bool SaveDirtyDocuments(bool beQuiet = false, bool* ignoredPtr = NULL) = 0;

	/**
		Close document and all its views.
		\param	documentIndex	optional index of document to close, if negative then current active document will be closed.
		\param	beQuiet			if true, no user interaction will be done.
								Typically user will be asked for saving document.
		\param	ignoredPtr		optional return flag indicating that closing was aborted by user.
	*/
	virtual bool CloseDocument(int documentIndex = -1, bool beQuiet = false, bool* ignoredPtr = NULL) = 0;

	/**
		Close current view.
		\param	beQuiet			if true, no user interaction will be done.
								Typically user will be asked for saving document.
		\param	ignoredPtr		optional return flag indicating that closing was aborted by user.
	*/
	virtual bool CloseCurrentView(bool beQuiet = false, bool* ignoredPtr = NULL) = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentManager_included


