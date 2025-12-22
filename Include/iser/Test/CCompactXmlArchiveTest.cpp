#include <iser/Test/CCompactXmlArchiveTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>


void CCompactXmlArchiveTest::BasicCompactXmlSerializationTest()
{
	// Write data
	SimpleModel writeModel;
	writeModel.value = 99;

	iser::CCompactXmlMemWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));
	QByteArray xmlData = writeArchive.GetData();

	// Read data
	SimpleModel readModel;
	iser::CCompactXmlMemReadArchive readArchive(xmlData);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QCOMPARE(readModel.value, 99);
}


void CCompactXmlArchiveTest::CompactXmlComplexDataTest()
{
	// Write data
	ComplexModel writeModel;
	writeModel.intValue = 777;
	writeModel.doubleValue = 123.456;
	writeModel.stringValue = "Compact XML Test";

	iser::CCompactXmlMemWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));
	QByteArray xmlData = writeArchive.GetData();

	// Read data
	ComplexModel readModel;
	iser::CCompactXmlMemReadArchive readArchive(xmlData);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QVERIFY(readModel == writeModel);
	QCOMPARE(readModel.stringValue, QString("Compact XML Test"));
}


void CCompactXmlArchiveTest::CompactXmlArrayTest()
{
	// Write data
	ArrayModel writeModel;
	writeModel.numbers << 1.1 << 2.2 << 3.3 << 4.4;

	iser::CCompactXmlMemWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));
	QByteArray xmlData = writeArchive.GetData();

	// Read data
	ArrayModel readModel;
	iser::CCompactXmlMemReadArchive readArchive(xmlData);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QVERIFY(readModel == writeModel);
	QCOMPARE(readModel.numbers.size(), 4);
}


I_ADD_TEST(CCompactXmlArchiveTest);
