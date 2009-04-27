#ifndef iqt_CApplicationEnvironment_included
#define iqt_CApplicationEnvironment_included


#include "isys/IApplicationEnvironment.h"


namespace iqt
{


/**
	Qt-based implementation of application environment information.
*/
class CApplicationEnvironment: virtual public isys::IApplicationEnvironment
{
public:
	// reimplemented (isys::IApplicationEnvironment)
	virtual istd::CString GetTempDirPath() const;
	virtual istd::CString GetWorkingDirectory() const;
	virtual istd::CStringList GetApplicationArguments() const;
	virtual istd::CString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const;
	virtual EnvironmentVariables GetEnvironmentVariables() const;
};


} // namespace iqt


#endif // !iqt_CApplicationEnvironment_included
