#ifndef iser_CArchiveTag_included
#define iser_CArchiveTag_included


// Qt includes
#include <QtCore/QByteArray>


namespace iser
{


class IArchive;


/**
	Process tag used to group data in archive stream.

	\ingroup Persistence
*/
class CArchiveTag
{
public:
	/**
		Describe type of tag.
	*/
	enum TagType
	{
		/**
			Unknown type of tag.
		*/
		TT_UNKNOWN,
		/**
			Normal tag used for grouping of tags or processed elements.
			It should be outputted to serialized stream.
		*/
		TT_GROUP,
		/**
			Multiple tag containing variable number of child tags.
			All childs must be the same type.
		*/
		TT_MULTIPLE,
		/**
			Leaf tag, it can contain only one primitive element.
			Single group can contain only one leaf tag with the same name.
			Depending on implementation some leaf specific optimizations can be enabled (e.g. using of attributes for XML documents).
		*/
		TT_LEAF,
		/**
			Weak group tag, if possible it will be not outputted.
		*/
		TT_WEAK
	};

	CArchiveTag();
	/**
		Constructor with parameter initialization.
		\param	id					ID of this tag.
		\param	comment				human readable and are used for diagnostic.
		\param	isTagSkippingUsed	true if it is needed to support tag skipping for this tag.
	*/
	CArchiveTag(const QByteArray& id,
				const QByteArray& comment,
				TagType tagType = TT_UNKNOWN,
				const CArchiveTag* parentPtr = NULL,
				bool isTagSkippingUsed = false);

	/**
		Get ID of this tag.
	*/
	const QByteArray& GetId() const;

	/**
		Get automatic generated binary ID.
	*/
	quint32 GetBinaryId() const;

	/**
		Get comment of this tag.
		Comments are human readable and are used for diagnostic.
	*/
	const QByteArray& GetComment() const;

	/**
		Get type of this tag.
	*/
	TagType GetTagType() const;

	/**
		Get tag beeing parent of this.
	*/
	const CArchiveTag* GetParentTag() const;

	/**
		Check if it is needed to support tag skipping for this tag.
	*/
	bool IsTagSkippingUsed() const;

private:
	QByteArray m_id;
	quint32 m_binaryId;

	QByteArray m_comment;

	TagType m_tagType;
	const CArchiveTag* m_parentPtr;

	bool m_isTagSkippingUsed;
};


// inline methods

inline bool CArchiveTag::IsTagSkippingUsed() const
{
	return m_isTagSkippingUsed;
}


} // namespace iser


#endif // !iser_CArchiveTag_included


