#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CComponentSimulationTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void DoSimiulationTest();
};


