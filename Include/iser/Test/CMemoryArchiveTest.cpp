#include <iser/Test/CMemoryArchiveTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>


void CMemoryArchiveTest::BasicSerializationTest()
{
	// Write data
	SimpleModel writeModel;
	writeModel.value = 42;

	iser::CMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	// Read data
	SimpleModel readModel;
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QCOMPARE(readModel.value, 42);
}


void CMemoryArchiveTest::PrimitiveTypesTest()
{
	// Write data
	ComplexModel writeModel;
	writeModel.intValue = 123;
	writeModel.doubleValue = 456.789;
	writeModel.stringValue = "Test String";

	iser::CMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	// Read data
	ComplexModel readModel;
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QVERIFY(readModel == writeModel);
}


void CMemoryArchiveTest::StringSerializationTest()
{
	ComplexModel writeModel;
	writeModel.intValue = 999;
	writeModel.doubleValue = 3.14159;
	writeModel.stringValue = "Unicode test: äöü ñ 中文";

	iser::CMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	ComplexModel readModel;
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel.Serialize(readArchive));

	QVERIFY(readModel == writeModel);
	QCOMPARE(readModel.stringValue, QString("Unicode test: äöü ñ 中文"));
}


void CMemoryArchiveTest::ResetPositionTest()
{
	// Write data
	SimpleModel writeModel;
	writeModel.value = 42;

	iser::CMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	// Read data first time
	SimpleModel readModel1;
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel1.Serialize(readArchive));
	QCOMPARE(readModel1.value, 42);

	// Reset and read again
	readArchive.ResetPosition();
	SimpleModel readModel2;
	QVERIFY(readModel2.Serialize(readArchive));
	QCOMPARE(readModel2.value, 42);
}


I_ADD_TEST(CMemoryArchiveTest);
