#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsManager.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/SelectionParamIntegrationTest/CSelectionParamIntegrationTest.h>

class CSelectionParamTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetSetSelectionTest();
	void ResetDataTest();
	void CopyTest();
	void CloneTest();
	void SerializeTest();

	void GetSetSelectionWithConstraintsTest();
	void ResetDataWithConstraintsTest();
	void CopyWithConstraintsTest();
	void CloneWithConstraintsTest();
	void SerializeWithConstraintsTest();
	void TestParamsManager();

	void cleanupTestCase();

private:
	std::shared_ptr<CSelectionParamIntegrationTest> m_testPartituraInstanceCompPtr;

	iprm::ISelectionParam* m_selectionPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithConstraintsPtr = nullptr;
	iprm::IOptionsManager* m_selectionConstraintsPtr = nullptr;
};


