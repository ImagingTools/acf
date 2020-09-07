#ifndef itest_ITestContainer_included
#define itest_ITestContainer_included


// ACF includes
#include <istd/IPolymorphic.h>


namespace itest
{


/**
	Interface for running a collection of tests
*/
class ITestExecutor: virtual public istd::IPolymorphic
{
public:
	virtual int RunTests(int argc, char *argv[]) = 0;
};


} // namespace itest


#endif // !itest_ITestContainer_included


