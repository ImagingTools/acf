#ifndef iqt_CSystemEnvironment_included
#define iqt_CSystemEnvironment_included


#include "isys/CSystemEnvironmentBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	System implementation of ISystemEnvironment interface on Qt plattform.

	\ingroup Qt
*/
class CSystemEnvironment: virtual public isys::CSystemEnvironmentBase
{
public:
	typedef isys::CSystemEnvironmentBase BaseClass;

	// reimplemented (isys::ISystemEnvironment)
	virtual QString GetUser() const;
	virtual QString GetTempDirPath() const;
	virtual QString GetUniqueIdentifier() const;
};


} // namespace iqt


#endif // iqt_CSystemEnvironment_included


