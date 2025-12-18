#ifndef idoc_IDocumentMetaInfo_included
#define idoc_IDocumentMetaInfo_included


// Qt includes
#include <QtCore/QVariant>

// ACF includes
#include <istd/TInterfacePtr.h>
#include <istd/IChangeable.h>


namespace idoc
{


/**
	Interface for document meta-information.
*/
class IDocumentMetaInfo: virtual public istd::IChangeable
{
public:

	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			Meta information has been changed.
		*/
		CF_METAINFO = 0xd83067d
	};

	/**
		Type of the meta information.
	*/
	enum MetaInfoType
	{
		/**
			Title of the document.
		*/
		MIT_TITLE,

		/**
			Person, who has created the document.
		*/
		MIT_AUTHOR,

		/**
			Tool used for document creation.
		*/
		MIT_CREATOR,

		/**
			Document description.
		*/
		MIT_DESCRIPTION,

		/**
			Document creation time.
		*/
		MIT_CREATION_TIME,

		/**
			Last document modification time.
		*/
		MIT_MODIFICATION_TIME,

		/**
			Version of the creation tool given as a QString.
		*/
		MIT_CREATOR_VERSION,

		/**
			Version of the document given as a QString.
		*/
		MIT_DOCUMENT_VERSION,

		/**
			Checksum of the document data.
		*/
		MIT_CONTENT_CHECKSUM,

		/**
			Starting of user defined meta info types.
		*/
		MIT_USER = 1024
	};

	typedef QSet<int> MetaInfoTypes;

	/**
		Get list of contained meta info types.
		\param	allowReadOnly	if enabled, read only types will be also listed out, if this flag is false only the writable types will be returned.
								\sa IsMetaInfoWritable.
	*/
	virtual MetaInfoTypes GetMetaInfoTypes(bool allowReadOnly = true) const = 0;

	/**
		Get document's meta info.
	*/
	virtual QVariant GetMetaInfo(int metaInfoType) const = 0;

	/**
		Set new document's meta information for a given meta type.
	*/
	virtual bool SetMetaInfo(int metaInfoType, const QVariant& metaInfo) = 0;

	/**
		Remove document's meta information for a given meta type.
	*/
	virtual void RemoveMetaInfo(int metaInfoType) = 0;

	/**
		Get an unique ID of the meta information.
	*/
	virtual QByteArray GetMetaInfoId(int metaInfoType) const = 0;

	/**
		Get a human readable name for a given meta information type.
	*/
	virtual QString GetMetaInfoName(int metaInfoType) const = 0;

	/**
		Get a human readable description for a given meta information type.
	*/
	virtual QString GetMetaInfoDescription(int metaInfoType) const = 0;

	/**
		Check if meta information can be changed.
	*/
	virtual bool IsMetaInfoWritable(int metaInfoType) const = 0;
};


typedef istd::TSharedInterfacePtr<IDocumentMetaInfo> MetaInfoPtr;


} // namespace idoc


#endif // !idoc_IDocumentMetaInfo_included
