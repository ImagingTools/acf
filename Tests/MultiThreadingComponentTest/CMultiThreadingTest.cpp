#include "CMultiThreadingTest.h"


// ACF includes
#include <iprm/CSelectionParam.h>
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

	int count = 100;
	while (--count) {
		m_testPartituraInstanceCompPtr.reset(new CMultiThreadingComponentTest());
		m_testPartituraInstanceCompPtr->EnsureAutoInitComponentsCreated();

		Loop loop1(*m_testPartituraInstanceCompPtr);
		Loop loop2(*m_testPartituraInstanceCompPtr);
		Loop loop3(*m_testPartituraInstanceCompPtr);
		Loop loop4(*m_testPartituraInstanceCompPtr);

		loop1.start();
		loop2.start();
		loop3.start();
		loop4.start();

		loop1.wait();
		loop2.wait();
		loop3.wait();
		loop4.wait();
	}
}



void CMultiThreadingTest::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CMultiThreadingTest);


