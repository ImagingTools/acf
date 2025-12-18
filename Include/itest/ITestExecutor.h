#pragma once


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


