#include <iprm/Test/CIdParamTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


void CIdParamTest::initTestCase()
{

}


void CIdParamTest::GetSetTest()
{
	QVERIFY(m_param.GetId().isEmpty());

	m_param.SetId(m_id);

	QVERIFY(m_param.GetId() == m_id);
}


void CIdParamTest::ResetTest()
{
	QVERIFY(m_param.GetId() == m_id);

	QVERIFY(m_param.ResetData());

	QVERIFY(m_param.GetId().isEmpty());
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


void CIdParamTest::CloneTest()
{
	QVERIFY(m_param.GetId() == m_id);

	istd::TDelPtr<iprm::CIdParam> clonePtr;
	QVERIFY(clonePtr.SetCastedOrRemove(m_param.CloneMe()));
	QVERIFY(clonePtr.IsValid());

	QVERIFY(clonePtr->GetId() == m_id);
}


void CIdParamTest::IsEqualTest()
{
	iprm::CIdParam param;
	param.SetId(m_id);

	QVERIFY(m_param.IsEqual(param));
}


void CIdParamTest::cleanupTestCase()
{
}


I_ADD_TEST(CIdParamTest);


