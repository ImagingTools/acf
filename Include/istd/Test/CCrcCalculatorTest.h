#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CCrcCalculator.h>
#include <itest/CStandardTestExecutor.h>

class CCrcCalculatorTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetCrcFromDataTest();
	void GetCrcFromStreamTest();
	void CrcConsistencyTest();

	void cleanupTestCase();
};


