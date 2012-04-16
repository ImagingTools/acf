#ifndef idoc_IDocumentTemplate_included
#define idoc_IDocumentTemplate_included


// ACF includes
#include "istd/IChangeable.h"
#include <QtCore/QString>

#include "iser/IFileLoader.h"

#include "imod/IObserver.h"

#include "ibase/ICommand.h"

#include "idoc/IUndoManager.h"
#include "idoc/IDocumentTypesInfo.h"
#include "idoc/IDocumentStateComparator.h"


namespace idoc
{


/**
	Document template creating and controling views and models.
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
	virtual iser::IFileLoader* GetFileLoader(const QByteArray& documentTypeId) const = 0;

	/**
		Creates a document instance for document type \c documentTypeId.
		\param	documentTypeId	ID of document type.
	*/
	virtual istd::IChangeable* CreateDocument(const QByteArray& documentTypeId) const = 0;

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

	/**
		Creates a document state comparator.
	*/
	virtual IDocumentStateComparator* CreateStateComparator(const QByteArray& documentTypeId) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTemplate_included


