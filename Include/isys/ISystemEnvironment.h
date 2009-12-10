#ifndef isys_ISystemEnviroment_included
#define isys_ISystemEnviroment_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace isys
{

/**
	Interface to getting the information about the currently system configuration.

	\ingroup System
*/
class ISystemEnvironment: virtual public istd::IPolymorphic
{
public:
	/**
		Return the name of currently logged user.
	*/
	virtual istd::CString GetUser() const = 0;

	/**
		Return the path of the system temp directory.
	*/
	virtual istd::CString GetTempDirPath() const = 0;

	/**
		Get the value of system variable.
		\param variableName Name of the system variable
	*/
	virtual istd::CString GetSystemVariableValue(const istd::CString& variableName) const = 0;

	/**
		Generate unique identifier.
	*/
	virtual istd::CString GetUniqueIdentifier() const = 0;
};


} // namespace isys


#endif // isys_ISystemEnvironment_included


