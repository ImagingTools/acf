#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imath/CDouble.h>
#include <itest/CStandardTestExecutor.h>

class CDoubleTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void OperatorsTest();
	void ComparisonOperatorsTest();
	void ArithmeticOperatorsTest();
	void GetRoundedTest();
	void IsRoundedEqualTest();
	void GetRoundedDownTest();
	void GetRoundedUpTest();
	void IsSimiliarTest();

	void cleanupTestCase();
};


