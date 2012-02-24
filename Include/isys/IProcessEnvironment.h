#ifndef isys_IProcessEnvironment_included
#define isys_IProcessEnvironment_included


// STL includes
#include <map>


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"

#include "isys/isys.h"


namespace isys
{


/**
	Interface for access of some system dependent features and infos.

	\ingroup System
*/
class IProcessEnvironment: virtual public istd::IPolymorphic
{
public:
	typedef std::map<QString, QString> EnvironmentVariables;
	
	/**
		Get current thread ID.
	*/
	virtual int GetMainThreadId() const = 0;

	/**
		Suspend the current thread for \c seconds.
	*/
	virtual void Sleep(double seconds) = 0;

	/**
		Returns the standard temp path, that will be used by the application.
	*/
	virtual QString GetTempDirPath() const = 0;

	/**
		Returns the working directory of application.
	*/
	virtual QString GetWorkingDirectory() const = 0;

	/**
		Returns the arguments of command line for running application process.
	*/
	virtual QStringList GetApplicationArguments() const = 0;

	/**
		Returns the module file name. If the \c useApplicationModule is \c true returns the name of application file/directory,
		otherwise the file name or (directory path) of module in which context this function was called.
		if \c onlyDirectory is \c true only directory path will be returned.
	*/
	virtual QString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const = 0;

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
	virtual void SetEnvironmentVariableValue(const QString& variableName, const QString& value) = 0;
};


} // namespace isys


#endif // !isys_IProcessEnvironment_included
