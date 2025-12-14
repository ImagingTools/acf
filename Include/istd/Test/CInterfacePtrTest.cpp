#include <istd/Test/CInterfacePtrTest.h>


// ACF includes
#include <istd/TInterfacePtr.h>
#include <istd/TOptInterfacePtr.h>
#include <itest/CStandardTestExecutor.h>


class IInterface1: public istd::IPolymorphic
{
};

class IInterface2: public istd::IPolymorphic
{
};

class IInterface22: public IInterface2
{
};

class T1 : public IInterface1
{
};

class T2 : public IInterface2
{
};

class T22 : public IInterface22
{
};


void CInterfacePtrTest::DoSharedInterfaceTest()
{
	istd::TSharedInterfacePtr<IInterface1> sharedPtr(new T1);
	QVERIFY(sharedPtr.IsValid());

	istd::TSharedInterfacePtr<IInterface1> sharedPtr2 = sharedPtr;
	QVERIFY(sharedPtr2.IsValid());

	istd::TSharedInterfacePtr<const IInterface1> sharedConstPtr;
	sharedConstPtr = sharedPtr;
	QVERIFY(sharedConstPtr.IsValid());
	sharedConstPtr = std::make_shared<T1>();
	QVERIFY(sharedConstPtr.IsValid());
	QVERIFY(sharedConstPtr);

	istd::TSharedInterfacePtr<const IInterface1> sharedConstPtr2;
	sharedConstPtr2 = std::make_unique<T1>();
	QVERIFY(sharedConstPtr2.IsValid());

	sharedPtr.Reset();
	QVERIFY(!sharedPtr.IsValid());

	sharedPtr2.Reset();
	QVERIFY(!sharedPtr2.IsValid());
}


void CInterfacePtrTest::DoUniqueInterfaceTest()
{
	istd::TUniqueInterfacePtr<IInterface1> ptr1(new T1);
	QVERIFY(ptr1.IsValid());

	istd::TSharedInterfacePtr<IInterface1> sharedPtr = istd::TSharedInterfacePtr<IInterface1>::CreateFromUnique(ptr1);
	QVERIFY(sharedPtr.IsValid());
	QVERIFY(!ptr1.IsValid());

	istd::TUniqueInterfacePtr<IInterface1> ptr2(std::make_unique<T1>());
	QVERIFY(ptr2.IsValid());

	istd::TUniqueInterfacePtr<IInterface2> ptr3(std::make_unique<T22>());
	QVERIFY(ptr3.IsValid());

	istd::TUniqueInterfacePtr<const IInterface1> ptr4(std::make_unique<T1>());
	QVERIFY(ptr4.IsValid());
}


void CInterfacePtrTest::DoOptionalInterfaceTest()
{
	istd::TSharedInterfacePtr<IInterface1> sharedPtr(new T1);
	QVERIFY(sharedPtr.IsValid());

	T1 instance1;
	istd::TOptInterfacePtr<IInterface1> opt1;
	opt1.SetUnmanagedPtr(&instance1);
	QVERIFY(opt1.IsValid());
	QVERIFY(opt1.IsUnmanaged());
	QVERIFY(!opt1.IsManaged());

	istd::TOptInterfacePtr<IInterface1> opt2(sharedPtr);
	QVERIFY(opt2.IsValid());
	QVERIFY(sharedPtr.IsValid());

	sharedPtr.Reset();
	QVERIFY(opt2.IsValid());

	opt2 = opt1;
	QVERIFY(opt2.IsValid());
	QVERIFY(opt2.IsUnmanaged());
	QVERIFY(!opt2.IsManaged());

	// Incompatible interfaces
	{
		istd::TUniqueInterfacePtr<IInterface2> t2Ptr(new T2);
		QVERIFY(t2Ptr.IsValid());
		istd::TOptInterfacePtr<IInterface1> optInterface1;
		QVERIFY(!optInterface1.TakeOver(t2Ptr));
		QVERIFY(!optInterface1.IsValid());
		QVERIFY(t2Ptr.IsValid());
	}

	// Same interfaces
	{
		istd::TUniqueInterfacePtr<IInterface1> t1tr(new T1);
		QVERIFY(t1tr.IsValid());
		istd::TOptInterfacePtr<IInterface1> optInterface1;
		QVERIFY(optInterface1.TakeOver(t1tr));
		QVERIFY(optInterface1.IsValid());
		QVERIFY(optInterface1.IsManaged());
		QVERIFY(!t1tr.IsValid());
	}

	// Compatible interfaces
	{
		istd::TSharedInterfacePtr<IInterface22> t2Ptr(new T22);
		QVERIFY(t2Ptr.IsValid());
		istd::TOptInterfacePtr<IInterface2> optInterface1;
		QVERIFY(optInterface1.SetCastedPtr(t2Ptr));
		QVERIFY(optInterface1.IsValid());
		QVERIFY(t2Ptr.IsValid());
	}

	// Adopt
	{
		
		IInterface22* t2Ptr = new T22;
		istd::TOptInterfacePtr<IInterface2> optInterface1;
		QVERIFY(optInterface1.AdoptCastedRawPtr(t2Ptr));
		QVERIFY(optInterface1.IsValid());
	}
}


I_ADD_TEST(CInterfacePtrTest);


