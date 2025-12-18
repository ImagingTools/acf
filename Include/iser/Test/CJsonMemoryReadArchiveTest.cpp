#include <iser/Test/CJsonMemoryReadArchiveTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>


void CJsonMemoryReadArchiveTest::DoTest()
{
	Model model;
	model.value = 42;
	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(model.Serialize(writeArchive));
	QByteArray data = writeArchive.GetData();

	Model model2;
	model2.value = 0;

	iser::CJsonMemReadArchive readArchive(data);
	QVERIFY(model2.Serialize(readArchive));
	QVERIFY(model2.value == 42);
}


void CJsonMemoryReadArchiveTest::DoArrayTest()
{
	QList<double> data1;
	data1 << 1.0 << 1.1 << 1.2;
	QList<double> data2;
	data2 << 2.0 << 2.1 << 2.2;
	QList<QList<double>> modelData;
	modelData << data1 << data2;

	NestedArray model(modelData);
	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(model.Serialize(writeArchive));
	QByteArray data = writeArchive.GetData();

	NestedArray model2;

	iser::CJsonMemReadArchive readArchive(data);
	QVERIFY(model2.Serialize(readArchive));
	QVERIFY(model2 == model);
}

I_ADD_TEST(CJsonMemoryReadArchiveTest);
