#ifndef iser_CReaderVersionInfo_included
#define iser_CReaderVersionInfo_included


#include <map>

#include "iser/IVersionInfo.h"


namespace iser
{


class CReaderVersionInfo: virtual public IVersionInfo
{
public:
	void Reset();
	bool InsertVersionId(int id, I_DWORD version, const std::string& description);
	bool RemoveVersionId(int id);

	// reimplemented (iser::IVersionInfo)
	virtual I_DWORD GetVersion(int versionId = IVersionInfo::UserVersionId) const;
	virtual const std::string& GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;

protected:
	struct VersionIdElement
	{
		I_DWORD version;
		std::string description;

		VersionIdElement(I_DWORD _version, std::string _description):version(_version), description(_description){}
	};

	const VersionIdElement& GetVersionIdElement(int versionId) const;

private:
	typedef std::map<int, VersionIdElement> VersionElements;

	VersionElements m_versionIdList;

	static VersionIdElement s_element;
};


} // namespace iser


#endif // !iser_CReaderVersionInfo_included


