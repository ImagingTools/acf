#ifndef iwin_CDllFunctionsProvider_included
#define iwin_CDllFunctionsProvider_included


#include "iwin/iwin.h"


#include <windows.h>

#include "isys/IFunctionsProvider.h"

#include "istd/CString.h"


namespace iwin
{


/**
	Provide access to public functions in DLL.
 */
class CDllFunctionsProvider: virtual public isys::IFunctionsProvider
{
public:
	/**
		Construct this object opening specified DLL.
		\param	dllPath	path to DLL or empty string if no DLL should be opened.
	 */
	CDllFunctionsProvider(const istd::CString& dllPath = "");
	~CDllFunctionsProvider();

	bool OpenDll(const istd::CString& dllPath);

	void Reset();

	// reimplemented (isys::IFunctionsProvider)
	virtual bool IsValid() const;
	virtual void* GetFunction(const std::string& id) const;

private:
	HINSTANCE m_handler;
};


} // namespace iwin


#endif // !iwin_CDllFunctionsProvider_included


