#include "CVector2dTest.h"


// ACF includes
#include <istd/CIndex2d.h>


// protected slots

void CVector2dTest::initTestCase()
{
}


void CVector2dTest::ConstructorTest()
{
	// Test default constructor
	i2d::CVector2d vector1;
	
	// Test constructor with x,y values
	i2d::CVector2d vector2(3.0, 4.0);
	QVERIFY(qAbs(vector2.GetX() - 3.0) < 0.001);
	QVERIFY(qAbs(vector2.GetY() - 4.0) < 0.001);
	
	// Test copy constructor
	i2d::CVector2d vector3(vector2);
	QVERIFY(qAbs(vector3.GetX() - 3.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 4.0) < 0.001);
	
	// Test constructor from QPointF
	QPointF point(5.5, 6.5);
	i2d::CVector2d vector4(point);
	QVERIFY(qAbs(vector4.GetX() - 5.5) < 0.001);
	QVERIFY(qAbs(vector4.GetY() - 6.5) < 0.001);
	
	// Test constructor from CIndex2d
	istd::CIndex2d index(10, 20);
	i2d::CVector2d vector5(index);
	QVERIFY(qAbs(vector5.GetX() - 10.0) < 0.001);
	QVERIFY(qAbs(vector5.GetY() - 20.0) < 0.001);
}


void CVector2dTest::GetSetTest()
{
	i2d::CVector2d vector;
	
	// Test SetX and GetX
	vector.SetX(7.5);
	QVERIFY(qAbs(vector.GetX() - 7.5) < 0.001);
	
	// Test SetY and GetY
	vector.SetY(8.5);
	QVERIFY(qAbs(vector.GetY() - 8.5) < 0.001);
}


void CVector2dTest::ArithmeticOperatorsTest()
{
	i2d::CVector2d vector1(3.0, 4.0);
	i2d::CVector2d vector2(1.0, 2.0);
	
	// Test unary minus
	i2d::CVector2d negVector = -vector1;
	QVERIFY(qAbs(negVector.GetX() + 3.0) < 0.001);
	QVERIFY(qAbs(negVector.GetY() + 4.0) < 0.001);
	
	// Test addition
	i2d::CVector2d sum = vector1 + vector2;
	QVERIFY(qAbs(sum.GetX() - 4.0) < 0.001);
	QVERIFY(qAbs(sum.GetY() - 6.0) < 0.001);
	
	// Test subtraction
	i2d::CVector2d diff = vector1 - vector2;
	QVERIFY(qAbs(diff.GetX() - 2.0) < 0.001);
	QVERIFY(qAbs(diff.GetY() - 2.0) < 0.001);
	
	// Test scalar multiplication
	i2d::CVector2d scaled = vector1 * 2.0;
	QVERIFY(qAbs(scaled.GetX() - 6.0) < 0.001);
	QVERIFY(qAbs(scaled.GetY() - 8.0) < 0.001);
	
	// Test scalar division
	i2d::CVector2d divided = vector1 / 2.0;
	QVERIFY(qAbs(divided.GetX() - 1.5) < 0.001);
	QVERIFY(qAbs(divided.GetY() - 2.0) < 0.001);
	
	// Test compound operators
	i2d::CVector2d vector3(1.0, 1.0);
	vector3 += vector2;
	QVERIFY(qAbs(vector3.GetX() - 2.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 3.0) < 0.001);
	
	vector3 -= vector2;
	QVERIFY(qAbs(vector3.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 1.0) < 0.001);
	
	vector3 *= 3.0;
	QVERIFY(qAbs(vector3.GetX() - 3.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 3.0) < 0.001);
	
	vector3 /= 3.0;
	QVERIFY(qAbs(vector3.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vector3.GetY() - 1.0) < 0.001);
}


