#ifndef iqt_CDllFunctionsProvider_included
#define iqt_CDllFunctionsProvider_included


#include "iqt/iqt.h"

#include <QString>

#include "isys/IFunctionsProvider.h"

#include <QLibrary>


namespace iqt
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
	CDllFunctionsProvider(const QString& dllPath = "");
	virtual ~CDllFunctionsProvider();

	bool OpenDll(const QString& dllPath);

	void Reset();

	// reimplemented (isys::IFunctionsProvider)
	virtual bool IsValid() const;
	virtual void* GetFunction(const std::string& id) const;

private:
	mutable QLibrary m_library;
};


} // namespace iqt


#endif // !iqt_CDllFunctionsProvider_included


