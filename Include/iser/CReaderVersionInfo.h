#ifndef iser_CReaderVersionInfo_included
#define iser_CReaderVersionInfo_included


// STL includes
#include <map>


// ACF includes
#include "iser/IVersionInfo.h"


namespace iser
{


/**
	Implementation of version designed to be automaticaly created during reading of ACF header.
	\sa iser::CReadArchiveBase.

	\ingroup Persistence
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
	bool InsertVersionId(int id, quint32 version, const QString& description);
	/**
		Remove single version info.
		\param	id	ID of version info.
	*/
	bool RemoveVersionId(int id);

	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

protected:
	struct VersionIdElement
	{
		quint32 version;
		QString description;

		VersionIdElement(quint32 _version, const QString& _description):version(_version), description(_description){}
	};

	const VersionIdElement& GetVersionIdElement(int versionId) const;

private:
	typedef std::map<int, VersionIdElement> VersionElements;

	VersionElements m_versionIdList;

	static VersionIdElement s_element;
};


} // namespace iser


#endif // !iser_CReaderVersionInfo_included


