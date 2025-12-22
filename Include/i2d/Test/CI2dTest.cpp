#include <i2d/Test/CI2dTest.h>


// Standard includes
#include <cmath>

// ACF includes
#include <istd/istd.h>
#include <i2d/CVector2d.h>
#include <i2d/CPosition2d.h>
#include <i2d/CCircle.h>
#include <i2d/CRectangle.h>
#include <i2d/CLine2d.h>
#include <i2d/CMatrix2d.h>
#include <i2d/CAffine2d.h>
#include <i2d/CAnnulus.h>
#include <i2d/CPolyline.h>
#include <i2d/CPolygon.h>
#include <itest/CStandardTestExecutor.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void CI2dTest::DoVector2dTest()
{
	// Test default constructor
	i2d::CVector2d v1;
	v1.SetX(3.0);
	v1.SetY(4.0);
	QVERIFY(qAbs(v1.GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(v1.GetY() - 4.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CVector2d v2(3.0, 4.0);
	QVERIFY(qAbs(v2.GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(v2.GetY() - 4.0) < I_EPSILON);

	// Test copy constructor
	i2d::CVector2d v3(v2);
	QVERIFY(qAbs(v3.GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(v3.GetY() - 4.0) < I_EPSILON);

	// Test length calculation
	double length = v2.GetLength();
	QVERIFY(qAbs(length - 5.0) < I_EPSILON);

	// Test angle calculation
	double angle = v2.GetAngle();
	QVERIFY(qAbs(angle - qAtan2(4.0, 3.0)) < I_EPSILON);

	// Test normalization
	i2d::CVector2d normalized = v2.GetNormalized();
	QVERIFY(qAbs(normalized.GetLength() - 1.0) < I_EPSILON);

	// Test normalization with custom length
	i2d::CVector2d normalized10 = v2.GetNormalized(10.0);
	QVERIFY(qAbs(normalized10.GetLength() - 10.0) < I_EPSILON);

	// Test addition
	i2d::CVector2d v4(1.0, 2.0);
	i2d::CVector2d v5 = v2 + v4;
	QVERIFY(qAbs(v5.GetX() - 4.0) < I_EPSILON);
	QVERIFY(qAbs(v5.GetY() - 6.0) < I_EPSILON);

	// Test subtraction
	i2d::CVector2d v6 = v2 - v4;
	QVERIFY(qAbs(v6.GetX() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(v6.GetY() - 2.0) < I_EPSILON);

	// Test multiplication
	i2d::CVector2d v7 = v2 * 2.0;
	QVERIFY(qAbs(v7.GetX() - 6.0) < I_EPSILON);
	QVERIFY(qAbs(v7.GetY() - 8.0) < I_EPSILON);

	// Test division
	i2d::CVector2d v8 = v2 / 2.0;
	QVERIFY(qAbs(v8.GetX() - 1.5) < I_EPSILON);
	QVERIFY(qAbs(v8.GetY() - 2.0) < I_EPSILON);

	// Test negation
	i2d::CVector2d v9 = -v2;
	QVERIFY(qAbs(v9.GetX() + 3.0) < I_EPSILON);
	QVERIFY(qAbs(v9.GetY() + 4.0) < I_EPSILON);

	// Test orthogonal vector
	i2d::CVector2d ortho = v2.GetOrthogonal();
	QVERIFY(qAbs(ortho.GetX() - 4.0) < I_EPSILON);
	QVERIFY(qAbs(ortho.GetY() + 3.0) < I_EPSILON);

	// Test dot product
	double dot = v2.GetDotProduct(v4);
	QVERIFY(qAbs(dot - 11.0) < I_EPSILON); // 3*1 + 4*2 = 11

	// Test cross product Z
	double crossZ = v2.GetCrossProductZ(v4);
	QVERIFY(qAbs(crossZ - 2.0) < I_EPSILON); // 3*2 - 4*1 = 2

	// Test translation
	i2d::CVector2d translated = v2.GetTranslated(v4);
	QVERIFY(qAbs(translated.GetX() - 4.0) < I_EPSILON);
	QVERIFY(qAbs(translated.GetY() - 6.0) < I_EPSILON);

	// Test horizontal translation
	i2d::CVector2d hTranslated = v2.GetHorizontalTranslated(2.0);
	QVERIFY(qAbs(hTranslated.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(hTranslated.GetY() - 4.0) < I_EPSILON);

	// Test vertical translation
	i2d::CVector2d vTranslated = v2.GetVerticalTranslated(3.0);
	QVERIFY(qAbs(vTranslated.GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(vTranslated.GetY() - 7.0) < I_EPSILON);

	// Test GetZero
	const i2d::CVector2d& zero = i2d::CVector2d::GetZero();
	QVERIFY(qAbs(zero.GetX()) < I_EPSILON);
	QVERIFY(qAbs(zero.GetY()) < I_EPSILON);

	// Test Init with angle
	i2d::CVector2d v10;
	v10.Init(0.0, 5.0);
	QVERIFY(qAbs(v10.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(v10.GetY()) < I_EPSILON);

	// Test QPointF conversion
	QPointF point = v2;
	QVERIFY(qAbs(point.x() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(point.y() - 4.0) < I_EPSILON);

	// Test construction from QPointF
	i2d::CVector2d v11(QPointF(5.0, 6.0));
	QVERIFY(qAbs(v11.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(v11.GetY() - 6.0) < I_EPSILON);
}


void CI2dTest::DoPosition2dTest()
{
	// Test default constructor
	i2d::CPosition2d pos1;
	pos1.SetPosition(i2d::CVector2d(10.0, 20.0));
	QVERIFY(qAbs(pos1.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(pos1.GetPosition().GetY() - 20.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CPosition2d pos2(i2d::CVector2d(15.0, 25.0));
	QVERIFY(qAbs(pos2.GetPosition().GetX() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(pos2.GetPosition().GetY() - 25.0) < I_EPSILON);

	// Test GetX and GetY
	QVERIFY(qAbs(pos2.GetX() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(pos2.GetY() - 25.0) < I_EPSILON);

	// Test SetX and SetY
	pos1.SetX(30.0);
	pos1.SetY(40.0);
	QVERIFY(qAbs(pos1.GetX() - 30.0) < I_EPSILON);
	QVERIFY(qAbs(pos1.GetY() - 40.0) < I_EPSILON);
}


void CI2dTest::DoCircleTest()
{
	// Test default constructor
	i2d::CCircle circle1;
	circle1.SetRadius(5.0);
	circle1.SetPosition(i2d::CVector2d(10.0, 10.0));
	QVERIFY(qAbs(circle1.GetRadius() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(circle1.GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(circle1.GetY() - 10.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CCircle circle2(7.0, i2d::CVector2d(20.0, 30.0));
	QVERIFY(qAbs(circle2.GetRadius() - 7.0) < I_EPSILON);
	QVERIFY(qAbs(circle2.GetX() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(circle2.GetY() - 30.0) < I_EPSILON);

	// Test bounding box
	i2d::CRectangle bbox = circle2.GetBoundingBox();
	QVERIFY(qAbs(bbox.GetPosition().GetX() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetPosition().GetY() - 30.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetWidth() - 14.0) < I_EPSILON);  // 2 * radius
	QVERIFY(qAbs(bbox.GetHeight() - 14.0) < I_EPSILON); // 2 * radius

	// Test equality operator
	i2d::CCircle circle3(7.0, i2d::CVector2d(20.0, 30.0));
	QVERIFY(circle2 == circle3);

	// Test inequality operator
	i2d::CCircle circle4(8.0, i2d::CVector2d(20.0, 30.0));
	QVERIFY(circle2 != circle4);

	// Test circle-circle intersection (not intersecting)
	i2d::CCircle circle5(5.0, i2d::CVector2d(0.0, 0.0));
	i2d::CCircle circle6(5.0, i2d::CVector2d(20.0, 0.0));
	QVERIFY(!circle5.IsIntersectedBy(circle6, false));

	// Test circle-circle intersection (intersecting)
	i2d::CCircle circle7(5.0, i2d::CVector2d(0.0, 0.0));
	i2d::CCircle circle8(5.0, i2d::CVector2d(8.0, 0.0));
	QVERIFY(circle7.IsIntersectedBy(circle8, false));

	// Test circle-line intersection
	i2d::CCircle circle9(5.0, i2d::CVector2d(0.0, 0.0));
	i2d::CLine2d line1(i2d::CVector2d(-10.0, 0.0), i2d::CVector2d(10.0, 0.0));
	QVERIFY(circle9.IsIntersectedBy(line1, false));

	// Test line not intersecting circle
	i2d::CLine2d line2(i2d::CVector2d(-10.0, 10.0), i2d::CVector2d(10.0, 10.0));
	QVERIFY(!circle9.IsIntersectedBy(line2, false));
}


void CI2dTest::DoRectangleTest()
{
	// Test default constructor
	i2d::CRectangle rect1;
	rect1.SetPosition(i2d::CVector2d(10.0, 20.0));
	rect1.SetWidth(30.0);
	rect1.SetHeight(40.0);
	QVERIFY(qAbs(rect1.GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(rect1.GetY() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(rect1.GetWidth() - 30.0) < I_EPSILON);
	QVERIFY(qAbs(rect1.GetHeight() - 40.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CRectangle rect2(i2d::CVector2d(5.0, 10.0), 20.0, 25.0);
	QVERIFY(qAbs(rect2.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetY() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetWidth() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetHeight() - 25.0) < I_EPSILON);

	// Test GetLeft, GetRight, GetTop, GetBottom
	QVERIFY(qAbs(rect2.GetLeft() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetRight() - 25.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetTop() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetBottom() - 35.0) < I_EPSILON);

	// Test corner getters
	i2d::CVector2d topLeft = rect2.GetTopLeft();
	QVERIFY(qAbs(topLeft.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(topLeft.GetY() - 10.0) < I_EPSILON);

	i2d::CVector2d bottomRight = rect2.GetBottomRight();
	QVERIFY(qAbs(bottomRight.GetX() - 25.0) < I_EPSILON);
	QVERIFY(qAbs(bottomRight.GetY() - 35.0) < I_EPSILON);

	// Test center
	i2d::CVector2d center = rect2.GetCenter();
	QVERIFY(qAbs(center.GetX() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(center.GetY() - 22.5) < I_EPSILON);

	// Test equality operator
	i2d::CRectangle rect3(i2d::CVector2d(5.0, 10.0), 20.0, 25.0);
	QVERIFY(rect2 == rect3);

	// Test inequality operator
	i2d::CRectangle rect4(i2d::CVector2d(5.0, 10.0), 21.0, 25.0);
	QVERIFY(rect2 != rect4);
}


void CI2dTest::DoLine2dTest()
{
	// Test default constructor
	i2d::CLine2d line1;
	line1.SetPoint1(i2d::CVector2d(0.0, 0.0));
	line1.SetPoint2(i2d::CVector2d(10.0, 10.0));
	QVERIFY(qAbs(line1.GetPoint1().GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(line1.GetPoint1().GetY() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(line1.GetPoint2().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(line1.GetPoint2().GetY() - 10.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CLine2d line2(i2d::CVector2d(5.0, 5.0), i2d::CVector2d(15.0, 20.0));
	QVERIFY(qAbs(line2.GetPoint1().GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(line2.GetPoint1().GetY() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(line2.GetPoint2().GetX() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(line2.GetPoint2().GetY() - 20.0) < I_EPSILON);

	// Test length calculation
	double length = line2.GetLength();
	double expectedLength = qSqrt((15.0 - 5.0) * (15.0 - 5.0) + (20.0 - 5.0) * (20.0 - 5.0));
	QVERIFY(qAbs(length - expectedLength) < I_EPSILON);

	// Test difference vector
	i2d::CVector2d diffVector = line2.GetDiffVector();
	QVERIFY(qAbs(diffVector.GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(diffVector.GetY() - 15.0) < I_EPSILON);

	// Test bounding box
	i2d::CRectangle bbox = line2.GetBoundingBox();
	QVERIFY(qAbs(bbox.GetLeft() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetRight() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetTop() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetBottom() - 20.0) < I_EPSILON);

	// Test equality operator
	i2d::CLine2d line3(i2d::CVector2d(5.0, 5.0), i2d::CVector2d(15.0, 20.0));
	QVERIFY(line2 == line3);

	// Test inequality operator
	i2d::CLine2d line4(i2d::CVector2d(5.0, 5.0), i2d::CVector2d(16.0, 20.0));
	QVERIFY(line2 != line4);
}


void CI2dTest::DoMatrix2dTest()
{
	// Test default constructor (identity matrix)
	i2d::CMatrix2d mat1;
	mat1.Reset(); // Initialize to identity
	QVERIFY(qAbs(mat1.GetAt(0, 0) - 1.0) < I_EPSILON);
	QVERIFY(qAbs(mat1.GetAt(0, 1) - 0.0) < I_EPSILON);
	QVERIFY(qAbs(mat1.GetAt(1, 0) - 0.0) < I_EPSILON);
	QVERIFY(qAbs(mat1.GetAt(1, 1) - 1.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CMatrix2d mat2(2.0, 0.0, 0.0, 3.0);
	QVERIFY(qAbs(mat2.GetAt(0, 0) - 2.0) < I_EPSILON);
	QVERIFY(qAbs(mat2.GetAt(0, 1) - 0.0) < I_EPSILON);
	QVERIFY(qAbs(mat2.GetAt(1, 0) - 0.0) < I_EPSILON);
	QVERIFY(qAbs(mat2.GetAt(1, 1) - 3.0) < I_EPSILON);

	// Test Reset to identity
	mat2.Reset();
	QVERIFY(qAbs(mat2.GetAt(0, 0) - 1.0) < I_EPSILON);
	QVERIFY(qAbs(mat2.GetAt(1, 1) - 1.0) < I_EPSILON);

	// Test rotation matrix
	i2d::CMatrix2d rotMat;
	rotMat.Reset(M_PI / 2.0); // 90 degrees
	
	// Rotate vector (1, 0) by 90 degrees should give approximately (0, 1)
	i2d::CVector2d v1(1.0, 0.0);
	i2d::CVector2d v2 = rotMat.GetMultiplied(v1);
	QVERIFY(qAbs(v2.GetX()) < I_BIG_EPSILON);
	QVERIFY(qAbs(v2.GetY() - 1.0) < I_BIG_EPSILON);

	// Test scaling matrix
	i2d::CMatrix2d scaleMat;
	scaleMat.Reset(0.0, i2d::CVector2d(2.0, 3.0)); // No rotation, scale by (2, 3)
	i2d::CVector2d v3(1.0, 1.0);
	i2d::CVector2d v4 = scaleMat.GetMultiplied(v3);
	QVERIFY(qAbs(v4.GetX() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(v4.GetY() - 3.0) < I_EPSILON);

	// Test matrix multiplication
	i2d::CMatrix2d mat3(2.0, 0.0, 0.0, 2.0);
	i2d::CMatrix2d mat4(3.0, 0.0, 0.0, 3.0);
	i2d::CMatrix2d result = mat3.GetMultiplied(mat4);
	QVERIFY(qAbs(result.GetAt(0, 0) - 6.0) < I_EPSILON);
	QVERIFY(qAbs(result.GetAt(1, 1) - 6.0) < I_EPSILON);

	// Test determinant
	i2d::CMatrix2d mat5(2.0, 0.0, 0.0, 3.0);
	double det = mat5.GetDet();
	QVERIFY(qAbs(det - 6.0) < I_EPSILON);

	// Test inverse
	i2d::CMatrix2d inverse = mat5.GetInverted();
	QVERIFY(qAbs(inverse.GetAt(0, 0) - 0.5) < I_EPSILON);
	QVERIFY(qAbs(inverse.GetAt(1, 1) - 1.0/3.0) < I_EPSILON);
}


void CI2dTest::DoAffine2dTest()
{
	// Test default constructor
	i2d::CAffine2d affine1;
	affine1.Reset(); // Identity transformation
	i2d::CVector2d v1(1.0, 2.0);
	i2d::CVector2d result1 = affine1.GetApply(v1);
	QVERIFY(qAbs(result1.GetX() - 1.0) < I_EPSILON);
	QVERIFY(qAbs(result1.GetY() - 2.0) < I_EPSILON);

	// Test translation only constructor
	i2d::CAffine2d affine2(i2d::CVector2d(5.0, 10.0));
	i2d::CVector2d result2 = affine2.GetApply(v1);
	QVERIFY(qAbs(result2.GetX() - 6.0) < I_EPSILON);
	QVERIFY(qAbs(result2.GetY() - 12.0) < I_EPSILON);

	// Test translation getter/setter
	QVERIFY(qAbs(affine2.GetTranslation().GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(affine2.GetTranslation().GetY() - 10.0) < I_EPSILON);
	
	affine2.SetTranslation(i2d::CVector2d(3.0, 4.0));
	QVERIFY(qAbs(affine2.GetTranslation().GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(affine2.GetTranslation().GetY() - 4.0) < I_EPSILON);

	// Test with rotation and scale
	i2d::CAffine2d affine3;
	affine3.Reset(i2d::CVector2d(0.0, 0.0), M_PI / 2.0, 2.0); // 90 degrees, scale 2
	i2d::CVector2d v2(1.0, 0.0);
	i2d::CVector2d result3 = affine3.GetApply(v2);
	QVERIFY(qAbs(result3.GetX()) < I_BIG_EPSILON);
	QVERIFY(qAbs(result3.GetY() - 2.0) < I_BIG_EPSILON);

	// Test deformation matrix
	i2d::CMatrix2d deform(2.0, 0.0, 0.0, 3.0);
	i2d::CAffine2d affine4(deform, i2d::CVector2d(1.0, 1.0));
	i2d::CVector2d v3(1.0, 1.0);
	i2d::CVector2d result4 = affine4.GetApply(v3);
	QVERIFY(qAbs(result4.GetX() - 3.0) < I_EPSILON); // 2*1 + 1 = 3
	QVERIFY(qAbs(result4.GetY() - 4.0) < I_EPSILON); // 3*1 + 1 = 4

	// Test apply to delta (no translation)
	i2d::CVector2d delta(1.0, 1.0);
	i2d::CVector2d resultDelta = affine4.GetApplyToDelta(delta);
	QVERIFY(qAbs(resultDelta.GetX() - 2.0) < I_EPSILON); // 2*1
	QVERIFY(qAbs(resultDelta.GetY() - 3.0) < I_EPSILON); // 3*1

	// Test combined transformation
	i2d::CAffine2d affine5;
	affine5.Reset(i2d::CVector2d(1.0, 0.0));
	i2d::CAffine2d affine6;
	affine6.Reset(i2d::CVector2d(0.0, 1.0));
	i2d::CAffine2d combined = affine5.GetApply(affine6);
	i2d::CVector2d v4(0.0, 0.0);
	i2d::CVector2d resultCombined = combined.GetApply(v4);
	QVERIFY(qAbs(resultCombined.GetX() - 1.0) < I_EPSILON);
	QVERIFY(qAbs(resultCombined.GetY() - 1.0) < I_EPSILON);

	// Test inverted transformation
	i2d::CAffine2d affine7;
	affine7.Reset(i2d::CVector2d(5.0, 10.0));
	i2d::CVector2d v5(10.0, 20.0);
	i2d::CVector2d forward = affine7.GetApply(v5);
	i2d::CVector2d backward = affine7.GetInvertedApply(forward);
	QVERIFY(qAbs(backward.GetX() - v5.GetX()) < I_BIG_EPSILON);
	QVERIFY(qAbs(backward.GetY() - v5.GetY()) < I_BIG_EPSILON);
}


void CI2dTest::DoAnnulusTest()
{
	// Test default constructor
	i2d::CAnnulus annulus1;
	annulus1.SetPosition(i2d::CVector2d(0.0, 0.0));
	annulus1.SetInnerRadius(5.0);
	annulus1.SetOuterRadius(10.0);
	QVERIFY(qAbs(annulus1.GetInnerRadius() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(annulus1.GetOuterRadius() - 10.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CAnnulus annulus2(i2d::CVector2d(10.0, 20.0), 3.0, 8.0);
	QVERIFY(qAbs(annulus2.GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(annulus2.GetY() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(annulus2.GetInnerRadius() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(annulus2.GetOuterRadius() - 8.0) < I_EPSILON);

	// Test validity
	QVERIFY(annulus2.IsAnnulusValid());

	// Test with invalid radii (inner > outer)
	i2d::CAnnulus annulus3(i2d::CVector2d(0.0, 0.0), 10.0, 5.0);
	QVERIFY(!annulus3.IsAnnulusValid());

	// Test SetInnerOuterRadius
	annulus3.SetInnerOuterRadius(2.0, 7.0);
	QVERIFY(qAbs(annulus3.GetInnerRadius() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(annulus3.GetOuterRadius() - 7.0) < I_EPSILON);
	QVERIFY(annulus3.IsAnnulusValid());

	// Test inner circle
	i2d::CCircle innerCircle = annulus2.GetInnerCircle();
	QVERIFY(qAbs(innerCircle.GetRadius() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(innerCircle.GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(innerCircle.GetY() - 20.0) < I_EPSILON);

	// Test outer circle
	i2d::CCircle outerCircle = annulus2.GetOuterCircle();
	QVERIFY(qAbs(outerCircle.GetRadius() - 8.0) < I_EPSILON);
	QVERIFY(qAbs(outerCircle.GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(outerCircle.GetY() - 20.0) < I_EPSILON);

	// Test equality operator
	i2d::CAnnulus annulus4(i2d::CVector2d(10.0, 20.0), 3.0, 8.0);
	QVERIFY(annulus2 == annulus4);

	// Test inequality
	i2d::CAnnulus annulus5(i2d::CVector2d(10.0, 20.0), 3.0, 9.0);
	QVERIFY(!(annulus2 == annulus5));

	// Test bounding box
	i2d::CRectangle bbox = annulus2.GetBoundingBox();
	QVERIFY(qAbs(bbox.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetPosition().GetY() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetWidth() - 16.0) < I_EPSILON);  // 2 * outer radius
	QVERIFY(qAbs(bbox.GetHeight() - 16.0) < I_EPSILON); // 2 * outer radius
}


void CI2dTest::DoPolylineTest()
{
	// Test default constructor
	i2d::CPolyline polyline1;
	QVERIFY(polyline1.IsEmpty());
	QVERIFY(polyline1.GetNodesCount() == 0);

	// Test adding nodes
	polyline1.InsertNode(i2d::CVector2d(0.0, 0.0));
	polyline1.InsertNode(i2d::CVector2d(10.0, 0.0));
	polyline1.InsertNode(i2d::CVector2d(10.0, 10.0));
	QVERIFY(polyline1.GetNodesCount() == 3);
	QVERIFY(!polyline1.IsEmpty());

	// Test getting nodes
	QVERIFY(qAbs(polyline1.GetNodePos(0).GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(0).GetY() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(1).GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(1).GetY() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(2).GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(2).GetY() - 10.0) < I_EPSILON);

	// Test closed state
	QVERIFY(!polyline1.IsClosed()); // Open by default
	polyline1.SetClosed(true);
	QVERIFY(polyline1.IsClosed());

	// Test segments count
	int segmentsOpen = 2; // For 3 points open: 2 segments
	int segmentsClosed = 3; // For 3 points closed: 3 segments
	polyline1.SetClosed(false);
	QVERIFY(polyline1.GetSegmentsCount() == segmentsOpen);
	polyline1.SetClosed(true);
	QVERIFY(polyline1.GetSegmentsCount() == segmentsClosed);

	// Test length calculation
	polyline1.SetClosed(false);
	double expectedLength = 20.0; // 10 + 10
	QVERIFY(qAbs(polyline1.GetLength() - expectedLength) < I_EPSILON);

	// Test getting segment line
	i2d::CLine2d segment1 = polyline1.GetSegmentLine(0);
	QVERIFY(qAbs(segment1.GetPoint1().GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(segment1.GetPoint1().GetY() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(segment1.GetPoint2().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(segment1.GetPoint2().GetY() - 0.0) < I_EPSILON);

	// Test setting node position
	polyline1.SetNodePos(1, i2d::CVector2d(5.0, 5.0));
	QVERIFY(qAbs(polyline1.GetNodePos(1).GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(1).GetY() - 5.0) < I_EPSILON);

	// Test removing node
	polyline1.RemoveNode(1);
	QVERIFY(polyline1.GetNodesCount() == 2);
	QVERIFY(qAbs(polyline1.GetNodePos(1).GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(polyline1.GetNodePos(1).GetY() - 10.0) < I_EPSILON);

	// Test clearing
	polyline1.Clear();
	QVERIFY(polyline1.IsEmpty());
	QVERIFY(polyline1.GetNodesCount() == 0);
}


void CI2dTest::DoPolygonTest()
{
	// Test default constructor
	i2d::CPolygon polygon1;
	QVERIFY(polygon1.IsEmpty());
	QVERIFY(polygon1.GetNodesCount() == 0);

	// Test adding nodes to create a square
	polygon1.InsertNode(i2d::CVector2d(0.0, 0.0));
	polygon1.InsertNode(i2d::CVector2d(10.0, 0.0));
	polygon1.InsertNode(i2d::CVector2d(10.0, 10.0));
	polygon1.InsertNode(i2d::CVector2d(0.0, 10.0));
	QVERIFY(polygon1.GetNodesCount() == 4);

	// Test contains point
	QVERIFY(polygon1.Contains(i2d::CVector2d(5.0, 5.0))); // Inside
	QVERIFY(!polygon1.Contains(i2d::CVector2d(15.0, 15.0))); // Outside

	// Test area calculation
	double area = polygon1.GetArea();
	QVERIFY(qAbs(area - 100.0) < I_EPSILON); // 10 * 10 = 100

	// Test perimeter calculation
	double perimeter = polygon1.GetPerimeter();
	QVERIFY(qAbs(perimeter - 40.0) < I_EPSILON); // 4 * 10 = 40

	// Test outline length (should be same as perimeter for polygon)
	double outlineLength = polygon1.GetOutlineLength();
	QVERIFY(qAbs(outlineLength - 40.0) < I_EPSILON);

	// Test QPolygonF conversion
	QPolygonF qpoly = polygon1;
	QVERIFY(qpoly.size() == 4);
	QVERIFY(qAbs(qpoly[0].x() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(qpoly[0].y() - 0.0) < I_EPSILON);

	// Test constructor from QPolygonF
	QPolygonF qpoly2;
	qpoly2 << QPointF(1.0, 1.0) << QPointF(2.0, 1.0) << QPointF(2.0, 2.0);
	i2d::CPolygon polygon2(qpoly2);
	QVERIFY(polygon2.GetNodesCount() == 3);
	QVERIFY(qAbs(polygon2.GetNodePos(0).GetX() - 1.0) < I_EPSILON);
	QVERIFY(qAbs(polygon2.GetNodePos(0).GetY() - 1.0) < I_EPSILON);

	// Test reverse nodes
	polygon2.ReverseNodes();
	QVERIFY(qAbs(polygon2.GetNodePos(0).GetX() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(polygon2.GetNodePos(0).GetY() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(polygon2.GetNodePos(2).GetX() - 1.0) < I_EPSILON);
	QVERIFY(qAbs(polygon2.GetNodePos(2).GetY() - 1.0) < I_EPSILON);

	// Test bounding box
	i2d::CRectangle bbox = polygon1.GetBoundingBox();
	QVERIFY(qAbs(bbox.GetLeft() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetRight() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetTop() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(bbox.GetBottom() - 10.0) < I_EPSILON);
}


I_ADD_TEST(CI2dTest);


