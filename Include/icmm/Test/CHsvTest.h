#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <icmm/CHsv.h>
#include <itest/CStandardTestExecutor.h>

class CHsvTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void ConstructorTest();
	void GetSetTest();
	void ArithmeticOperatorsTest();
	void CompoundOperatorsTest();

	void cleanupTestCase();
};


