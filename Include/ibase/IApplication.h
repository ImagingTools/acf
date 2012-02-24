#ifndef ibase_IApplication_included
#define ibase_IApplication_included


#include "istd/IPolymorphic.h"
#include <QString>


namespace ibase
{		


/**
	Main entry point of component-based application.

	\ingroup Main
	\ingroup ComponentConcept
*/
class IApplication: virtual public istd::IPolymorphic
{
public:
	/**
		Initialize the application object.
	*/
	virtual bool InitializeApplication(int argc, char** argv) = 0;

	/**
		Starts execution loop of the application.
	*/
	virtual int Execute(int argc, char** argv) = 0;

	/**
		Gets the command line arguments of this application.
	*/
	virtual QString GetHelpText() const = 0;
};


} // namespace ibase


#endif // !ibase_IApplication_included


