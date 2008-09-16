#include "iser/CArchiveTag.h"


#include "iser/IArchive.h"


namespace iser
{



CArchiveTag::CArchiveTag(
				const std::string& id,
				const std::string& comment,
				bool isTagSkippingUsed)
	:	m_id(id),
		m_comment(comment),
		m_isTagSkippingUsed(isTagSkippingUsed)
{
	m_binaryId = 0;
	int idSize = int(m_id.size());
	for (int i = 0; i < idSize; ++i){
		int character = m_id[i];
		int character2 = character * character;
		m_binaryId += (character << i) ^ (character2 >> i);
	}
}



const std::string& CArchiveTag::GetId() const
{
	return m_id;
}



I_DWORD CArchiveTag::GetBinaryId() const
{
	return m_binaryId;
}



const std::string& CArchiveTag::GetComment() const
{
	return m_comment;
}



} // namespace iser



