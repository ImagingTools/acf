#ifndef isys_ISystemEnviroment_included
#define isys_ISystemEnviroment_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"


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
	virtual QString GetUser() const = 0;

	/**
		Return the path of the system temp directory.
	*/
	virtual QString GetTempDirPath() const = 0;

	/**
		Get the value of system variable.
		\param variableName Name of the system variable
	*/
	virtual QString GetSystemVariableValue(const QString& variableName) const = 0;

	/**
		Generate unique identifier.
	*/
	virtual QString GetUniqueIdentifier() const = 0;
};


} // namespace isys


#endif // isys_ISystemEnvironment_included


