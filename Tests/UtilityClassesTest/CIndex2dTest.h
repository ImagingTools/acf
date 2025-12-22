#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CIndex2d.h>
#include <itest/CStandardTestExecutor.h>

class CIndex2dTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void GetSetTest();
	void ComparisonOperatorsTest();
	void QPointConversionTest();
	void QSizeConversionTest();

	void cleanupTestCase();
};


