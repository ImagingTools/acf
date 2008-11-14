#ifndef iwin_CSystemEnvironment_included
#define iwin_CSystemEnvironment_included


#include "isys/CSystemEnvironmentBase.h"


namespace iwin
{


/**
	System indepent implementation of ISystemEnvironment interface. 
*/
class CSystemEnvironment: virtual public isys::CSystemEnvironmentBase
{
public:
	typedef isys::CSystemEnvironmentBase BaseClass;

	// reimplemented (isys::ISystemEnvironment)
	virtual istd::CString GetUser() const;
	virtual istd::CString GetTempDirPath() const;
};


} // namespace iwin


#endif // iwin_CSystemEnvironment_included


