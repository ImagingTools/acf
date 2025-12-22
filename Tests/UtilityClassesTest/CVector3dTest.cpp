#include "CVector3dTest.h"


// ACF includes
#include <i2d/CVector2d.h>


// protected slots

void CVector3dTest::initTestCase()
{
}


void CVector3dTest::ConstructorTest()
{
	// Test default constructor
	i3d::CVector3d vector1;
	
	// Test constructor with x,y,z values
	i3d::CVector3d vector2(1.0, 2.0, 3.0);
	QVERIFY(qAbs(vector2.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector2.GetY() - 2.0) < 0.001);
	QVERIFY(qAbs(vector2.GetZ() - 3.0) < 0.001);
	
	// Test copy constructor
	i3d::CVector3d vector3(vector2);
	QVERIFY(qAbs(vector3.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 2.0) < 0.001);
	QVERIFY(qAbs(vector3.GetZ() - 3.0) < 0.001);
	
	// Test constructor from 2D vector
	i2d::CVector2d vector2d(4.0, 5.0);
	i3d::CVector3d vector4(vector2d, 6.0);
	QVERIFY(qAbs(vector4.GetX() - 4.0) < 0.001);
	QVERIFY(qAbs(vector4.GetY() - 5.0) < 0.001);
	QVERIFY(qAbs(vector4.GetZ() - 6.0) < 0.001);
}


void CVector3dTest::GetSetTest()
{
	i3d::CVector3d vector;
	
	// Test SetX and GetX
	vector.SetX(10.5);
	QVERIFY(qAbs(vector.GetX() - 10.5) < 0.001);
	
	// Test SetY and GetY
	vector.SetY(20.5);
	QVERIFY(qAbs(vector.GetY() - 20.5) < 0.001);
	
	// Test SetZ and GetZ
	vector.SetZ(30.5);
	QVERIFY(qAbs(vector.GetZ() - 30.5) < 0.001);
}


void CVector3dTest::ArithmeticOperatorsTest()
{
	i3d::CVector3d vector1(1.0, 2.0, 3.0);
	i3d::CVector3d vector2(4.0, 5.0, 6.0);
	
	// Test unary minus
	i3d::CVector3d negVector = -vector1;
	QVERIFY(qAbs(negVector.GetX() + 1.0) < 0.001);
	QVERIFY(qAbs(negVector.GetY() + 2.0) < 0.001);
	QVERIFY(qAbs(negVector.GetZ() + 3.0) < 0.001);
	
	// Test addition
	i3d::CVector3d sum = vector1 + vector2;
	QVERIFY(qAbs(sum.GetX() - 5.0) < 0.001);
	QVERIFY(qAbs(sum.GetY() - 7.0) < 0.001);
	QVERIFY(qAbs(sum.GetZ() - 9.0) < 0.001);
	
	// Test subtraction
	i3d::CVector3d diff = vector2 - vector1;
	QVERIFY(qAbs(diff.GetX() - 3.0) < 0.001);
	QVERIFY(qAbs(diff.GetY() - 3.0) < 0.001);
	QVERIFY(qAbs(diff.GetZ() - 3.0) < 0.001);
	
	// Test scalar multiplication
	i3d::CVector3d scaled = vector1 * 2.0;
	QVERIFY(qAbs(scaled.GetX() - 2.0) < 0.001);
	QVERIFY(qAbs(scaled.GetY() - 4.0) < 0.001);
	QVERIFY(qAbs(scaled.GetZ() - 6.0) < 0.001);
	
	// Test scalar division
	i3d::CVector3d divided = vector2 / 2.0;
	QVERIFY(qAbs(divided.GetX() - 2.0) < 0.001);
	QVERIFY(qAbs(divided.GetY() - 2.5) < 0.001);
	QVERIFY(qAbs(divided.GetZ() - 3.0) < 0.001);
	
	// Test compound operators
	i3d::CVector3d vector3(1.0, 2.0, 3.0);
	vector3 += vector2;
	QVERIFY(qAbs(vector3.GetX() - 5.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 7.0) < 0.001);
	QVERIFY(qAbs(vector3.GetZ() - 9.0) < 0.001);
	
	vector3 -= vector2;
	QVERIFY(qAbs(vector3.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 2.0) < 0.001);
	QVERIFY(qAbs(vector3.GetZ() - 3.0) < 0.001);
	
	vector3 *= 3.0;
	QVERIFY(qAbs(vector3.GetX() - 3.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 6.0) < 0.001);
	QVERIFY(qAbs(vector3.GetZ() - 9.0) < 0.001);
	
	vector3 /= 3.0;
	QVERIFY(qAbs(vector3.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 2.0) < 0.001);
	QVERIFY(qAbs(vector3.GetZ() - 3.0) < 0.001);
}


void CVector3dTest::GetTranslatedTest()
{
	i3d::CVector3d vector1(1.0, 2.0, 3.0);
	i3d::CVector3d translation(4.0, 5.0, 6.0);
	
	i3d::CVector3d translated = vector1.GetTranslated(translation);
	QVERIFY(qAbs(translated.GetX() - 5.0) < 0.001);
	QVERIFY(qAbs(translated.GetY() - 7.0) < 0.001);
	QVERIFY(qAbs(translated.GetZ() - 9.0) < 0.001);
}


void CVector3dTest::GetCrossProductTest()
{
	// Test cross product with standard unit vectors
	i3d::CVector3d xAxis(1.0, 0.0, 0.0);
	i3d::CVector3d yAxis(0.0, 1.0, 0.0);
	
	i3d::CVector3d crossProduct = xAxis.GetCrossProduct(yAxis);
	
	// X cross Y should give Z axis
	QVERIFY(qAbs(crossProduct.GetX() - 0.0) < 0.001);
	QVERIFY(qAbs(crossProduct.GetY() - 0.0) < 0.001);
	QVERIFY(qAbs(crossProduct.GetZ() - 1.0) < 0.001);
}


void CVector3dTest::GetNormalizedTest()
{
	i3d::CVector3d vector(3.0, 4.0, 0.0);
	
	// The length of (3,4,0) is 5
	i3d::CVector3d normalized = vector.GetNormalized();
	
	// Check that the normalized vector has length 1
	double length = qSqrt(normalized.GetX() * normalized.GetX() + 
	                      normalized.GetY() * normalized.GetY() + 
	                      normalized.GetZ() * normalized.GetZ());
	QVERIFY(qAbs(length - 1.0) < 0.001);
	
	// Test with custom length
	i3d::CVector3d normalized2 = vector.GetNormalized(10.0);
	double length2 = qSqrt(normalized2.GetX() * normalized2.GetX() + 
	                       normalized2.GetY() * normalized2.GetY() + 
	                       normalized2.GetZ() * normalized2.GetZ());
	QVERIFY(qAbs(length2 - 10.0) < 0.001);
}


void CVector3dTest::GetPlaneCastedTest()
{
	i3d::CVector3d vector(1.0, 2.0, 3.0);
	
	i2d::CVector2d vector2d = vector.GetPlaneCasted();
	QVERIFY(qAbs(vector2d.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector2d.GetY() - 2.0) < 0.001);
}


void CVector3dTest::cleanupTestCase()
{
}


I_ADD_TEST(CVector3dTest);
