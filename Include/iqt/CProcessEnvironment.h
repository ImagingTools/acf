#ifndef iqt_CProcessEnvironment_included
#define iqt_CProcessEnvironment_included


#include "isys/IProcessEnvironment.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Qt-based implementation of application environment information.
*/
class CProcessEnvironment: virtual public isys::IProcessEnvironment
{
public:
	// reimplemented (isys::IProcessEnvironment)
	virtual int GetMainThreadId() const;
	virtual void Sleep(double seconds);
	virtual QString GetTempDirPath() const;
	virtual QString GetWorkingDirectory() const;
	virtual QStringList GetApplicationArguments() const;
	virtual QString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const;
	virtual EnvironmentVariables GetEnvironmentVariables() const;
	virtual void SetEnvironmentVariableValue(const QString& variableName, const QString& value);
};


} // namespace iqt


#endif // !iqt_CProcessEnvironment_included