void CVector2dTest::GetTranslatedTest()
{
	i2d::CVector2d vector1(1.0, 2.0);
	i2d::CVector2d translation(3.0, 4.0);
	
	// Test general translation
	i2d::CVector2d translated = vector1.GetTranslated(translation);
	QVERIFY(qAbs(translated.GetX() - 4.0) < 0.001);
	QVERIFY(qAbs(translated.GetY() - 6.0) < 0.001);
	
	// Test horizontal translation
	i2d::CVector2d hTranslated = vector1.GetHorizontalTranslated(5.0);
	QVERIFY(qAbs(hTranslated.GetX() - 6.0) < 0.001);
	QVERIFY(qAbs(hTranslated.GetY() - 2.0) < 0.001);
	
	// Test vertical translation
	i2d::CVector2d vTranslated = vector1.GetVerticalTranslated(5.0);
	QVERIFY(qAbs(vTranslated.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(vTranslated.GetY() - 7.0) < 0.001);
}


void CVector2dTest::GetAngleTest()
{
	// Test angle of vector pointing right (0 degrees)
	i2d::CVector2d vector1(1.0, 0.0);
	double angle1 = vector1.GetAngle();
	QVERIFY(qAbs(angle1) < 0.001);
	
	// Test angle of vector pointing up (90 degrees = pi/2)
	i2d::CVector2d vector2(0.0, 1.0);
	double angle2 = vector2.GetAngle();
	QVERIFY(qAbs(angle2 - M_PI/2) < 0.001);
	
	// Test angle of vector pointing left (180 degrees = pi)
	i2d::CVector2d vector3(-1.0, 0.0);
	double angle3 = vector3.GetAngle();
	QVERIFY(qAbs(qAbs(angle3) - M_PI) < 0.001);
}


void CVector2dTest::GetOrthogonalTest()
{
	i2d::CVector2d vector(3.0, 4.0);
	i2d::CVector2d orthogonal = vector.GetOrthogonal();
	
	// Orthogonal vector should have dot product of 0
	double dotProduct = vector.GetDotProduct(orthogonal);
	QVERIFY(qAbs(dotProduct) < 0.001);
	
	// Orthogonal vector should have same length
	double length1 = vector.GetLength();
	double length2 = orthogonal.GetLength();
	QVERIFY(qAbs(length1 - length2) < 0.001);
}


void CVector2dTest::GetNormalizedTest()
{
	i2d::CVector2d vector(3.0, 4.0);
	
	// The length of (3,4) is 5
	i2d::CVector2d normalized = vector.GetNormalized();
	
	// Check that the normalized vector has length 1
	double length = normalized.GetLength();
	QVERIFY(qAbs(length - 1.0) < 0.001);
	
	// Test with custom length
	i2d::CVector2d normalized2 = vector.GetNormalized(10.0);
	double length2 = normalized2.GetLength();
	QVERIFY(qAbs(length2 - 10.0) < 0.001);
}


void CVector2dTest::GetCrossProductZTest()
{
	// Test cross product with standard unit vectors
	i2d::CVector2d xAxis(1.0, 0.0);
	i2d::CVector2d yAxis(0.0, 1.0);
	
	double crossZ = xAxis.GetCrossProductZ(yAxis);
	
	// X cross Y should give positive Z
	QVERIFY(crossZ > 0);
	QVERIFY(qAbs(crossZ - 1.0) < 0.001);
	
	// Y cross X should give negative Z
	double crossZ2 = yAxis.GetCrossProductZ(xAxis);
	QVERIFY(crossZ2 < 0);
	QVERIFY(qAbs(crossZ2 + 1.0) < 0.001);
}


void CVector2dTest::GetDotProductTest()
{
	// Test dot product of perpendicular vectors (should be 0)
	i2d::CVector2d xAxis(1.0, 0.0);
	i2d::CVector2d yAxis(0.0, 1.0);
	
	double dot1 = xAxis.GetDotProduct(yAxis);
	QVERIFY(qAbs(dot1) < 0.001);
	
	// Test dot product of parallel vectors
	i2d::CVector2d vector1(3.0, 4.0);
	i2d::CVector2d vector2(3.0, 4.0);
	
	double dot2 = vector1.GetDotProduct(vector2);
	// (3,4) dot (3,4) = 9 + 16 = 25
	QVERIFY(qAbs(dot2 - 25.0) < 0.001);
}


void CVector2dTest::cleanupTestCase()
{
}


I_ADD_TEST(CVector2dTest);
