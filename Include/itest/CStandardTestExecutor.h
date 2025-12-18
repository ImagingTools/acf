#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QVector>

// ACF includes
#include <istd/TSmartPtr.h>
#include <itest/ITestExecutor.h>


namespace itest
{


/**
	Standard implementation of ITestExecutor with static test registration
*/
class CStandardTestExecutor: virtual public ITestExecutor
{
public:
	// reimplemented (ITestExecutor)
	virtual int RunTests(int argc, char *argv[]) override;

public:
	template <class TestObject>
	class TestRegistrator
	{
	public:
		TestRegistrator(const QString& name)
		{
			TestPtr testPtr(new TestObject);

			testPtr->setObjectName(name);

			AddTest(testPtr);
		}
	};

private:
	typedef istd::TSmartPtr<QObject> TestPtr;
	typedef QVector<TestPtr> TestInstances;

	static TestInstances& GetTests();
	static bool FindTest(const QString& name);
	static void AddTest(TestPtr test);
};


} // namespace itest

#define I_ADD_TEST(className) const itest::CStandardTestExecutor::TestRegistrator<className> className##_AddTest(#className);


