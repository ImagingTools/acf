#include "CMultiThreadingTest.h"


// ACF includes
#include <iprm/CSelectionParam.h>
#include <iprm/IParamsSet.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CMultiThreadingTest::initTestCase()
{
}


void CMultiThreadingTest::RunParallel4()
{
	class Loop : public QThread
	{
	public:
		Loop(CMultiThreadingComponentTest& test)
			:m_test(test)
		{
		}

		void run() override
		{
			iprm::ISelectionParam* selectionPtr = m_test.GetInterface<iprm::ISelectionParam>("Selection");
			iprm::ISelectionParam* selectionWithConstraintsPtr = m_test.GetInterface<iprm::ISelectionParam>("SelectionWithConstraints");
			iprm::IOptionsManager* selectionConstraintsPtr = m_test.GetInterface<iprm::IOptionsManager>("SelectionConstraints");

			QVERIFY(selectionPtr != nullptr);
			QVERIFY(selectionWithConstraintsPtr != nullptr);
			QVERIFY(selectionConstraintsPtr != nullptr);
		}

		private:
			CMultiThreadingComponentTest& m_test;
	};

	int count = 10;
	while (--count) {
		m_testPartituraInstanceCompPtr.reset(new CMultiThreadingComponentTest());
		m_testPartituraInstanceCompPtr->EnsureAutoInitComponentsCreated();

		Loop loop1(*m_testPartituraInstanceCompPtr);
		Loop loop2(*m_testPartituraInstanceCompPtr);
		Loop loop3(*m_testPartituraInstanceCompPtr);
		Loop loop4(*m_testPartituraInstanceCompPtr);
		Loop loop5(*m_testPartituraInstanceCompPtr);
		Loop loop6(*m_testPartituraInstanceCompPtr);
		Loop loop7(*m_testPartituraInstanceCompPtr);
		Loop loop8(*m_testPartituraInstanceCompPtr);
		Loop loop9(*m_testPartituraInstanceCompPtr);
		Loop loop10(*m_testPartituraInstanceCompPtr);
		Loop loop11(*m_testPartituraInstanceCompPtr);
		Loop loop12(*m_testPartituraInstanceCompPtr);
		Loop loop13(*m_testPartituraInstanceCompPtr);
		Loop loop14(*m_testPartituraInstanceCompPtr);
		Loop loop15(*m_testPartituraInstanceCompPtr);
		Loop loop16(*m_testPartituraInstanceCompPtr);

		loop1.start();
		loop2.start();
		loop3.start();
		loop4.start();
		loop5.start();
		loop6.start();
		loop7.start();
		loop8.start();
		loop9.start();
		loop10.start();
		loop11.start();
		loop12.start();
		loop13.start();
		loop14.start();
		loop15.start();
		loop16.start();

		loop1.wait();
		loop2.wait();
		loop3.wait();
		loop4.wait();
		loop5.wait();
		loop6.wait();
		loop7.wait();
		loop8.wait();
		loop9.wait();
		loop10.wait();
		loop11.wait();
		loop12.wait();
		loop13.wait();
		loop14.wait();
		loop15.wait();
		loop16.wait();
	}
}


void CMultiThreadingTest::CloneTest()
{
	iprm::IParamsSet* paramsSetPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IParamsSet>("ParamsSet");
	QVERIFY(paramsSetPtr != nullptr);

	istd::IChangeableUniquePtr ptr1 = paramsSetPtr->CloneMe(istd::IChangeable::CM_WITH_REFS);
	QVERIFY(ptr1.IsValid());

	istd::IChangeableUniquePtr ptr2 = ptr1->CloneMe(istd::IChangeable::CM_WITH_REFS);
	QVERIFY(ptr2.IsValid());

	iser::CMemoryWriteArchive archive1;
	QVERIFY(ptr1.GetPtr<iser::ISerializable>()->Serialize(archive1));

	iser::CMemoryWriteArchive archive2;
	QVERIFY(ptr2.GetPtr<iser::ISerializable>()->Serialize(archive2));

	QByteArray data1((const char*)archive1.GetBuffer(), archive1.GetBufferSize());
	QByteArray data2((const char*)archive2.GetBuffer(), archive2.GetBufferSize());
	QVERIFY(data1 == data2);

	ptr1 = paramsSetPtr->CloneMe();
	QVERIFY(ptr1.IsValid());

	ptr2 = ptr1->CloneMe();
	QVERIFY(ptr2.IsValid());

	QVERIFY(ptr1.GetPtr<iser::ISerializable>()->Serialize(archive1));
	QVERIFY(ptr2.GetPtr<iser::ISerializable>()->Serialize(archive2));

	QByteArray data3((const char*)archive1.GetBuffer(), archive1.GetBufferSize());
	QByteArray data4((const char*)archive2.GetBuffer(), archive2.GetBufferSize());
	QVERIFY(data3 == data4);
}


void CMultiThreadingTest::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CMultiThreadingTest);


