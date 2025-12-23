#include <iprm/Test/CIdParamTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iprm/CNameParam.h>


void CIdParamTest::initTestCase()
{

}


void CIdParamTest::GetSetTest()
{
	QVERIFY(m_param.GetId().isEmpty());

	m_param.SetId(m_id);

	QVERIFY(m_param.GetId() == m_id);
}


void CIdParamTest::SetSameIdTest()
{
	// Verify that setting the same ID value multiple times works correctly
	m_param.SetId(m_id);
	QVERIFY(m_param.GetId() == m_id);

	// Set the same value again - should remain unchanged
	m_param.SetId(m_id);
	QVERIFY(m_param.GetId() == m_id);
}


void CIdParamTest::SetEmptyIdTest()
{
	// Test setting empty ID
	m_param.SetId(m_id);
	QVERIFY(m_param.GetId() == m_id);

	m_param.SetId(QByteArray());
	QVERIFY(m_param.GetId().isEmpty());
}


void CIdParamTest::GetSupportedOperationsTest()
{
	// Test that all expected operations are supported
	int operations = m_param.GetSupportedOperations();
	QVERIFY(operations & istd::IChangeable::SO_CLONE);
	QVERIFY(operations & istd::IChangeable::SO_COPY);
	QVERIFY(operations & istd::IChangeable::SO_COMPARE);
	QVERIFY(operations & istd::IChangeable::SO_RESET);
}


void CIdParamTest::ResetTest()
{
	m_param.SetId(m_id);
	QVERIFY(m_param.GetId() == m_id);
	QVERIFY(m_param.ResetData());
	QVERIFY(m_param.GetId().isEmpty());
}


void CIdParamTest::ResetEmptyTest()
{
	// Test resetting when already empty - using a fresh instance to ensure clean state
	iprm::CIdParam param;
	QVERIFY(param.GetId().isEmpty());
	QVERIFY(param.ResetData());
	QVERIFY(param.GetId().isEmpty());
}


void CIdParamTest::SerializeTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);

	m_param.SetId(m_id);

	QVERIFY(m_param.Serialize(writeArchive));

	QVERIFY(m_param.ResetData());
	QVERIFY(m_param.GetId().isEmpty());

	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_param.Serialize(readArchive));

	QVERIFY(m_param.GetId() == m_id);
}


void CIdParamTest::CopyFromTest()
{
	iprm::CIdParam param;

	QVERIFY(m_param.GetId() == m_id);
	QVERIFY(param.GetId().isEmpty());

	QVERIFY(param.CopyFrom(m_param));

	QVERIFY(param.GetId() == m_id);
}


void CIdParamTest::CopyFromIncompatibleTest()
{
	// Test CopyFrom with incompatible type
	iprm::CNameParam nameParam;
	nameParam.SetName("TestName");

	iprm::CIdParam param;
	param.SetId(m_id);

	// CopyFrom should fail with incompatible type
	QVERIFY(!param.CopyFrom(nameParam));
	// ID should remain unchanged
	QVERIFY(param.GetId() == m_id);
}


void CIdParamTest::CloneTest()
{
	QVERIFY(m_param.GetId() == m_id);

	istd::TUniqueInterfacePtr<iprm::IIdParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(m_param.CloneMe()));
	QVERIFY(clonePtr.IsValid());

	QVERIFY(clonePtr->GetId() == m_id);
}


void CIdParamTest::CloneWithModeTest()
{
	// Test clone with explicit compatibility mode
	m_param.SetId(m_id);
	
	istd::TUniqueInterfacePtr<iprm::IIdParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(m_param.CloneMe(istd::IChangeable::CM_WITHOUT_REFS)));
	QVERIFY(clonePtr.IsValid());
	QVERIFY(clonePtr->GetId() == m_id);

	// Test with CM_WITH_REFS mode
	istd::TUniqueInterfacePtr<iprm::IIdParam> clonePtr2;
	QVERIFY(clonePtr2.MoveCastedPtr(m_param.CloneMe(istd::IChangeable::CM_WITH_REFS)));
	QVERIFY(clonePtr2.IsValid());
	QVERIFY(clonePtr2->GetId() == m_id);
}


void CIdParamTest::IsEqualTest()
{
	iprm::CIdParam param;
	param.SetId(m_id);

	QVERIFY(m_param.IsEqual(param));
}


void CIdParamTest::IsEqualDifferentIdTest()
{
	// Test IsEqual with different IDs
	iprm::CIdParam param1;
	param1.SetId(m_id);

	iprm::CIdParam param2;
	param2.SetId("different_id");

	QVERIFY(!param1.IsEqual(param2));
	QVERIFY(!param2.IsEqual(param1));
}


void CIdParamTest::IsEqualIncompatibleTest()
{
	// Test IsEqual with incompatible type
	iprm::CNameParam nameParam;
	nameParam.SetName("TestName");

	iprm::CIdParam param;
	param.SetId(m_id);

	// IsEqual should return false for incompatible type
	QVERIFY(!param.IsEqual(nameParam));
}


void CIdParamTest::cleanupTestCase()
{
}


I_ADD_TEST(CIdParamTest);


