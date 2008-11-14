#include "iwin/CSystemEnvironment.h"


namespace iwin
{


// reimplemented (isys::ISystemEnvironment)

istd::CString CSystemEnvironment::GetUser() const
{
	return GetSystemVariableValue("USER");
}


istd::CString CSystemEnvironment::GetTempDirPath() const
{
	return GetSystemVariableValue("TEMP");
}


} // namespace isys


