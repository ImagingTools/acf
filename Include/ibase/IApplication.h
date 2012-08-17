#ifndef ibase_IApplication_included
#define ibase_IApplication_included


#include "istd/IPolymorphic.h"
#include <QtCore/QString>


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
		Start execution loop of the application.
	*/
	virtual int Execute(int argc, char** argv) = 0;

	/**
		Get information about the possible command line arguments of this application.
	*/
	virtual QString GetHelpText() const = 0;

	/**
		Get the command line arguments of this application.
	*/
	virtual QStringList GetApplicationArguments() const = 0;
};


} // namespace ibase


#endif // !ibase_IApplication_included


