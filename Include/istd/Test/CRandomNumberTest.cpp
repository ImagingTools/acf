#include "CRandomNumberTest.h"


// protected slots

void CRandomNumberTest::initTestCase()
{
	m_randomNumberPtr = new istd::CRandomNumber();
	QVERIFY(m_randomNumberPtr != nullptr);
}


void CRandomNumberTest::GetNextIntTest()
{
	// Test that GetNextInt returns a value
	int value = m_randomNumberPtr->GetNextInt(0, 100);
	
	// Verify the value is within range
	QVERIFY(value >= 0);
	QVERIFY(value <= 100);
}


void CRandomNumberTest::GetNextIntRangeTest()
{
	// Test with different ranges
	int value1 = m_randomNumberPtr->GetNextInt(10, 20);
	QVERIFY(value1 >= 10);
	QVERIFY(value1 <= 20);
	
	int value2 = m_randomNumberPtr->GetNextInt(-50, -10);
	QVERIFY(value2 >= -50);
	QVERIFY(value2 <= -10);
	
	// Test single value range
	int value3 = m_randomNumberPtr->GetNextInt(42, 42);
	QVERIFY(value3 == 42);
	
	// Generate multiple values to verify randomness
	bool allSame = true;
	int firstValue = m_randomNumberPtr->GetNextInt(0, 1000);
	for (int i = 0; i < 10; i++)
	{
		int value = m_randomNumberPtr->GetNextInt(0, 1000);
		if (value != firstValue)
		{
			allSame = false;
			break;
		}
	}
	// With a range of 1000, it's highly unlikely all 10 values are the same
	QVERIFY(!allSame);
}


void CRandomNumberTest::GetNextDoubleTest()
{
	// Test that GetNextDouble returns a value
	double value = m_randomNumberPtr->GetNextDouble(0.0, 1.0);
	
	// Verify the value is within range
	QVERIFY(value >= 0.0);
	QVERIFY(value <= 1.0);
}


void CRandomNumberTest::GetNextDoubleRangeTest()
{
	// Test with different ranges
	double value1 = m_randomNumberPtr->GetNextDouble(10.5, 20.5);
	QVERIFY(value1 >= 10.5);
	QVERIFY(value1 <= 20.5);
	
	double value2 = m_randomNumberPtr->GetNextDouble(-100.0, -50.0);
	QVERIFY(value2 >= -100.0);
	QVERIFY(value2 <= -50.0);
	
	// Generate multiple values to verify randomness
	bool allSame = true;
	double firstValue = m_randomNumberPtr->GetNextDouble(0.0, 1000.0);
	for (int i = 0; i < 10; i++)
	{
		double value = m_randomNumberPtr->GetNextDouble(0.0, 1000.0);
		if (qAbs(value - firstValue) > 0.001)
		{
			allSame = false;
			break;
		}
	}
	// With a range of 1000, it's highly unlikely all 10 values are the same
	QVERIFY(!allSame);
}


void CRandomNumberTest::cleanupTestCase()
{
	delete m_randomNumberPtr;
	m_randomNumberPtr = nullptr;
}


I_ADD_TEST(CRandomNumberTest);
