#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/IParamsManager.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/ParamsManagerTest/CParamsManagerTest.h>

class CParamsManagerTestRunner: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetIndexOperationFlagsTest();
	void SetIndexOperationFlagsTest();
	void GetParamsSetsCountTest();
	void GetParamsTypeConstraintsTest();
	void InsertParamsSetTest();
	void RemoveParamsSetTest();
	void SwapParamsSetTest();
	void GetParamsSetTest();
	void CreateParameterSetTest();
	void GetSetParamsSetNameTest();
	void GetSetParamsSetDescriptionTest();
	void SerializeTest();
	void CopyTest();

	void cleanupTestCase();

private:
	std::shared_ptr<CParamsManagerTest> m_testPartituraInstanceCompPtr;
	iprm::IParamsManager* m_paramsManagerPtr = nullptr;
};

