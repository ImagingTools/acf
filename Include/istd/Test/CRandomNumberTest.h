#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CRandomNumber.h>
#include <itest/CStandardTestExecutor.h>

class CRandomNumberTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetNextIntTest();
	void GetNextIntRangeTest();
	void GetNextDoubleTest();
	void GetNextDoubleRangeTest();

	void cleanupTestCase();

private:
	istd::CRandomNumber* m_randomNumberPtr = nullptr;
};


