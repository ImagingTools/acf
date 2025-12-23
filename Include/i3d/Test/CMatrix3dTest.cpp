#include "CMatrix3dTest.h"


// protected slots

void CMatrix3dTest::initTestCase()
{
}


void CMatrix3dTest::ConstructorTest()
{
	// Test default constructor
	i3d::CMatrix3d matrix1;
	
	// Test constructor with vectors
	i3d::CVector3d axisX(1.0, 0.0, 0.0);
	i3d::CVector3d axisY(0.0, 1.0, 0.0);
	i3d::CVector3d axisZ(0.0, 0.0, 1.0);
	i3d::CMatrix3d matrix2(axisX, axisY, axisZ);
	
	// Verify identity-like construction
	QVERIFY(qAbs(matrix2.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix2.GetAt(1, 1) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix2.GetAt(2, 2) - 1.0) < 0.001);
	
	// Test constructor with explicit values
	i3d::CMatrix3d matrix3(
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	);
	QVERIFY(qAbs(matrix3.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix3.GetAt(0, 1) - 2.0) < 0.001);
	QVERIFY(qAbs(matrix3.GetAt(2, 2) - 9.0) < 0.001);
	
	// Test copy constructor
	i3d::CMatrix3d matrix4(matrix3);
	QVERIFY(qAbs(matrix4.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix4.GetAt(2, 2) - 9.0) < 0.001);
}


void CMatrix3dTest::ResetTest()
{
	i3d::CMatrix3d matrix(
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	);
	
	// Reset to identity
	matrix.Reset();
	
	// Verify it's an identity matrix
	QVERIFY(qAbs(matrix.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix.GetAt(1, 1) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix.GetAt(2, 2) - 1.0) < 0.001);
	QVERIFY(qAbs(matrix.GetAt(0, 1)) < 0.001);
	QVERIFY(qAbs(matrix.GetAt(0, 2)) < 0.001);
	QVERIFY(qAbs(matrix.GetAt(1, 0)) < 0.001);
}


