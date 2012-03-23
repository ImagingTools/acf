#ifndef ibase_IProcessExecutor_included
#define ibase_IProcessExecutor_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IPolymorphic.h"

#include "isys/IProcessEnvironment.h"


namespace ibase
{		


/**
	Main entry point of component-based application.

	\ingroup Main
*/
class IProcessExecutor: virtual public istd::IPolymorphic
{
public:
	/**
		Define execution environment for the process.
	*/
	virtual void SetEnvironment(const isys::IProcessEnvironment& processEnvironment) = 0;

	/**
		Execute external process.
		\param applicationPath - Path to the application's file. 
		\param applicationArguments - Calling arguments for the application. 
	*/
	virtual int ExecuteProcess(const QString& executablePath, const QStringList& processArguments) = 0;
};


} // namespace ibase


#endif // !ibase_IProcessExecutor_included


