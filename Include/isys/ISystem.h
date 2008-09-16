#ifndef isys_ISystem_included
#define isys_ISystem_included


#include "isys/isys.h"


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace isys
{


/**
	\ingroup isys

	Interface for access of some system dependent features and infos.
*/
class ISystem: virtual public istd::IPolymorphic
{
public:
	/**
		Returns the standard temp path, that will be used by the application.
	*/
	virtual istd::CString GetTempDirPath() const = 0;

	/**
		Returns the working directory of application.
	*/
	virtual istd::CString GetWorkingDirectory() const = 0;
};


} // namespace isys


#endif // !isys_ISystem_included
