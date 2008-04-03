#ifndef iser_IVersionInfo_included
#define iser_IVersionInfo_included


#include <set>

#include "istd/IPolymorphic.h"

#include "iser/iser.h"


namespace iser
{


class IVersionInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Type of version.
		This is needed for the sake of supporting mutliply separated versions for separated modules.
		Whole ACF framework lay under single versioning mechanism and has one version number identified by FrameworkVersionId type ID.
		User version type ID should be greater or equal UserVersionId value.
		As a default version type ID for user applications UserVersionId should be used.
	*/
	enum VersionId
	{
		FrameworkVersionId,
		UserVersionId = 1024
	};

	enum{
		/**
			Biggest version value used if no version is needed or known.
		*/
		UnknownVersion = 0xffffffff
	};

	typedef std::set<int> VersionIds;

	/**
		Get version of archived stream for specific versioning type.
		\param	versionId	versioning type. It allows you to distinguish different version aspects.
								For example version number of framework cann differ from application version.
								@sa VersionId.
	 */
	virtual I_DWORD GetVersion(int versionId = UserVersionId) const = 0;

	/**
		Get description of version ID.
		\param	versionId	versioning type. It allows you to distinguish different version aspects.
								For example version number of framework cann differ from application version.
								@sa VersionId, GetVersion.
	*/
	virtual const std::string& GetVersionIdDescription(int versionId) const = 0;

	/**
		Get list of version ID's.
	*/
	virtual VersionIds GetVersionIds() const = 0;
};


} // namespace iser


#endif // !iser_IVersionInfo_included


