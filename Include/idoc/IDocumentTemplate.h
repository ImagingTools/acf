#ifndef idoc_IDocumentTemplate_included
#define idoc_IDocumentTemplate_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IChangeable.h"
#include "ifile/IFilePersistence.h"
#include "imod/IObserver.h"
#include "ibase/ICommand.h"
#include "idoc/IUndoManager.h"
#include "idoc/IDocumentTypesInfo.h"


namespace idoc
{


/**
	Common interface for a document template.
	
	Document template provides all necessary functionality for creating and controling data model of a document, its persistence and its representation (view).
*/
class IDocumentTemplate: virtual public IDocumentTypesInfo
{
public:
	/**
		Return supported view type IDs for specified document type.
		\param	documentTypeId	ID of document type.
	*/
	virtual Ids GetViewTypeIds(const QByteArray& documentTypeId) const = 0;

	/**
		Get human readable name of some view type ID.
	*/
	virtual QString GetViewTypeName(
				const QByteArray& documentTypeId,
				const QByteArray& viewTypeId) const = 0;

	/**
		Get file loader/saver for spacified document ID.
		\param	documentTypeId	type ID of document will be loaded/saved.
	*/
	virtual ifile::IFilePersistence* GetFileLoader(const QByteArray& documentTypeId) const = 0;

	/**
		Creates a document instance for document type \c documentTypeId.
		\param	documentTypeId	ID of document type, can be empty for default document.
								If document type is undefined (empty), this field will be used to return ID of used type.
		\param	initialize		If true, document should be filled with initial data (operation new),
								otherwise initialization is not needed (e.g. for document loading - filling of data will be done by loader).
	*/
	virtual istd::IChangeable* CreateDocument(QByteArray& documentTypeId, bool initialize = true) const = 0;

	/**
		Creates a view instance for document \c document of type \c viewTypeId.
		\param	documentTypeId	ID of document type.
		\param	documentPtr		pointer to document object.
		\param	viewTypeId		optional ID specifying view type if more view types are supported.
	*/
	virtual istd::IPolymorphic* CreateView(
				const QByteArray& documentTypeId,
				istd::IChangeable* documentPtr,
				const QByteArray& viewTypeId = QByteArray()) const = 0;

	/**
		Creates an undo manger for a given document \c documentPtr.
		\param	documentTypeId	ID of document type.
		\param	documentPtr	pointer to document object.
		\return	pointer to created instance of undo manager or NULL, if creating of undo manager was impossible.
	*/
	virtual idoc::IUndoManager* CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTemplate_included


