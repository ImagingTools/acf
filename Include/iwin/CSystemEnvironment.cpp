#include "iwin/CSystemEnvironment.h"


// WINFOWS includes
#include <windows.h>


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


istd::CString CSystemEnvironment::GetUniqueIdentifier() const
{
	// TODO: move to basics
	GUID guid;
	::UuidCreate(&guid);
	RPC_CSTR guidBstr = NULL;
	::UuidToString(&guid, &guidBstr);

	istd::CString uniqueIdentifier = istd::CString((char*)guidBstr);
	
	::RpcStringFree(&guidBstr);

	return uniqueIdentifier;
}


} // namespace isys


