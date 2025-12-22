#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <i2d/CVector2d.h>
#include <itest/CStandardTestExecutor.h>

class CVector2dTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void GetSetTest();
	void ArithmeticOperatorsTest();
	void GetTranslatedTest();
	void GetAngleTest();
	void GetOrthogonalTest();
	void GetNormalizedTest();
	void GetCrossProductZTest();
	void GetDotProductTest();

	void cleanupTestCase();
};


