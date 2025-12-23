#include "CIndex2dTest.h"


// protected slots

void CIndex2dTest::initTestCase()
{
}


void CIndex2dTest::ConstructorTest()
{
	// Test default constructor
	istd::CIndex2d index1;
	
	// Test constructor with x,y values
	istd::CIndex2d index2(10, 20);
	QVERIFY(index2.GetX() == 10);
	QVERIFY(index2.GetY() == 20);
	
	// Test copy constructor
	istd::CIndex2d index3(index2);
	QVERIFY(index3.GetX() == 10);
	QVERIFY(index3.GetY() == 20);
	
	// Test constructor from QPoint
	QPoint point(5, 15);
	istd::CIndex2d index4(point);
	QVERIFY(index4.GetX() == 5);
	QVERIFY(index4.GetY() == 15);
	
	// Test constructor from QSize
	QSize size(30, 40);
	istd::CIndex2d index5(size);
	QVERIFY(index5.GetX() == 30);
	QVERIFY(index5.GetY() == 40);
}


void CIndex2dTest::GetSetTest()
{
	istd::CIndex2d index;
	
	// Test SetX and GetX
	index.SetX(100);
	QVERIFY(index.GetX() == 100);
	
	// Test SetY and GetY
	index.SetY(200);
	QVERIFY(index.GetY() == 200);
	
	// Verify both values are set correctly
	QVERIFY(index.GetX() == 100);
	QVERIFY(index.GetY() == 200);
}


void CIndex2dTest::ComparisonOperatorsTest()
{
	istd::CIndex2d index1(10, 20);
	istd::CIndex2d index2(10, 20);
	istd::CIndex2d index3(5, 20);
	istd::CIndex2d index4(10, 10);
	
	// Test equality
	QVERIFY(index1 == index2);
	QVERIFY(!(index1 == index3));
	
	// Test inequality
	QVERIFY(index1 != index3);
	QVERIFY(!(index1 != index2));
	
	// Test less than (compares Y first, then X)
	QVERIFY(index4 < index1);  // 10 < 20 (Y comparison)
	QVERIFY(index3 < index1);  // Same Y, 5 < 10 (X comparison)
	
	// Test greater than
	QVERIFY(index1 > index4);
	QVERIFY(index1 > index3);
	
	// Test less than or equal
	QVERIFY(index4 <= index1);
	QVERIFY(index1 <= index2);
	
	// Test greater than or equal
	QVERIFY(index1 >= index4);
	QVERIFY(index1 >= index2);
}


void CIndex2dTest::QPointConversionTest()
{
	// Test conversion to QPoint
	istd::CIndex2d index(15, 25);
	QPoint point = index;
	QVERIFY(point.x() == 15);
	QVERIFY(point.y() == 25);
	
	// Test conversion to QPointF
	QPointF pointF = index;
	QVERIFY(qAbs(pointF.x() - 15.0) < 0.001);
	QVERIFY(qAbs(pointF.y() - 25.0) < 0.001);
	
	// Test round-trip conversion
	QPoint point2(100, 200);
	istd::CIndex2d index2(point2);
	QPoint point3 = index2;
	QVERIFY(point2 == point3);
}


void CIndex2dTest::QSizeConversionTest()
{
	// Test conversion to QSize
	istd::CIndex2d index(640, 480);
	QSize size = index;
	QVERIFY(size.width() == 640);
	QVERIFY(size.height() == 480);
	
	// Test round-trip conversion
	QSize size2(1024, 768);
	istd::CIndex2d index2(size2);
	QSize size3 = index2;
	QVERIFY(size2 == size3);
}


void CIndex2dTest::cleanupTestCase()
{
}


I_ADD_TEST(CIndex2dTest);
