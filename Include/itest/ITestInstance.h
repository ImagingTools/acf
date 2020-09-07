#ifndef itest_ITestInstance_included
#define itest_ITestInstance_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <istd/IPolymorphic.h>


namespace itest
{


/**
	Test instance interface.
*/
class ITestInstance: virtual public istd::IPolymorphic
{
public:
	// Runs the test instance and returns report about each sub-test
	// If sub-test report is empty, the test was successful
	virtual QStringList Run() = 0;
};


} // namespace itest


#endif // !itest_ITestInstance_included


