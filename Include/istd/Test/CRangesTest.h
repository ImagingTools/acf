#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/TRanges.h>
#include <itest/CStandardTestExecutor.h>


class CRangesTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Constructor tests
	void DefaultConstructorTest();
	void RangeConstructorTest();
	
	// Basic property tests
	void IsEmptyTest();
	void ResetTest();
	void GetSetBeginStateTest();
	
	// Switch point tests
	void InsertSwitchPointTest();
	void GetSwitchPointsTest();
	
	// Membership tests
	void IsInsidePointTest();
	void IsInsideRangeTest();
	void IsInsideRangesTest();
	
	// Inversion tests
	void GetInvertedTest();
	void InvertTest();
	
	// Union tests
	void GetUnionRangesTest();
	void UnionRangesTest();
	void UnionRangeTest();
	
	// Intersection tests
	void GetIntersectionRangesTest();
	void IntersectionRangesTest();
	void IntersectionRangeTest();
	
	// Morphological operations
	void ErodeTest();
	void DilateTest();
	void RemoveGapsTest();
	
	// Translation test
	void ShiftRangesTest();
	
	// Conversion tests
	void GetAsListTest();
	
	// Operator tests
	void EqualityOperatorTest();
	void HashTest();
	
	// CIntRanges specific tests
	void CIntRangesTest();

	void cleanupTestCase();
};
