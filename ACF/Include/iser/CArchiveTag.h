#ifndef iser_CArchiveTag_included
#define iser_CArchiveTag_included


#include <string>

#include "iser/IVersionInfo.h"


namespace iser
{


class IArchive;


/**
	Process tag used to group data in archive stream.
*/
class CArchiveTag
{
public:
	CArchiveTag(	const std::string& id,
					const std::string& comment,
					bool isTagSkippingUsed = false);

	/**
		Get ID of this tag.
	*/
	const std::string& GetId() const;

	/**
		Get automatic generated binary ID.
	*/
	I_DWORD GetBinaryId() const;

	/**
		Get comment of this tag.
		Comments are human readable and are used for diagnostic.
	*/
	const std::string& GetComment() const;
	/**
		Check if it is needed to support tag skipping for this tag.
	*/
	bool IsTagSkippingUsed() const;

private:
	std::string m_id;
	I_DWORD m_binaryId;

	std::string m_comment;

	bool m_isTagSkippingUsed;
};


// inline methods

inline bool CArchiveTag::IsTagSkippingUsed() const
{
	return m_isTagSkippingUsed;
}


} // namespace iser



#endif // !iser_CArchiveTag_included



