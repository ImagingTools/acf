#ifndef iser_CReaderVersionInfo_included
#define iser_CReaderVersionInfo_included


#include <map>

#include "iser/IVersionInfo.h"


namespace iser
{


/**
	Implementation of version designed to be automaticaly created during reading of ACF header.
	\sa iser::CReadArchiveBase.
*/
class CReaderVersionInfo: virtual public IVersionInfo
{
public:
	/**
		Remove all stored version infos.
	*/
	void Reset();
	/**
		Insert new version id with its actual version number and its description.
		\sa iser::IVersionInfo.
		\param	id			ID of version info.
		\param	version		version number for specified version info ID.
		\param	description	description of role of specified version info.
	*/
	bool InsertVersionId(int id, I_DWORD version, const istd::CString& description);
	/**
		Remove single version info.
		\param	id	ID of version info.
	*/
	bool RemoveVersionId(int id);

	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, I_DWORD& result) const;
	virtual istd::CString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;

protected:
	struct VersionIdElement
	{
		I_DWORD version;
		istd::CString description;

		VersionIdElement(I_DWORD _version, const istd::CString& _description):version(_version), description(_description){}
	};

	const VersionIdElement& GetVersionIdElement(int versionId) const;

private:
	typedef std::map<int, VersionIdElement> VersionElements;

	VersionElements m_versionIdList;

	static VersionIdElement s_element;
};


} // namespace iser


#endif // !iser_CReaderVersionInfo_included


