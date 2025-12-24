#include <iser/CArchiveTag.h>


namespace iser
{


CArchiveTag::CArchiveTag()
:	m_binaryId(0),
	m_tagType(TT_UNKNOWN),
	m_parentPtr(NULL),
	m_isTagSkippingUsed(false)
{
}


CArchiveTag::CArchiveTag(
			const QByteArray& id,
			const QByteArray& comment,
			TagType tagType,
			const CArchiveTag* parentPtr,
			bool isTagSkippingUsed)
:	m_id(id),
	m_binaryId(0),
	m_comment(comment),
	m_tagType(tagType),
	m_parentPtr(parentPtr),
	m_isTagSkippingUsed(isTagSkippingUsed)
{
	int idSize = int(m_id.size());

	for (int charIndex = 0; charIndex < idSize; ++charIndex){
		int character = m_id[charIndex];
		int character2 = character * character;
		m_binaryId += (character << charIndex) ^ (character2 >> charIndex);
	}
}


const QByteArray& CArchiveTag::GetId() const
{
	return m_id;
}


quint32 CArchiveTag::GetBinaryId() const
{
	return m_binaryId;
}


const QByteArray& CArchiveTag::GetComment() const
{
	return m_comment;
}


CArchiveTag::TagType CArchiveTag::GetTagType() const
{
	return m_tagType;
}


const CArchiveTag* CArchiveTag::GetParentTag() const
{
	return m_parentPtr;
}


} // namespace iser


