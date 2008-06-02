#ifndef idoc_IDocumentTemplate_included
#define idoc_IDocumentTemplate_included


#include "idoc/idoc.h"


#include "istd/IChangeable.h"
#include "istd/TIFactory.h"
#include "istd/CString.h"

#include "iser/IFileLoader.h"

#include "imod/IObserver.h"
#include "imod/IUndoManager.h"

#include "idoc/ICommand.h"


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
			Document can be created.
		*/
		New = 0x0001,

		/**
			Document is editable.
		*/
		Edit = 0x0010,

		/**
			All features are supported.
		*/
		Default = 0xffff
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
		Return supported view type IDs for specified document type.
		\param	documentTypeId	ID of document type.
	*/
	virtual Ids GetViewTypeIds(const std::string& documentTypeId) const = 0;

	/**
		Get list of supported document ID's can be created for specified file.
	*/
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const = 0;

	/**
		Get file loader/saver for spacified document ID.
		\param	documentTypeId	type ID of document will be loaded/saved.
		\param	forSaving		if it is true, document saving is meant, elsewhere document loading.
	*/
	virtual iser::IFileLoader* GetFileLoader(const std::string& documentTypeId, bool forSaving = false) const = 0;

	/**
		Creates a document instance for document type \c documentTypeId.
	*/
	virtual istd::IChangeable* CreateDocument(const std::string& documentTypeId) const = 0;

	/**
		Creates a view instance for document \c document of type \c viewTypeId.
		\param	documentPtr	pointer to document object.
		\param	viewTypeId	optional ID specifying view type if more view types are supported.
	*/
	virtual istd::IPolymorphic* CreateView(
				const std::string& documentTypeId,
				istd::IChangeable* documentPtr,
				const std::string& viewTypeId = std::string()) const = 0;

	/**
		Creates an undo manger for a given document \c documentPtr.
		\param	documentPtr	pointer to document object.
	*/
	virtual imod::IUndoManager* CreateUndoManager(const std::string& documentTypeId, istd::IChangeable* documentPtr) const = 0;

	/**
		Get list of file filters supported by this document template.
		\param	documentTypeIdPtr	optional specification of document ID
								for which list of filters should be returned.
								If it is NULL, all filters will be returned.
	*/
	virtual istd::CStringList GetFileFilters(const std::string* documentTypeIdPtr = NULL) const = 0;

	/**
		Returns registered file extensions for this document template.
		\param	documentTypeIdPtr	optional specification of document ID
								for which list of possible extensions should be returned.
								If it is NULL, all possible extensions will be returned.
	*/
	virtual istd::CStringList GetFileExtensions(const std::string* documentTypeIdPtr = NULL) const = 0;

	/**
		Return default directory for specified document type.
		\param	sugestedDir	template directory sugested by user.
	*/
	virtual istd::CString GetDefaultDirectory(const istd::CString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTemplate_included


