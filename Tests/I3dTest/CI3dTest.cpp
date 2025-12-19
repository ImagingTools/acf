#include "CI3dTest.h"


void CI3dTest::initTestCase()
{
	// Setup
}


void CI3dTest::cleanupTestCase()
{
	// Cleanup
}


// Vector tests

void CI3dTest::testVector3dBasics()
{
	i3d::CVector3d v1(1.0, 2.0, 3.0);
	
	QCOMPARE(v1.GetX(), 1.0);
	QCOMPARE(v1.GetY(), 2.0);
	QCOMPARE(v1.GetZ(), 3.0);
	
	double length = v1.GetLength();
	QVERIFY(qAbs(length - qSqrt(14.0)) < I_BIG_EPSILON);
}


void CI3dTest::testVector3dOperations()
{
	i3d::CVector3d v1(1.0, 0.0, 0.0);
	i3d::CVector3d v2(0.0, 1.0, 0.0);
	
	// Cross product
	i3d::CVector3d cross = v1.GetCrossProduct(v2);
	QVERIFY(qAbs(cross.GetX() - 0.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(cross.GetY() - 0.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(cross.GetZ() - 1.0) < I_BIG_EPSILON);
	
	// Dot product
	double dot = v1.GetDotProduct(v2);
	QVERIFY(qAbs(dot) < I_BIG_EPSILON);
}


// Plane tests

void CI3dTest::testPlane3dConstruction()
{
	i3d::CVector3d point(0.0, 0.0, 0.0);
	i3d::CVector3d normal(0.0, 0.0, 1.0);
	i3d::CPlane3d plane(point, normal);
	
	QVERIFY(plane.GetPoint() == point);
	QVERIFY(plane.GetNormal() == normal);
}


void CI3dTest::testPlane3dDistance()
{
	i3d::CPlane3d plane(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(0.0, 0.0, 1.0));
	i3d::CVector3d point(0.0, 0.0, 5.0);
	
	double distance = plane.GetDistance(point);
	QVERIFY(qAbs(distance - 5.0) < I_BIG_EPSILON);
}


void CI3dTest::testPlane3dProjection()
{
	i3d::CPlane3d plane(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(0.0, 0.0, 1.0));
	i3d::CVector3d point(3.0, 4.0, 5.0);
	
	i3d::CVector3d projected = plane.GetProjection(point);
	QVERIFY(qAbs(projected.GetX() - 3.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(projected.GetY() - 4.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(projected.GetZ() - 0.0) < I_BIG_EPSILON);
}


// Line tests

void CI3dTest::testLine3dBasics()
{
	i3d::CLine3d line(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(1.0, 0.0, 0.0));
	
	QVERIFY(qAbs(line.GetLength() - 1.0) < I_BIG_EPSILON);
	QVERIFY(!line.IsNull());
}


void CI3dTest::testLine3dDistance()
{
	i3d::CLine3d line(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(1.0, 0.0, 0.0));
	i3d::CVector3d point(0.5, 1.0, 0.0);
	
	double distance = line.GetDistance(point);
	QVERIFY(qAbs(distance - 1.0) < I_BIG_EPSILON);
}


void CI3dTest::testLine3dClosestPoint()
{
	i3d::CLine3d line(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(1.0, 0.0, 0.0));
	i3d::CVector3d point(0.5, 1.0, 0.0);
	
	i3d::CVector3d closest = line.GetClosestPoint(point);
	QVERIFY(qAbs(closest.GetX() - 0.5) < I_BIG_EPSILON);
	QVERIFY(qAbs(closest.GetY() - 0.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(closest.GetZ() - 0.0) < I_BIG_EPSILON);
}


// Sphere tests

void CI3dTest::testSphereBasics()
{
	i3d::CSphere sphere(2.0, i3d::CVector3d(0.0, 0.0, 0.0));
	
	QVERIFY(qAbs(sphere.GetRadius() - 2.0) < I_BIG_EPSILON);
	QVERIFY(sphere.GetCenter() == i3d::CVector3d(0.0, 0.0, 0.0));
	
	double volume = sphere.GetVolume();
	double expectedVolume = (4.0 / 3.0) * I_PI * 8.0; // r^3 = 8
	QVERIFY(qAbs(volume - expectedVolume) < 0.01);
}


void CI3dTest::testSphereIntersection()
{
	i3d::CSphere s1(1.0, i3d::CVector3d(0.0, 0.0, 0.0));
	i3d::CSphere s2(1.0, i3d::CVector3d(1.5, 0.0, 0.0));
	
	QVERIFY(s1.Intersects(s2));
	
	i3d::CSphere s3(1.0, i3d::CVector3d(3.0, 0.0, 0.0));
	QVERIFY(!s1.Intersects(s3));
}


// Box tests

void CI3dTest::testBox3dBasics()
{
	i3d::CBox3d box(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(2.0, 2.0, 2.0));
	
	QVERIFY(qAbs(box.GetVolume() - 8.0) < I_BIG_EPSILON);
	
	i3d::CVector3d center = box.GetCenter();
	QVERIFY(qAbs(center.GetX() - 1.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(center.GetY() - 1.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(center.GetZ() - 1.0) < I_BIG_EPSILON);
}


void CI3dTest::testBox3dIntersection()
{
	i3d::CBox3d b1(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(2.0, 2.0, 2.0));
	i3d::CBox3d b2(i3d::CVector3d(1.0, 1.0, 1.0), i3d::CVector3d(3.0, 3.0, 3.0));
	
	QVERIFY(b1.Intersects(b2));
	
	i3d::CBox3d b3(i3d::CVector3d(3.0, 3.0, 3.0), i3d::CVector3d(4.0, 4.0, 4.0));
	QVERIFY(!b1.Intersects(b3));
}


// Quaternion tests

void CI3dTest::testQuaternionRotation()
{
	// 90 degree rotation around Z axis
	i3d::CQuaternion3d q = i3d::CQuaternion3d::FromAxisAngle(
		i3d::CVector3d(0.0, 0.0, 1.0), 
		I_PI / 2.0);
	
	i3d::CVector3d v(1.0, 0.0, 0.0);
	i3d::CVector3d rotated = q.Rotate(v);
	
	QVERIFY(qAbs(rotated.GetX() - 0.0) < 0.001);
	QVERIFY(qAbs(rotated.GetY() - 1.0) < 0.001);
	QVERIFY(qAbs(rotated.GetZ() - 0.0) < 0.001);
}


void CI3dTest::testQuaternionSlerp()
{
	i3d::CQuaternion3d q1; // Identity
	i3d::CQuaternion3d q2 = i3d::CQuaternion3d::FromAxisAngle(
		i3d::CVector3d(0.0, 0.0, 1.0), 
		I_PI / 2.0);
	
	i3d::CQuaternion3d qMid = i3d::CQuaternion3d::Slerp(q1, q2, 0.5);
	
	// Check that interpolated quaternion has reasonable values
	QVERIFY(qMid.GetLength() > 0.9 && qMid.GetLength() < 1.1);
}


// Affine transform tests

void CI3dTest::testAffine3dBasics()
{
	i3d::CAffine3d transform = i3d::CAffine3d::CreateTranslation(i3d::CVector3d(1.0, 2.0, 3.0));
	
	i3d::CVector3d point(0.0, 0.0, 0.0);
	i3d::CVector3d transformed = transform.Transform(point);
	
	QVERIFY(qAbs(transformed.GetX() - 1.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(transformed.GetY() - 2.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(transformed.GetZ() - 3.0) < I_BIG_EPSILON);
}


void CI3dTest::testAffine3dComposition()
{
	i3d::CAffine3d t1 = i3d::CAffine3d::CreateTranslation(i3d::CVector3d(1.0, 0.0, 0.0));
	i3d::CAffine3d t2 = i3d::CAffine3d::CreateScale(2.0);
	
	i3d::CAffine3d combined = t1 * t2;
	
	i3d::CVector3d point(1.0, 1.0, 1.0);
	i3d::CVector3d transformed = combined.Transform(point);
	
	// First scale by 2, then translate by 1 in X
	QVERIFY(qAbs(transformed.GetX() - 3.0) < I_BIG_EPSILON); // 1*2 + 1
	QVERIFY(qAbs(transformed.GetY() - 2.0) < I_BIG_EPSILON); // 1*2
	QVERIFY(qAbs(transformed.GetZ() - 2.0) < I_BIG_EPSILON); // 1*2
}


// Geometry utility tests

void CI3dTest::testAngleCalculations()
{
	i3d::CVector3d v1(1.0, 0.0, 0.0);
	i3d::CVector3d v2(0.0, 1.0, 0.0);
	
	double angle = i3d::Geometry::GetAngleBetweenVectors(v1, v2);
	QVERIFY(qAbs(angle - I_PI / 2.0) < 0.001);
}


void CI3dTest::testLinePlaneIntersection()
{
	i3d::CLine3d line(i3d::CVector3d(0.0, 0.0, -1.0), i3d::CVector3d(0.0, 0.0, 1.0));
	i3d::CPlane3d plane(i3d::CVector3d(0.0, 0.0, 0.0), i3d::CVector3d(0.0, 0.0, 1.0));
	
	i3d::CVector3d intersection;
	bool hasIntersection = i3d::Geometry::GetIntersection(line, plane, intersection);
	
	QVERIFY(hasIntersection);
	QVERIFY(qAbs(intersection.GetX() - 0.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(intersection.GetY() - 0.0) < I_BIG_EPSILON);
	QVERIFY(qAbs(intersection.GetZ() - 0.0) < I_BIG_EPSILON);
}


void CI3dTest::testBarycentricCoordinates()
{
	i3d::CVector3d p1(0.0, 0.0, 0.0);
	i3d::CVector3d p2(1.0, 0.0, 0.0);
	i3d::CVector3d p3(0.0, 1.0, 0.0);
	
	// Center of triangle
	i3d::CVector3d point(0.333, 0.333, 0.0);
	
	double u, v, w;
	bool result = i3d::Geometry::GetBarycentricCoordinates(point, p1, p2, p3, u, v, w);
	
	QVERIFY(result);
	QVERIFY(qAbs(u + v + w - 1.0) < 0.01);
}
