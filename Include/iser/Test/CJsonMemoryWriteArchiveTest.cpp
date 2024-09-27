#include "CJsonMemoryWriteArchiveTest.h"


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


void CJsonMemoryWriteArchiveTest::initTestCase()
{
	m_metaInfo.SetMetaInfo(0, QByteArray("Test ByteArray"));
	m_metaInfo.SetMetaInfo(1, QString("Test String"));
	m_metaInfo.SetMetaInfo(2, int(10));
	m_metaInfo.SetMetaInfo(3, double(10.0));
}


void CJsonMemoryWriteArchiveTest::WriteTest()
{
	{
		iser::CJsonMemWriteArchive writeArchive(m_buffer);
		QVERIFY(m_metaInfo.Serialize(writeArchive));
	}
}

void CJsonMemoryWriteArchiveTest::ReadTest()
{
	idoc::CStandardDocumentMetaInfo metaInfo;
	iser::CJsonMemReadArchive readArchive(m_buffer);
	QVERIFY(metaInfo.Serialize(readArchive));
	QVERIFY(m_metaInfo.IsEqual(metaInfo));
}

void CJsonMemoryWriteArchiveTest::_Test()
{
	m_buffer.clear();

	Model model;

	model.value = 10;

	{
		iser::CJsonMemWriteArchive writeArchive(m_buffer);
		QVERIFY(model.Serialize(writeArchive));
	}

	model.value = 0;

	iser::CJsonMemReadArchive readArchive(m_buffer);
	QVERIFY(model.Serialize(readArchive));
}


void CJsonMemoryWriteArchiveTest::cleanupTestCase()
{
}


I_ADD_TEST(CJsonMemoryWriteArchiveTest);
