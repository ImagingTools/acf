#ifndef iser_IVersionInfo_included
#define iser_IVersionInfo_included


// STL includes
#include <set>


// ACF includes
#include "istd/istd.h"
#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace iser
{


/**
	Provides access to version information.

	\ingroup Persistence
*/
class IVersionInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Type of version.
		This is needed for the sake of supporting mutliply separated versions for separated modules.
		Whole ACF framework lay under single versioning mechanism and has one version number identified by AcfVersionId type ID.
		User version type ID should be greater or equal UserVersionId value.
	*/
	enum VersionId
	{
		AcfVersionId = 0,
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
							\sa VersionId.
		\param	result		output version number. For unknown version it will be set to bigg number to indicate of using the newest version.
		\return	true, if version is known.
	 */
	virtual bool GetVersionNumber(int versionId, I_DWORD& result) const = 0;

	/**
		Get description of version ID.
		\param	versionId	versioning type. It allows you to distinguish different version aspects.
							For example version number of framework cann differ from application version.
							\sa VersionId, GetVersionNumber.
	*/
	virtual istd::CString GetVersionIdDescription(int versionId) const = 0;

	/**
		Get list of version ID's.
	*/
	virtual VersionIds GetVersionIds() const = 0;

	/**
		Encode version number to human readable form.
	*/
	virtual istd::CString GetEncodedVersionName(int versionId, I_DWORD versionNumber) const = 0;
};


} // namespace iser


#endif // !iser_IVersionInfo_included


