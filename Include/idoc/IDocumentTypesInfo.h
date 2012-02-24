#ifndef idoc_IDocumentTypesInfo_included
#define idoc_IDocumentTypesInfo_included


// STL includes
#include <vector>


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"

#include "iser/IFileTypeInfo.h"


namespace idoc
{


/**
	Provide information about different document types.
*/
class IDocumentTypesInfo: virtual public istd::IPolymorphic
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
		\param	documentTypeId	ID of document type as returned by \c GetDocumentTypeIds().
	*/
	virtual QString GetDocumentTypeName(const std::string& documentTypeId) const = 0;

	/**
		Get file type information object for some selected document type.
		\param	documentTypeId	ID of document type as returned by \c GetDocumentTypeIds().
	*/
	virtual iser::IFileTypeInfo* GetDocumentFileTypeInfo(const std::string& documentTypeId) const = 0;

	/**
		Get list of supported document ID's can be created for specified file.
	*/
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const = 0;

	/**
		Return default directory for specified document type.
		\param	sugestedDir			template directory sugested by user.
		\param	documentTypeIdPtr	optional ID of document type.
	*/
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTypesInfo_included


