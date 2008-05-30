#ifndef ibase_IApplicationInfo_included
#define ibase_IApplicationInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "ibase/ibase.h"


#include "iser/IVersionInfo.h"


namespace ibase
{


/**
	This interface provides some information about the application.
*/
class IApplicationInfo: public iser::IVersionInfo
{
public:
	/**
		Get company name.
	*/
	virtual istd::CString GetCompanyName() const = 0;
	
	/**
		Get main name of application.
	*/
	virtual istd::CString GetApplicationName() const = 0;

	/**
		Get working directory of this application.
	*/
	virtual istd::CString GetApplicationPath() const = 0;

	/**
		Encode version number to human readable form.
	*/
	virtual istd::CString EncodeVersionName(I_DWORD version, int versionId = UserVersionId) const = 0;
};


} // namespace ibase


#endif // !ibase_IApplicationInfo_included


