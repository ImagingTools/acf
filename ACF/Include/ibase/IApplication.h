#ifndef ibase_IApplication_included
#define ibase_IApplication_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace ibase
{		


/**
	Main entry point of component-based application.
*/
class IApplication: virtual public istd::IPolymorphic
{
public:
	/**
	* Starts execution loop of the application.
	*/
	virtual int Execute(int argc, char** argv) = 0;

	/**
	* Gets the command line arguments of this application.
	*/
	virtual istd::CString GetHelpText() const = 0;
};


} // namespace ibase


#endif // !ibase_IApplication_included


