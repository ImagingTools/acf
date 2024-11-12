#include "CSelectionParamTest.h"


// ACF includes
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

	istd::TDelPtr<iprm::ISelectionParam> clonePtr;
	clonePtr.SetCastedOrRemove(m_selectionPtr->CloneMe());

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


void CSelectionParamTest::cleanupTestCase()
{
	m_testPartituraInstanceCompPtr.reset();
}


I_ADD_TEST(CSelectionParamTest);
