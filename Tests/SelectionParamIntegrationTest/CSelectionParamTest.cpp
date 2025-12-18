#include "CSelectionParamTest.h"


// ACF includes
#include <iprm/IParamsManager.h>
#include <iprm/CSelectionParam.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


// protected slots

void CSelectionParamTest::initTestCase()
{
	m_testPartituraInstanceCompPtr.reset(new CSelectionParamIntegrationTest);

	m_selectionPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("Selection");
	m_selectionWithConstraintsPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::ISelectionParam>("SelectionWithConstraints");
	m_selectionConstraintsPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IOptionsManager>("SelectionConstraints");

	QVERIFY(m_selectionPtr != nullptr);
	QVERIFY(m_selectionWithConstraintsPtr != nullptr);
	QVERIFY(m_selectionConstraintsPtr != nullptr);
}


void CSelectionParamTest::GetSetSelectionTest()
{
	QVERIFY(m_selectionPtr->GetSelectedOptionIndex() < 0);

	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(10));

	QVERIFY(m_selectionPtr->GetSelectedOptionIndex() == 10);
}


void CSelectionParamTest::ResetDataTest()
{
	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(10));

	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(-1));

	QVERIFY(m_selectionPtr->GetSelectedOptionIndex() < 0);
}


void CSelectionParamTest::CopyTest()
{
	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(10));

	iprm::CSelectionParam selectionParam;

	QVERIFY(selectionParam.CopyFrom(*m_selectionPtr));

	QVERIFY(selectionParam.GetSelectedOptionIndex() == 10);
}


void CSelectionParamTest::CloneTest()
{
	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(10));

	istd::TUniqueInterfacePtr<iprm::ISelectionParam> clonePtr;
	clonePtr.MoveCastedPtr(m_selectionPtr->CloneMe());

	QVERIFY(clonePtr.IsValid());
	QVERIFY(clonePtr->GetSelectedOptionIndex() == 10);
}


void CSelectionParamTest::SerializeTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);

	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(10));

	QVERIFY(m_selectionPtr->Serialize(writeArchive));

	QVERIFY(m_selectionPtr->SetSelectedOptionIndex(-1));

	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_selectionPtr->Serialize(readArchive));

	QVERIFY(m_selectionPtr->GetSelectedOptionIndex() == 10);
}


void CSelectionParamTest::GetSetSelectionWithConstraintsTest()
{
}


void CSelectionParamTest::ResetDataWithConstraintsTest()
{
}


void CSelectionParamTest::CopyWithConstraintsTest()
{
}


void CSelectionParamTest::CloneWithConstraintsTest()
{
}


void CSelectionParamTest::SerializeWithConstraintsTest()
{
}


void CSelectionParamTest::TestParamsManager()
{
	iprm::IParamsManager* paramsManagerPtr = m_testPartituraInstanceCompPtr->GetInterface<iprm::IParamsManager>("ParamsManager");
	QVERIFY(paramsManagerPtr != nullptr);

	const iprm::IParamsSet* fixedParamsSetPtr = paramsManagerPtr->GetParamsSet(0);
	QVERIFY(fixedParamsSetPtr != nullptr);

	int newIndex = paramsManagerPtr->InsertParamsSet();
	QVERIFY(newIndex == 1);

	const iprm::IParamsSet* paramsSetPtr = paramsManagerPtr->GetParamsSet(newIndex);
	QVERIFY(paramsSetPtr != nullptr);

	istd::TUniqueInterfacePtr<iprm::IParamsSet> paramsSetClonePtr;
	paramsSetClonePtr.MoveCastedPtr(paramsSetPtr->CloneMe());
	QVERIFY(paramsSetClonePtr.IsValid());

	paramsManagerPtr->RemoveParamsSet(1);
	Q_ASSERT(paramsManagerPtr->GetParamsSetsCount() == 1);
	QVERIFY(paramsSetClonePtr.IsValid());
}


void CSelectionParamTest::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CSelectionParamTest);
