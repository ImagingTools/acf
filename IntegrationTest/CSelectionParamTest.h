#pragma once


// ACF includes
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsManager.h>
#include <IntegrationTest/CPartituraBasedTest.h>


const char partituraName[] = "CSelectionParamTest";
class CSelectionParamTest : public CPartituraBasedTest<partituraName>
{
	Q_OBJECT
public:
	typedef CPartituraBasedTest<partituraName> BaseClass;

private slots:
	virtual void initTestCase() override;
	virtual void GetSetSelectionTest();
	virtual void ResetDataTest();
	virtual void CopyTest();
	virtual void CloneTest();
	virtual void SerializeTest();

	virtual void GetSetSelectionWithConstraintsTest();
	virtual void ResetDataWithConstraintsTest();
	virtual void CopyWithConstraintsTest();
	virtual void CloneWithConstraintsTest();
	virtual void SerializeWithConstraintsTest();

	virtual void cleanupTestCase() override;

private:
	iprm::ISelectionParam* m_selectionPtr = nullptr;
	iprm::ISelectionParam* m_selectionWithConstraintsPtr = nullptr;
	iprm::IOptionsManager* m_selectionConstraintsPtr = nullptr;
};