void CMatrix3dTest::GetIdentityTest()
{
	const i3d::CMatrix3d& identity = i3d::CMatrix3d::GetIdentity();
	
	// Verify it's an identity matrix
	QVERIFY(qAbs(identity.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(identity.GetAt(1, 1) - 1.0) < 0.001);
	QVERIFY(qAbs(identity.GetAt(2, 2) - 1.0) < 0.001);
	QVERIFY(qAbs(identity.GetAt(0, 1)) < 0.001);
	QVERIFY(qAbs(identity.GetAt(0, 2)) < 0.001);
	QVERIFY(qAbs(identity.GetAt(1, 0)) < 0.001);
	QVERIFY(qAbs(identity.GetAt(1, 2)) < 0.001);
	QVERIFY(qAbs(identity.GetAt(2, 0)) < 0.001);
	QVERIFY(qAbs(identity.GetAt(2, 1)) < 0.001);
}


void CMatrix3dTest::GetMultipliedVectorTest()
{
	// Create identity matrix
	i3d::CMatrix3d identity = i3d::CMatrix3d::GetIdentity();
	i3d::CVector3d vector(1.0, 2.0, 3.0);
	
	// Identity matrix should not change the vector
	i3d::CVector3d result = identity.GetMultiplied(vector);
	QVERIFY(qAbs(result.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(result.GetY() - 2.0) < 0.001);
	QVERIFY(qAbs(result.GetZ() - 3.0) < 0.001);
	
	// Create a scaling matrix
	i3d::CMatrix3d scale(
		2.0, 0.0, 0.0,
		0.0, 2.0, 0.0,
		0.0, 0.0, 2.0
	);
	
	i3d::CVector3d scaled = scale.GetMultiplied(vector);
	QVERIFY(qAbs(scaled.GetX() - 2.0) < 0.001);
	QVERIFY(qAbs(scaled.GetY() - 4.0) < 0.001);
	QVERIFY(qAbs(scaled.GetZ() - 6.0) < 0.001);
}


void CMatrix3dTest::GetMultipliedMatrixTest()
{
	i3d::CMatrix3d identity = i3d::CMatrix3d::GetIdentity();
	i3d::CMatrix3d matrix(
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	);
	
	// Multiply by identity should give original matrix
	i3d::CMatrix3d result = identity.GetMultiplied(matrix);
	QVERIFY(qAbs(result.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(result.GetAt(1, 1) - 5.0) < 0.001);
	QVERIFY(qAbs(result.GetAt(2, 2) - 9.0) < 0.001);
}


void CMatrix3dTest::GetAxisTest()
{
	i3d::CMatrix3d matrix(
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	);
	
	// Test GetAxisX
	i3d::CVector3d axisX = matrix.GetAxisX();
	QVERIFY(qAbs(axisX.GetX() - 1.0) < 0.001);
	QVERIFY(qAbs(axisX.GetY() - 2.0) < 0.001);
	QVERIFY(qAbs(axisX.GetZ() - 3.0) < 0.001);
	
	// Test GetAxisY
	i3d::CVector3d axisY = matrix.GetAxisY();
	QVERIFY(qAbs(axisY.GetX() - 4.0) < 0.001);
	QVERIFY(qAbs(axisY.GetY() - 5.0) < 0.001);
	QVERIFY(qAbs(axisY.GetZ() - 6.0) < 0.001);
	
	// Test GetAxisZ
	i3d::CVector3d axisZ = matrix.GetAxisZ();
	QVERIFY(qAbs(axisZ.GetX() - 7.0) < 0.001);
	QVERIFY(qAbs(axisZ.GetY() - 8.0) < 0.001);
	QVERIFY(qAbs(axisZ.GetZ() - 9.0) < 0.001);
}


void CMatrix3dTest::GetTransposedTest()
{
	i3d::CMatrix3d matrix(
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	);
	
	i3d::CMatrix3d transposed = matrix.GetTransposed();
	
	// Verify transpose: rows become columns
	QVERIFY(qAbs(transposed.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(transposed.GetAt(1, 0) - 2.0) < 0.001);
	QVERIFY(qAbs(transposed.GetAt(2, 0) - 3.0) < 0.001);
	QVERIFY(qAbs(transposed.GetAt(0, 1) - 4.0) < 0.001);
	QVERIFY(qAbs(transposed.GetAt(1, 1) - 5.0) < 0.001);
	QVERIFY(qAbs(transposed.GetAt(2, 1) - 6.0) < 0.001);
}


void CMatrix3dTest::GetDeterminantTest()
{
	// Test determinant of identity matrix (should be 1)
	i3d::CMatrix3d identity = i3d::CMatrix3d::GetIdentity();
	double det = identity.GetDet();
	QVERIFY(qAbs(det - 1.0) < 0.001);
	
	// Test determinant of a known matrix
	i3d::CMatrix3d matrix(
		1.0, 0.0, 0.0,
		0.0, 2.0, 0.0,
		0.0, 0.0, 3.0
	);
	double det2 = matrix.GetDet();
	// Determinant of diagonal matrix is product of diagonal elements: 1*2*3 = 6
	QVERIFY(qAbs(det2 - 6.0) < 0.001);
}


void CMatrix3dTest::ScalarOperatorsTest()
{
	i3d::CMatrix3d matrix(
		2.0, 4.0, 6.0,
		8.0, 10.0, 12.0,
		14.0, 16.0, 18.0
	);
	
	// Test multiplication by scalar
	i3d::CMatrix3d scaled = matrix * 2.0;
	QVERIFY(qAbs(scaled.GetAt(0, 0) - 4.0) < 0.001);
	QVERIFY(qAbs(scaled.GetAt(1, 1) - 20.0) < 0.001);
	QVERIFY(qAbs(scaled.GetAt(2, 2) - 36.0) < 0.001);
	
	// Test division by scalar
	i3d::CMatrix3d divided = matrix / 2.0;
	QVERIFY(qAbs(divided.GetAt(0, 0) - 1.0) < 0.001);
	QVERIFY(qAbs(divided.GetAt(1, 1) - 5.0) < 0.001);
	QVERIFY(qAbs(divided.GetAt(2, 2) - 9.0) < 0.001);
}


void CMatrix3dTest::cleanupTestCase()
{
}


I_ADD_TEST(CMatrix3dTest);
