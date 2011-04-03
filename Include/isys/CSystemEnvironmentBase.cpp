#include "isys/CSystemEnvironmentBase.h"


// STD includes
#include <cstdlib>


namespace isys
{


// reimplemented (isys::ISystemEnvironment)

istd::CString CSystemEnvironmentBase::GetSystemVariableValue(const istd::CString& variableName) const
{
	return istd::CString(::getenv(variableName.ToString().c_str()));
}


} // namespace isys


