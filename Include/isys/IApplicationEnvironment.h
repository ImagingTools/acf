#ifndef isys_IApplicationEnvironment_included
#define isys_IApplicationEnvironment_included


// STL includes
#include <map>


#include "isys/isys.h"

#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace isys
{


/**
	Interface for access of some system dependent features and infos.

	\ingroup System
*/
class IApplicationEnvironment: virtual public istd::IPolymorphic
{
public:
	typedef std::map<istd::CString, istd::CString> EnvironmentVariables;
	
	/**
		Returns the standard temp path, that will be used by the application.
	*/
	virtual istd::CString GetTempDirPath() const = 0;

	/**
		Returns the working directory of application.
	*/
	virtual istd::CString GetWorkingDirectory() const = 0;

	/**
		Returns the arguments of command line for running application process.
	*/
	virtual istd::CStringList GetApplicationArguments() const = 0;

	/**
		Returns the module file name. If the \c useApplicationModule is \c true returns the name of application file/directory,
		otherwise the file name or (directory path) of module in which context this function was called.
		if \c onlyDirectory is \c true only directory path will be returned.
	*/
	virtual istd::CString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const = 0;

	/**
		Returns the environment variables of the application process.
		The key of the EnvironmentVariables map is the name of the environment variable.
	*/
	virtual EnvironmentVariables GetEnvironmentVariables() const = 0;

	/**
		Set/Add the value of a system variable to the current process.
		\param variableName Name of the system variable
		\param variableValue Value of the system variable
	*/
	virtual void SetEnvironmentVariableValue(const istd::CString& variableName, const istd::CString& value) = 0;
};


} // namespace isys


#endif // !isys_IApplicationEnvironment_included
