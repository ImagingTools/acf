#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <imath/CLinearInterpolator.h>
#include <itest/CStandardTestExecutor.h>

class CLinearInterpolatorTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void BasicInterpolationTest();
	void ExtrapolationTest();
	void EdgeCasesTest();

	void cleanupTestCase();
};


