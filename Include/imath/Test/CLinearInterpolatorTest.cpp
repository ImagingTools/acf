#include "CLinearInterpolatorTest.h"


// protected slots

void CLinearInterpolatorTest::initTestCase()
{
}


void CLinearInterpolatorTest::BasicInterpolationTest()
{
	// Create simple linear function: y = x
	double positions[] = {0.0, 1.0, 2.0, 3.0};
	double values[] = {0.0, 1.0, 2.0, 3.0};
	
	imath::CLinearInterpolator interpolator(positions, values, 4, false);
	
	// Test exact node values
	double result = 0.0;
	QVERIFY(interpolator.GetValueAt(0.0, result));
	QVERIFY(qAbs(result - 0.0) < 0.001);
	
	QVERIFY(interpolator.GetValueAt(1.0, result));
	QVERIFY(qAbs(result - 1.0) < 0.001);
	
	QVERIFY(interpolator.GetValueAt(2.0, result));
	QVERIFY(qAbs(result - 2.0) < 0.001);
	
	// Test interpolated values
	QVERIFY(interpolator.GetValueAt(0.5, result));
	QVERIFY(qAbs(result - 0.5) < 0.001);
	
	QVERIFY(interpolator.GetValueAt(1.5, result));
	QVERIFY(qAbs(result - 1.5) < 0.001);
	
	QVERIFY(interpolator.GetValueAt(2.5, result));
	QVERIFY(qAbs(result - 2.5) < 0.001);
}


void CLinearInterpolatorTest::ExtrapolationTest()
{
	// Create simple linear function
	double positions[] = {1.0, 2.0, 3.0};
	double values[] = {10.0, 20.0, 30.0};
	
	// Test with extrapolation disabled
	imath::CLinearInterpolator interpolator1(positions, values, 3, false);
	
	double result = 0.0;
	
	// Values outside range should return boundary values when extrapolation is disabled
	QVERIFY(interpolator1.GetValueAt(0.5, result));
	QVERIFY(qAbs(result - 10.0) < 0.001);  // Should return first value
	
	QVERIFY(interpolator1.GetValueAt(3.5, result));
	QVERIFY(qAbs(result - 30.0) < 0.001);  // Should return last value
	
	// Test with extrapolation enabled
	imath::CLinearInterpolator interpolator2(positions, values, 3, true);
	
	// Values outside range should be extrapolated
	QVERIFY(interpolator2.GetValueAt(0.5, result));
	QVERIFY(qAbs(result - 5.0) < 0.001);  // Extrapolated: 10 - 0.5*10 = 5
	
	QVERIFY(interpolator2.GetValueAt(4.0, result));
	QVERIFY(qAbs(result - 40.0) < 0.001);  // Extrapolated: 30 + 1.0*10 = 40
}


void CLinearInterpolatorTest::EdgeCasesTest()
{
	// Test with two nodes
	double positions[] = {0.0, 10.0};
	double values[] = {0.0, 100.0};
	
	imath::CLinearInterpolator interpolator(positions, values, 2, false);
	
	double result = 0.0;
	
	// Test midpoint
	QVERIFY(interpolator.GetValueAt(5.0, result));
	QVERIFY(qAbs(result - 50.0) < 0.001);
	
	// Test 25% point
	QVERIFY(interpolator.GetValueAt(2.5, result));
	QVERIFY(qAbs(result - 25.0) < 0.001);
	
	// Test 75% point
	QVERIFY(interpolator.GetValueAt(7.5, result));
	QVERIFY(qAbs(result - 75.0) < 0.001);
}


void CLinearInterpolatorTest::cleanupTestCase()
{
}


I_ADD_TEST(CLinearInterpolatorTest);
