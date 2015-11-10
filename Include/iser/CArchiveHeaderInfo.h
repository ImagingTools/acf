#ifndef iser_CArchiveHeaderInfo_included
#define iser_CArchiveHeaderInfo_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "iser/IVersionInfo.h"


namespace iser
{


class IArchive;


/**
	Represents functions related to ACF header.
	\sa iser::CReadArchiveBase.

	\ingroup Persistence
*/
class CArchiveHeaderInfo: virtual public IVersionInfo
{
public:
	enum ChangeFlags
	{
		CF_VERSIONS_UPDATED = 0xb6eca0
	};

	/**
		Remove all stored version infos.
	*/
	void Reset();
	/**
		Insert new version id with its actual version number and its description.
		\sa iser::IVersionInfo.
		\param	versionId		ID of version info.
		\param	versionNumber	version number for specified version info ID.
		\param	description		description of role of specified version info.
	*/
	bool InsertVersionId(int versionId, quint32 versionNumber, const QString& description);
	/**
		Remove single version info.
		\param	versionId	ID of version info.
	*/
	bool RemoveVersionId(int versionId);

	bool SerializeArchiveHeader(iser::IArchive& archive);
	static bool WriteArchiveHeader(iser::IArchive& archive, const const IVersionInfo* versionInfoPtr);

	// reimplemented (iser::IVersionInfo)
	virtual VersionIds GetVersionIds() const;
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	struct VersionIdElement
	{
		quint32 versionNumber;
		QString description;
	};

private:
	typedef QMap<int, VersionIdElement> VersionElements;

	VersionElements m_versionIdList;
};


} // namespace iser


#endif // !iser_CArchiveHeaderInfo_included


