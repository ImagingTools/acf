#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <i3d/CVector3d.h>
#include <itest/CStandardTestExecutor.h>

class CVector3dTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void GetSetTest();
	void ArithmeticOperatorsTest();
	void GetTranslatedTest();
	void GetCrossProductTest();
	void GetNormalizedTest();
	void GetPlaneCastedTest();

	void cleanupTestCase();
};


