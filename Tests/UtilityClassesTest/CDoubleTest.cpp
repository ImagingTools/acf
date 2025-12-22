#include "CDoubleTest.h"


// protected slots

void CDoubleTest::initTestCase()
{
}


void CDoubleTest::ConstructorTest()
{
	// Test default constructor
	imath::CDouble value1;
	QVERIFY(value1 == 0.0);
	
	// Test constructor with value
	imath::CDouble value2(3.14);
	QVERIFY(qAbs(double(value2) - 3.14) < 0.001);
	
	// Test copy constructor
	imath::CDouble value3 = value2;
	QVERIFY(value3 == value2);
}


void CDoubleTest::OperatorsTest()
{
	imath::CDouble value1(10.0);
	imath::CDouble value2(5.0);
	
	// Test assignment
	imath::CDouble value3;
	value3 = value1;
	QVERIFY(value3 == value1);
	
	// Test double cast
	double d = value1;
	QVERIFY(d == 10.0);
}


void CDoubleTest::ComparisonOperatorsTest()
{
	imath::CDouble value1(10.0);
	imath::CDouble value2(5.0);
	imath::CDouble value3(10.0);
	
	// Test equality
	QVERIFY(value1 == value3);
	QVERIFY(value1 != value2);
	
	// Test less than / greater than
	QVERIFY(value2 < value1);
	QVERIFY(value1 > value2);
	
	// Test less than or equal / greater than or equal
	QVERIFY(value2 <= value1);
	QVERIFY(value1 <= value3);
	QVERIFY(value1 >= value3);
	QVERIFY(value1 >= value2);
}


void CDoubleTest::ArithmeticOperatorsTest()
{
	imath::CDouble value1(10.0);
	imath::CDouble value2(5.0);
	
	// Test addition
	imath::CDouble sum = value1 + value2;
	QVERIFY(sum == 15.0);
	
	// Test subtraction
	imath::CDouble diff = value1 - value2;
	QVERIFY(diff == 5.0);
	
	// Test multiplication
	imath::CDouble prod = value1 * value2;
	QVERIFY(prod == 50.0);
	
	// Test division
	imath::CDouble quot = value1 / value2;
	QVERIFY(quot == 2.0);
	
	// Test compound assignment operators
	imath::CDouble value3(10.0);
	value3 += value2;
	QVERIFY(value3 == 15.0);
	
	value3 -= value2;
	QVERIFY(value3 == 10.0);
	
	value3 *= value2;
	QVERIFY(value3 == 50.0);
	
	value3 /= value2;
	QVERIFY(value3 == 10.0);
}


void CDoubleTest::GetRoundedTest()
{
	imath::CDouble value1(3.14159);
	
	// Test rounding with different precisions
	imath::CDouble rounded2 = value1.GetRounded(2);
	QVERIFY(qAbs(double(rounded2) - 3.14) < 0.001);
	
	imath::CDouble rounded3 = value1.GetRounded(3);
	QVERIFY(qAbs(double(rounded3) - 3.142) < 0.0001);
	
	// Test static method
	double staticRounded = imath::CDouble::GetRounded(3.14159, 2);
	QVERIFY(qAbs(staticRounded - 3.14) < 0.001);
}


void CDoubleTest::IsRoundedEqualTest()
{
	imath::CDouble value1(3.14159);
	imath::CDouble value2(3.14151);
	
	// These should be equal when rounded to 2 decimal places
	QVERIFY(value1.IsRoundedEqual(value2, 2));
	
	// But not equal when rounded to 3 decimal places
	QVERIFY(!value1.IsRoundedEqual(value2, 3));
	
	// Test static method
	QVERIFY(imath::CDouble::IsRoundedEqual(3.14159, 3.14151, 2));
	QVERIFY(!imath::CDouble::IsRoundedEqual(3.14159, 3.14151, 3));
}


void CDoubleTest::GetRoundedDownTest()
{
	imath::CDouble value1(3.14159);
	
	// Test rounding down
	imath::CDouble roundedDown = value1.GetRoundedDown(2);
	QVERIFY(qAbs(double(roundedDown) - 3.14) < 0.001);
	
	imath::CDouble value2(3.19);
	imath::CDouble roundedDown2 = value2.GetRoundedDown(1);
	QVERIFY(qAbs(double(roundedDown2) - 3.1) < 0.01);
	
	// Test static method
	double staticRoundedDown = imath::CDouble::GetRoundedDown(3.14159, 2);
	QVERIFY(qAbs(staticRoundedDown - 3.14) < 0.001);
}


void CDoubleTest::GetRoundedUpTest()
{
	imath::CDouble value1(3.14159);
	
	// Test rounding up
	imath::CDouble roundedUp = value1.GetRoundedUp(2);
	QVERIFY(qAbs(double(roundedUp) - 3.15) < 0.001);
	
	imath::CDouble value2(3.11);
	imath::CDouble roundedUp2 = value2.GetRoundedUp(1);
	QVERIFY(qAbs(double(roundedUp2) - 3.2) < 0.01);
	
	// Test static method
	double staticRoundedUp = imath::CDouble::GetRoundedUp(3.14159, 2);
	QVERIFY(qAbs(staticRoundedUp - 3.15) < 0.001);
}


void CDoubleTest::IsSimiliarTest()
{
	imath::CDouble value1(3.14159);
	imath::CDouble value2(3.14161);
	
	// These should be similar with default tolerance
	QVERIFY(value1.IsSimiliar(value2));
	
	// Test with smaller tolerance
	QVERIFY(!value1.IsSimiliar(value2, 0.00001));
	
	// Test static method
	QVERIFY(imath::CDouble::IsSimiliar(3.14159, 3.14161));
	QVERIFY(!imath::CDouble::IsSimiliar(3.14159, 3.14161, 0.00001));
}


void CDoubleTest::cleanupTestCase()
{
}


I_ADD_TEST(CDoubleTest);
