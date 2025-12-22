#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <itest/CStandardTestExecutor.h>

class CMemoryArchiveTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void WriteReadIntTest();
	void WriteReadDoubleTest();
	void WriteReadStringTest();
	void WriteReadBoolTest();
	void ResetPositionTest();
	void IsValidTest();

	void cleanupTestCase();
};


