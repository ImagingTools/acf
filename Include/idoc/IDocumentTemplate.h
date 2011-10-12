#ifndef idoc_IDocumentTemplate_included
#define idoc_IDocumentTemplate_included


#include "istd/IChangeable.h"
#include "istd/TIFactory.h"
#include "istd/CString.h"

#include "iser/IFileLoader.h"

#include "imod/IObserver.h"
#include "imod/IUndoManager.h"

#include "ibase/ICommand.h"

#include "idoc/IDocumentStateComparator.h"


namespace idoc
{


/**
	Document template creating and controling views and models.
*/
class IDocumentTemplate: virtual public istd::IPolymorphic
{
public:
	typedef std::vector<std::string> Ids;

	/**
		Enumeration for supported types of operation with the document.
	*/
	enum SupportedFeatures
	{
		/**
			A new document can be created.
		*/
		SF_NEW_DOCUMENT = 0x0001,

		/**
			A document is editable.
		*/
		SF_EDIT_DOCUMENT = 0x0010,

		/**
			All features are supported.
		*/
		SF_DEFAULT = 0xffff
	};

	/**
		Return \c true, if the feature(s) is supported by this document template.
	*/
	virtual bool IsFeatureSupported(int featureFlags, const std::string& documentTypeId) const = 0;

	/**
		Get list of supported document ID's can be created for specified file.
	*/
	virtual Ids GetDocumentTypeIds() const = 0;

	/**
		Get human readable name of some document type ID.
	*/
	virtual istd::CString GetDocumentTypeName(const std::string& documentTypeId) const = 0;

	/**
		Return supported view type IDs for specified document type.
		\param	documentTypeId	ID of document type.
	*/
	virtual Ids GetViewTypeIds(const std::string& documentTypeId) const = 0;

	/**
		Get human readable name of some view type ID.
	*/
	virtual istd::CString GetViewTypeName(
				const std::string& documentTypeId,
				const std::string& viewTypeId) const = 0;

	/**
		Get list of supported document ID's can be created for specified file.
	*/
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const = 0;

	/**
		Get file loader/saver for spacified document ID.
		\param	documentTypeId	type ID of document will be loaded/saved.
	*/
	virtual iser::IFileLoader* GetFileLoader(const std::string& documentTypeId) const = 0;

	/**
		Creates a document instance for document type \c documentTypeId.
		\param	documentTypeId	ID of document type.
	*/
	virtual istd::IChangeable* CreateDocument(const std::string& documentTypeId) const = 0;

	/**
		Creates a view instance for document \c document of type \c viewTypeId.
		\param	documentTypeId	ID of document type.
		\param	documentPtr		pointer to document object.
		\param	viewTypeId		optional ID specifying view type if more view types are supported.
	*/
	virtual istd::IPolymorphic* CreateView(
				const std::string& documentTypeId,
				istd::IChangeable* documentPtr,
				const std::string& viewTypeId = std::string()) const = 0;

	/**
		Creates an undo manger for a given document \c documentPtr.
		\param	documentTypeId	ID of document type.
		\param	documentPtr	pointer to document object.
		\return	pointer to created instance of undo manager or NULL, if creating of undo manager was impossible.
	*/
	virtual imod::IUndoManager* CreateUndoManager(const std::string& documentTypeId, istd::IChangeable* documentPtr) const = 0;

	/**
		Creates a document state comparator.
	*/
	virtual IDocumentStateComparator* CreateStateComparator(const std::string& documentTypeId) const = 0;

	/**
		Return default directory for specified document type.
		\param	sugestedDir			template directory sugested by user.
		\param	documentTypeIdPtr	optional ID of document type.
	*/
	virtual istd::CString GetDefaultDirectory(const istd::CString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTemplate_included


