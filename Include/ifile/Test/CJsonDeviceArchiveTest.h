#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CJsonDeviceArchiveTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoFileTest();
	void DoMemoryTest();
	void DoPersistenceComponentTest();
};


