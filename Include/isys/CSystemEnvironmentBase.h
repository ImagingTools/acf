#ifndef isys_CSystemEnvironmentBase_included
#define isys_CSystemEnvironmentBase_included


#include "isys/ISystemEnvironment.h"


namespace isys
{


/**
	System indepent implementation of ISystemEnvironment interface. 
*/
class CSystemEnvironmentBase: virtual public isys::ISystemEnvironment
{
public:
	// reimplemented (isys::ISystemEnvironment)
	virtual istd::CString GetSystemVariableValue(const istd::CString& variableName) const;
};


} // namespace isys


#endif // isys_CSystemEnvironmentBase_included


