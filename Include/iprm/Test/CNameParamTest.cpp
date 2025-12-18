#include <iprm/Test/CNameParamTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


void CNameParamTest::initTestCase()
{
}


void CNameParamTest::GetSetTest()
{
	QVERIFY(m_param.GetName().isEmpty());

	m_param.SetName(m_refValue);

	QVERIFY(m_param.GetName() == m_refValue);
}


void CNameParamTest::ResetTest()
{
	QVERIFY(m_param.GetName() == m_refValue);

	QVERIFY(m_param.ResetData());

	QVERIFY(m_param.GetName().isEmpty());
}


void CNameParamTest::SerializeTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);

	m_param.SetName(m_refValue);

	QVERIFY(m_param.Serialize(writeArchive));

	QVERIFY(m_param.ResetData());
	QVERIFY(m_param.GetName().isEmpty());

	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(m_param.Serialize(readArchive));

	QVERIFY(m_param.GetName() == m_refValue);
}


void CNameParamTest::CopyFromTest()
{
	iprm::CNameParam param;

	QVERIFY(m_param.GetName() == m_refValue);
	QVERIFY(param.GetName().isEmpty());

	QVERIFY(param.CopyFrom(m_param));

	QVERIFY(param.GetName() == m_refValue);
}


void CNameParamTest::CloneTest()
{
	QVERIFY(m_param.GetName() == m_refValue);

	istd::TUniqueInterfacePtr<iprm::CNameParam> clonePtr;
	QVERIFY(clonePtr.MoveCastedPtr(m_param.CloneMe()));
	QVERIFY(clonePtr.IsValid());

	QVERIFY(clonePtr->GetName() == m_refValue);
}


void CNameParamTest::IsEqualTest()
{
	iprm::CNameParam param;
	param.SetName(m_refValue);

	QVERIFY(m_param.IsEqual(param));
}


void CNameParamTest::cleanupTestCase()
{
}


I_ADD_TEST(CNameParamTest);
