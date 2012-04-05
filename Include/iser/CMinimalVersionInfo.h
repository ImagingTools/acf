#ifndef iser_CMinimalVersionInfo_included
#define iser_CMinimalVersionInfo_included


// STL includes
#include <QtCore/QMap>


// ACF includes
#include "iser/IVersionInfo.h"


namespace iser
{


class ISerializable;


/**
	Implementation of interface iser::IVersionInfo using information about
	minimal needed version from interface iser::ISerializable.
	Please note, that list of ID's for version info must be separate created,
	becouse this information can not be retrived from iser::ISerializable.

	\ingroup Persistence
*/
class CMinimalVersionInfo: virtual public IVersionInfo
{
public:
	/**
		Constructor with initialization of reference to iser::ISerializable and registring of standard ID's.
		\param	serializablePtr	pointer to serializable parent object for which minimal needed version will be calculated.
		\param	addStandardIds	if it is true, standard ID will be registered.
	*/
	CMinimalVersionInfo(const ISerializable* serializablePtr, bool addStandardIds = true);

	/**
		Add new version ID.
		\param	versionId	ID of registered version.
		\param	description	description of this ID.
		\return				true, if success.
	*/
	bool AddId(int versionId, const QString& description = "");

	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

private:
	struct Info
	{
		mutable quint32 version;
		QString description;
	};

	typedef QMap<int, Info> Versions;

	Versions m_versions;

	const ISerializable& m_serializable;
};


} // namespace iser


#endif // !iser_CMinimalVersionInfo_included


