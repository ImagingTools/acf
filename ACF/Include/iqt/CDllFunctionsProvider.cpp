#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


CDllFunctionsProvider::CDllFunctionsProvider(const istd::CString& dllPath)
{
	if (!dllPath.IsEmpty()){
		m_library.setFileName(iqt::GetQString(dllPath));
	}
}


CDllFunctionsProvider::~CDllFunctionsProvider()
{
	Reset();
}


void CDllFunctionsProvider::Reset()
{
	m_library.unload();
}


bool CDllFunctionsProvider::OpenDll(const istd::CString& dllPath)
{
	Reset();

	m_library.setFileName(iqt::GetQString(dllPath));

	m_library.load();

	return IsValid();
}


// reimplemented (isys::IFunctionsProvider)

bool CDllFunctionsProvider::IsValid() const
{
	return m_library.isLoaded();
}


void* CDllFunctionsProvider::GetFunction(const ::std::string& id) const
{
	if (IsValid()){
		return m_library.resolve(id.c_str());
	}

	return NULL;
}


} // namespace iqt


