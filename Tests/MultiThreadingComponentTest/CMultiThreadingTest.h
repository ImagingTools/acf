#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsManager.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/MultiThreadingComponentTest/CMultiThreadingComponentTest.h>

class CMultiThreadingTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void RunParallel4();

	void cleanupTestCase();

private:
	std::shared_ptr<CMultiThreadingComponentTest> m_testPartituraInstanceCompPtr;
};


