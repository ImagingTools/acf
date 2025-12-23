#include <i2d/Test/CI2dTest.h>


// Standard includes
#include <cmath>

// ACF includes
#include <istd/istd.h>
#include <istd/CIndex2d.h>
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
#include <i2d/CAffineTransformation2d.h>
#include <i2d/CAnnulusSegment.h>
#include <i2d/CArc.h>
#include <i2d/CDirection2d.h>
#include <i2d/COrientedCircle.h>
#include <i2d/CParallelogram.h>
#include <i2d/CPolypoint.h>
#include <i2d/CQuadrangle.h>
#include <i2d/CRect.h>
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
	QVERIFY(qAbs(pos2.GetPosition().GetX() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(pos2.GetPosition().GetY() - 25.0) < I_EPSILON);

	// Test SetX and SetY
	pos1.SetPosition(i2d::CVector2d(30.0, 40.0));
	QVERIFY(qAbs(pos1.GetPosition().GetX() - 30.0) < I_EPSILON);
	QVERIFY(qAbs(pos1.GetPosition().GetY() - 40.0) < I_EPSILON);
}


void CI2dTest::DoCircleTest()
{
	// Test default constructor
	i2d::CCircle circle1;
	circle1.SetRadius(5.0);
	circle1.SetPosition(i2d::CVector2d(10.0, 10.0));
	QVERIFY(qAbs(circle1.GetRadius() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(circle1.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(circle1.GetPosition().GetY() - 10.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CCircle circle2(7.0, i2d::CVector2d(20.0, 30.0));
	QVERIFY(qAbs(circle2.GetRadius() - 7.0) < I_EPSILON);
	QVERIFY(qAbs(circle2.GetPosition().GetX() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(circle2.GetPosition().GetY() - 30.0) < I_EPSILON);

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
	i2d::CRectangle rect1(0, 0, 50.0, 50.0);
	QVERIFY(qAbs(rect1.GetCenter().GetX() - 25.0) < I_EPSILON);
	QVERIFY(qAbs(rect1.GetCenter().GetY() - 25.0) < I_EPSILON);
	QVERIFY(qAbs(rect1.GetWidth() - 50.0) < I_EPSILON);
	QVERIFY(qAbs(rect1.GetHeight() - 50.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CRectangle rect2(5.0, 10.0, 20.0, 25.0);
	QVERIFY(qAbs(rect2.GetLeft() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetTop() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetWidth() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetHeight() - 25.0) < I_EPSILON);

	// Test GetLeft, GetRight, GetTop, GetBottom
	QVERIFY(qAbs(rect2.GetLeft() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetRight() - 25.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetTop() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(rect2.GetBottom() - 35.0) < I_EPSILON);

	// Test corner getters
	i2d::CVector2d topLeft = rect2.GetLeftTop();
	QVERIFY(qAbs(topLeft.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(topLeft.GetY() - 10.0) < I_EPSILON);

	i2d::CVector2d bottomRight = rect2.GetRightBottom();
	QVERIFY(qAbs(bottomRight.GetX() - 25.0) < I_EPSILON);
	QVERIFY(qAbs(bottomRight.GetY() - 35.0) < I_EPSILON);

	// Test center
	i2d::CVector2d center = rect2.GetCenter();
	QVERIFY(qAbs(center.GetX() - 15.0) < I_EPSILON);
	QVERIFY(qAbs(center.GetY() - 22.5) < I_EPSILON);

	// Test equality operator
	i2d::CRectangle rect3(5.0, 10.0, 20.0, 25.0);
	QVERIFY(rect2 == rect3);

	// Test inequality operator
	i2d::CRectangle rect4(5.0, 10.0, 21.0, 25.0);
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
	annulus1.SetOuterRadius(10.0);
	annulus1.SetInnerRadius(5.0);
	QVERIFY(qAbs(annulus1.GetInnerRadius() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(annulus1.GetOuterRadius() - 10.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CAnnulus annulus2(i2d::CVector2d(10.0, 20.0), 3.0, 8.0);
	QVERIFY(qAbs(annulus2.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(annulus2.GetPosition().GetY() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(annulus2.GetInnerRadius() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(annulus2.GetOuterRadius() - 8.0) < I_EPSILON);

	// Test validity
	QVERIFY(annulus2.IsAnnulusValid());

	// Test bounding box
	i2d::CRectangle bbox = annulus2.GetBoundingBox();
	// Bounding box position is at top-left corner: center - radius
	QVERIFY(qAbs(bbox.GetWidth() - 16.0) < I_EPSILON);  // 2 * outer radius
	QVERIFY(qAbs(bbox.GetHeight() - 16.0) < I_EPSILON); // 2 * outer radius

	// Test with invalid radii (inner > outer)
	i2d::CAnnulus annulus3(i2d::CVector2d(0.0, 0.0), 5.0, 10.0);

	// Test SetInnerOuterRadius
	annulus3.SetInnerOuterRadius(2.0, 7.0);
	QVERIFY(qAbs(annulus3.GetInnerRadius() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(annulus3.GetOuterRadius() - 7.0) < I_EPSILON);
	QVERIFY(annulus3.IsAnnulusValid());

	// Test inner circle
	i2d::CCircle innerCircle = annulus2.GetInnerCircle();
	QVERIFY(qAbs(innerCircle.GetRadius() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(innerCircle.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(innerCircle.GetPosition().GetY() - 20.0) < I_EPSILON);

	// Test outer circle
	i2d::CCircle outerCircle = annulus2.GetOuterCircle();
	QVERIFY(qAbs(outerCircle.GetRadius() - 8.0) < I_EPSILON);
	QVERIFY(qAbs(outerCircle.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(outerCircle.GetPosition().GetY() - 20.0) < I_EPSILON);

	// Test equality operator
	i2d::CAnnulus annulus4(i2d::CVector2d(10.0, 20.0), 3.0, 8.0);
	QVERIFY(annulus2 == annulus4);

	// Test inequality
	i2d::CAnnulus annulus5(i2d::CVector2d(10.0, 20.0), 3.0, 9.0);
	QVERIFY(!(annulus2 == annulus5));
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


void CI2dTest::DoAffineTransformation2dTest()
{
	// Test default constructor
	i2d::CAffineTransformation2d trans1;
	trans1.Reset();
	i2d::CVector2d v1(1.0, 2.0);
	i2d::CVector2d result1;
	trans1.GetPositionAt(v1, result1);
	QVERIFY(qAbs(result1.GetX() - 1.0) < I_EPSILON);
	QVERIFY(qAbs(result1.GetY() - 2.0) < I_EPSILON);

	// Test with CAffine2d
	i2d::CAffine2d affine(i2d::CVector2d(5.0, 10.0));
	i2d::CAffineTransformation2d trans2(affine);
	const i2d::CAffine2d& gotAffine = trans2.GetTransformation();
	QVERIFY(qAbs(gotAffine.GetTranslation().GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(gotAffine.GetTranslation().GetY() - 10.0) < I_EPSILON);

	// Test SetTransformation
	i2d::CAffine2d affine2(i2d::CVector2d(3.0, 4.0));
	trans2.SetTransformation(affine2);
	QVERIFY(qAbs(trans2.GetTransformation().GetTranslation().GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(trans2.GetTransformation().GetTranslation().GetY() - 4.0) < I_EPSILON);

	// Test Reset with translation
	trans1.Reset(i2d::CVector2d(7.0, 8.0));
	i2d::CVector2d v2(0.0, 0.0);
	i2d::CVector2d result2;
	trans1.GetPositionAt(v2, result2);
	QVERIFY(qAbs(result2.GetX() - 7.0) < I_EPSILON);
	QVERIFY(qAbs(result2.GetY() - 8.0) < I_EPSILON);

	// Test Reset with translation, rotation and scale
	trans1.Reset(i2d::CVector2d(0.0, 0.0), M_PI / 2.0, 2.0);
	i2d::CVector2d v3(1.0, 0.0);
	i2d::CVector2d result3;
	trans1.GetPositionAt(v3, result3);
	QVERIFY(qAbs(result3.GetX()) < I_BIG_EPSILON);
	QVERIFY(qAbs(result3.GetY() - 2.0) < I_BIG_EPSILON);
}


void CI2dTest::DoAnnulusSegmentTest()
{
	// Test default constructor
	i2d::CAnnulusSegment segment1;
	segment1.SetPosition(i2d::CVector2d(0.0, 0.0));
	segment1.SetInnerRadius(5.0);
	segment1.SetOuterRadius(10.0);
	segment1.SetBeginAngle(0.0);
	segment1.SetEndAngle(M_PI / 2.0);
	QVERIFY(qAbs(segment1.GetBeginAngle() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(segment1.GetEndAngle() - M_PI / 2.0) < I_EPSILON);

	// Test parameterized constructor
	i2d::CAnnulusSegment segment2(i2d::CVector2d(10.0, 20.0), 3.0, 8.0, 0.0, M_PI);
	QVERIFY(qAbs(segment2.GetPosition().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(segment2.GetPosition().GetY() - 20.0) < I_EPSILON);
	QVERIFY(qAbs(segment2.GetInnerRadius() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(segment2.GetOuterRadius() - 8.0) < I_EPSILON);
	QVERIFY(qAbs(segment2.GetBeginAngle() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(segment2.GetEndAngle() - M_PI) < I_EPSILON);

	// Test Contains point
	// Point at angle 0, radius 5 should be inside
	i2d::CVector2d point1(15.0, 20.0); // radius 5 from center
	QVERIFY(segment2.Contains(point1));
	
	// Point at angle PI, radius 5 should be inside
	i2d::CVector2d point2(5.0, 20.0);
	QVERIFY(segment2.Contains(point2));
	
	// Point at (10, 15) relative to center (10, 20) is at angle -PI/2 (270°), outside [0, PI] range
	i2d::CVector2d point3(10.0, 15.0);
	QVERIFY(!segment2.Contains(point3));

	// Test angle setters
	segment1.SetBeginAngle(M_PI / 4.0);
	segment1.SetEndAngle(3.0 * M_PI / 4.0);
	QVERIFY(qAbs(segment1.GetBeginAngle() - M_PI / 4.0) < I_EPSILON);
	QVERIFY(qAbs(segment1.GetEndAngle() - 3.0 * M_PI / 4.0) < I_EPSILON);
}


void CI2dTest::DoArcTest()
{
	// Test default constructor
	i2d::CArc arc1;
	arc1.SetRadius(5.0);
	arc1.SetPosition(i2d::CVector2d(0.0, 0.0));
	arc1.SetStartAngle(0.0);
	arc1.SetEndAngle(M_PI);
	QVERIFY(qAbs(arc1.GetStartAngle() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(arc1.GetEndAngle() - M_PI) < I_EPSILON);

	// Test parameterized constructor
	i2d::CArc arc2(0.0, M_PI / 2.0, 10.0, i2d::CVector2d(5.0, 5.0));
	QVERIFY(qAbs(arc2.GetStartAngle() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(arc2.GetEndAngle() - M_PI / 2.0) < I_EPSILON);
	QVERIFY(qAbs(arc2.GetRadius() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(arc2.GetPosition().GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(arc2.GetPosition().GetY() - 5.0) < I_EPSILON);

	// Test equality operator
	i2d::CArc arc3(0.0, M_PI / 2.0, 10.0, i2d::CVector2d(5.0, 5.0));
	QVERIFY(arc2 == arc3);

	// Test inequality operator
	i2d::CArc arc4(0.0, M_PI / 3.0, 10.0, i2d::CVector2d(5.0, 5.0));
	QVERIFY(arc2 != arc4);

	// Test GetPositionFromAlpha
	i2d::CVector2d pos0 = arc2.GetPositionFromAlpha(0.0);
	QVERIFY(qAbs(pos0.GetX() - 15.0) < I_BIG_EPSILON); // 5 + 10*cos(0)
	QVERIFY(qAbs(pos0.GetY() - 5.0) < I_BIG_EPSILON);  // 5 + 10*sin(0)

	i2d::CVector2d pos1 = arc2.GetPositionFromAlpha(1.0);
	QVERIFY(qAbs(pos1.GetX() - 5.0) < I_BIG_EPSILON);  // 5 + 10*cos(PI/2)
	QVERIFY(qAbs(pos1.GetY() - 15.0) < I_BIG_EPSILON); // 5 + 10*sin(PI/2)
}


void CI2dTest::DoDirection2dTest()
{
	// Test default constructor
	i2d::CDirection2d dir1;
	QVERIFY(qAbs(dir1.ToRadian() - 0.0) < I_EPSILON);

	// Test constructor from radian
	i2d::CDirection2d dir2(M_PI / 4.0);
	QVERIFY(qAbs(dir2.ToRadian() - M_PI / 4.0) < I_EPSILON);

	// Test FromRadian
	i2d::CDirection2d dir3 = i2d::CDirection2d::FromRadian(M_PI / 2.0);
	QVERIFY(qAbs(dir3.ToRadian() - M_PI / 2.0) < I_EPSILON);

	// Test FromDegree
	i2d::CDirection2d dir4 = i2d::CDirection2d::FromDegree(90.0);
	QVERIFY(qAbs(dir4.ToRadian() - M_PI / 2.0) < I_EPSILON);
	QVERIFY(qAbs(dir4.ToDegree() - 90.0) < I_EPSILON);

	// Test ToDegree
	i2d::CDirection2d dir5(M_PI);
	QVERIFY(qAbs(dir5.ToDegree() - 180.0) < I_EPSILON);

	// Test constructor from vector
	i2d::CVector2d vec(1.0, 1.0);
	i2d::CDirection2d dir6(vec);
	QVERIFY(qAbs(dir6.ToRadian() - M_PI / 4.0) < I_EPSILON);

	// Test copy constructor
	i2d::CDirection2d dir7(dir6);
	QVERIFY(qAbs(dir7.ToRadian() - M_PI / 4.0) < I_EPSILON);

	// Test angle normalization (0 degrees)
	i2d::CDirection2d dir8 = i2d::CDirection2d::FromDegree(0.0);
	QVERIFY(qAbs(dir8.ToDegree() - 0.0) < I_EPSILON);

	// Test angle normalization (360 degrees -> 0)
	i2d::CDirection2d dir9 = i2d::CDirection2d::FromDegree(360.0);
	QVERIFY(qAbs(dir9.ToDegree() - 0.0) < I_EPSILON);
}


void CI2dTest::DoOrientedCircleTest()
{
	// Test default constructor
	i2d::COrientedCircle circle1;
	circle1.SetRadius(5.0);
	circle1.SetPosition(i2d::CVector2d(0.0, 0.0));
	circle1.SetOrientedOutside(true);
	QVERIFY(circle1.IsOrientedOutside());

	// Test parameterized constructor
	i2d::COrientedCircle circle2(10.0, i2d::CVector2d(5.0, 5.0), false);
	QVERIFY(qAbs(circle2.GetRadius() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(circle2.GetPosition().GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(circle2.GetPosition().GetY() - 5.0) < I_EPSILON);
	QVERIFY(!circle2.IsOrientedOutside());

	// Test SetOrientedOutside
	circle2.SetOrientedOutside(true);
	QVERIFY(circle2.IsOrientedOutside());

	// Test equality operator
	i2d::COrientedCircle circle3(10.0, i2d::CVector2d(5.0, 5.0), true);
	QVERIFY(circle2 == circle3);

	// Test inequality operator (different orientation)
	i2d::COrientedCircle circle4(10.0, i2d::CVector2d(5.0, 5.0), false);
	QVERIFY(circle2 != circle4);

	// Test inequality operator (different radius)
	i2d::COrientedCircle circle5(11.0, i2d::CVector2d(5.0, 5.0), true);
	QVERIFY(circle2 != circle5);
}


void CI2dTest::DoParallelogramTest()
{
	// Test default constructor
	i2d::CParallelogram para1;
	i2d::CAffine2d transform;
	transform.Reset();
	para1.SetTransform(transform);
	
	// Test GetTransform
	const i2d::CAffine2d& gotTransform = para1.GetTransform();
	QVERIFY(qAbs(gotTransform.GetTranslation().GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(gotTransform.GetTranslation().GetY() - 0.0) < I_EPSILON);

	// Test width and height
	para1.SetWidth(10.0);
	para1.SetHeight(5.0);
	QVERIFY(qAbs(para1.GetWidth() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(para1.GetHeight() - 5.0) < I_EPSILON);

	// Test translation
	para1.SetTranslation(i2d::CVector2d(3.0, 4.0));
	i2d::CVector2d translation = para1.GetTranslation();
	QVERIFY(qAbs(translation.GetX() - 3.0) < I_EPSILON);
	QVERIFY(qAbs(translation.GetY() - 4.0) < I_EPSILON);

	// Test rotation
	para1.SetRotation(M_PI / 4.0);
	double rotation = para1.GetRotation();
	QVERIFY(qAbs(rotation - M_PI / 4.0) < I_BIG_EPSILON);

	// Test GetNodes
	i2d::CParallelogram para2;
	para2.SetWidth(4.0);
	para2.SetHeight(3.0);
	para2.SetTranslation(i2d::CVector2d(0.0, 0.0));
	para2.SetRotation(0.0);
	QVector<i2d::CVector2d> nodes = para2.GetNodes();
	QVERIFY(nodes.size() == 4);
	// Check first node at origin
	QVERIFY(qAbs(nodes[0].GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(nodes[0].GetY() - 0.0) < I_EPSILON);

	// Test GetCenter
	i2d::CVector2d center = para2.GetCenter();
	QVERIFY(qAbs(center.GetX() - 2.0) < I_EPSILON); // width/2
	QVERIFY(qAbs(center.GetY() - 1.5) < I_EPSILON); // height/2
}


void CI2dTest::DoPolypointTest()
{
	// Test default constructor
	i2d::CPolypoint polypoint1;
	QVERIFY(polypoint1.IsEmpty());
	QVERIFY(polypoint1.GetNodesCount() == 0);

	// Test adding nodes
	polypoint1.InsertNode(i2d::CVector2d(0.0, 0.0));
	polypoint1.InsertNode(i2d::CVector2d(5.0, 5.0));
	polypoint1.InsertNode(i2d::CVector2d(10.0, 0.0));
	QVERIFY(polypoint1.GetNodesCount() == 3);
	QVERIFY(!polypoint1.IsEmpty());

	// Test getting nodes
	QVERIFY(qAbs(polypoint1.GetNodePos(0).GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(0).GetY() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetY() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(2).GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(2).GetY() - 0.0) < I_EPSILON);

	// Test SetNodePos
	polypoint1.SetNodePos(1, i2d::CVector2d(7.0, 8.0));
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetX() - 7.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetY() - 8.0) < I_EPSILON);

	// Test InsertNode at index
	polypoint1.InsertNode(1, i2d::CVector2d(2.0, 2.0));
	QVERIFY(polypoint1.GetNodesCount() == 4);
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetX() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetY() - 2.0) < I_EPSILON);

	// Test RemoveNode
	polypoint1.RemoveNode(1);
	QVERIFY(polypoint1.GetNodesCount() == 3);
	QVERIFY(qAbs(polypoint1.GetNodePos(1).GetX() - 7.0) < I_EPSILON);

	// Test SetNodesCount
	polypoint1.SetNodesCount(5);
	QVERIFY(polypoint1.GetNodesCount() == 5);

	// Test Clear
	polypoint1.Clear();
	QVERIFY(polypoint1.IsEmpty());
	QVERIFY(polypoint1.GetNodesCount() == 0);
}


void CI2dTest::DoQuadrangleTest()
{
	// Test default constructor
	i2d::CQuadrangle quad1;
	QVERIFY(quad1.IsQuadrangleEmpty());

	// Test constructor with diagonals
	i2d::CLine2d diagonal1(i2d::CVector2d(0.0, 0.0), i2d::CVector2d(10.0, 10.0));
	i2d::CLine2d diagonal2(i2d::CVector2d(0.0, 10.0), i2d::CVector2d(10.0, 0.0));
	i2d::CQuadrangle quad2(diagonal1, diagonal2);
	QVERIFY(!quad2.IsQuadrangleEmpty());
	QVERIFY(quad2.IsQuadrangleValid());

	// Test GetFirstDiagonal and GetSecondDiagonal
	const i2d::CLine2d& gotDiag1 = quad2.GetFirstDiagonal();
	QVERIFY(qAbs(gotDiag1.GetPoint1().GetX() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(gotDiag1.GetPoint1().GetY() - 0.0) < I_EPSILON);
	QVERIFY(qAbs(gotDiag1.GetPoint2().GetX() - 10.0) < I_EPSILON);
	QVERIFY(qAbs(gotDiag1.GetPoint2().GetY() - 10.0) < I_EPSILON);

	// Test SetFirstDiagonal
	i2d::CLine2d newDiag1(i2d::CVector2d(0.0, 0.0), i2d::CVector2d(20.0, 20.0));
	quad2.SetFirstDiagonal(newDiag1);
	const i2d::CLine2d& updatedDiag1 = quad2.GetFirstDiagonal();
	QVERIFY(qAbs(updatedDiag1.GetPoint2().GetX() - 20.0) < I_EPSILON);

	// Test constructor from rectangle
	i2d::CRectangle rect(0.0, 0.0, 10.0, 10.0);
	i2d::CQuadrangle quad3(rect);
	QVERIFY(!quad3.IsQuadrangleEmpty());
	QVERIFY(quad3.IsQuadrangleValid());

	// Test GetCenter
	i2d::CVector2d center = quad3.GetCenter();
	QVERIFY(qAbs(center.GetX() - 5.0) < I_EPSILON);
	QVERIFY(qAbs(center.GetY() - 5.0) < I_EPSILON);

	// Test equality operator
	i2d::CQuadrangle quad4(diagonal1, diagonal2);
	i2d::CQuadrangle quad5(diagonal1, diagonal2);
	QVERIFY(quad4 == quad5);

	// Test inequality operator
	i2d::CLine2d diagonal3(i2d::CVector2d(0.0, 10.0), i2d::CVector2d(11.0, 0.0));
	i2d::CQuadrangle quad6(diagonal1, diagonal3);
	QVERIFY(quad4 != quad6);
}


void CI2dTest::DoRectTest()
{
	// Test default constructor
	i2d::CRect rect1;
	QVERIFY(rect1.IsEmpty());

	// Test constructor with coordinates
	i2d::CRect rect2(0, 0, 10, 10);
	QVERIFY(rect2.IsValid());
	QVERIFY(rect2.GetLeft() == 0);
	QVERIFY(rect2.GetTop() == 0);
	QVERIFY(rect2.GetRight() == 10);
	QVERIFY(rect2.GetBottom() == 10);

	// Test width and height
	QVERIFY(rect2.GetWidth() == 10);
	QVERIFY(rect2.GetHeight() == 10);

	// Test constructor with indices
	istd::CIndex2d leftTop(5, 5);
	istd::CIndex2d rightBottom(15, 15);
	i2d::CRect rect3(leftTop, rightBottom);
	QVERIFY(rect3.GetLeft() == 5);
	QVERIFY(rect3.GetTop() == 5);
	QVERIFY(rect3.GetRight() == 15);
	QVERIFY(rect3.GetBottom() == 15);

	// Test GetLeftTop and GetRightBottom
	istd::CIndex2d gotLeftTop = rect3.GetLeftTop();
	QVERIFY(gotLeftTop.GetX() == 5);
	QVERIFY(gotLeftTop.GetY() == 5);
	istd::CIndex2d gotRightBottom = rect3.GetRightBottom();
	QVERIFY(gotRightBottom.GetX() == 15);
	QVERIFY(gotRightBottom.GetY() == 15);

	// Test GetCenter
	istd::CIndex2d center = rect3.GetCenter();
	QVERIFY(center.GetX() == 10);
	QVERIFY(center.GetY() == 10);

	// Test SetLeft, SetTop, SetRight, SetBottom
	rect2.SetLeft(2);
	rect2.SetTop(3);
	rect2.SetRight(12);
	rect2.SetBottom(13);
	QVERIFY(rect2.GetLeft() == 2);
	QVERIFY(rect2.GetTop() == 3);
	QVERIFY(rect2.GetRight() == 12);
	QVERIFY(rect2.GetBottom() == 13);

	// Test IsEmpty
	i2d::CRect rect4(5, 5, 5, 5);
	QVERIFY(rect4.IsEmpty());

	// Test IsValidNonEmpty
	QVERIFY(!rect4.IsValidNonEmpty());
	QVERIFY(rect2.IsValidNonEmpty());

	// Test GetRectangle conversion
	i2d::CRectangle floatRect = rect2.GetRectangle();
	QVERIFY(qAbs(floatRect.GetLeft() - 2.0) < I_EPSILON);
	QVERIFY(qAbs(floatRect.GetTop() - 3.0) < I_EPSILON);

	// Test copy constructor
	i2d::CRect rect5(rect2);
	QVERIFY(rect5.GetLeft() == rect2.GetLeft());
	QVERIFY(rect5.GetTop() == rect2.GetTop());
}


I_ADD_TEST(CI2dTest);


