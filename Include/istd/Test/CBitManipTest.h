#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CBitManip.h>
#include <itest/CStandardTestExecutor.h>

class CBitManipTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetFirstBitIndexTest();
	void GetFirstBitIndexEdgeCasesTest();

	void cleanupTestCase();
};


