#include <iser/Test/CBitMemoryArchiveTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>


void CBitMemoryArchiveTest::BasicBitSerializationTest()
{
	// Write data
	BoolModel writeModel;
	writeModel.value = true;

	iser::CBitMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	// Read data
	BoolModel readModel;
	iser::CBitMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QCOMPARE(readModel.value, true);
}


void CBitMemoryArchiveTest::MultipleBoolsTest()
{
	// Write data
	MultiBoolModel writeModel;
	writeModel.value1 = true;
	writeModel.value2 = false;
	writeModel.value3 = true;

	iser::CBitMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	// Read data
	MultiBoolModel readModel;
	iser::CBitMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QVERIFY(readModel == writeModel);
	QCOMPARE(readModel.value1, true);
	QCOMPARE(readModel.value2, false);
	QCOMPARE(readModel.value3, true);
}


void CBitMemoryArchiveTest::MixedDataTest()
{
	// Write data
	MixedModel writeModel;
	writeModel.boolValue = true;
	writeModel.intValue = 42;

	iser::CBitMemoryWriteArchive writeArchive;
	QVERIFY(writeModel.Serialize(writeArchive));

	// Read data
	MixedModel readModel;
	iser::CBitMemoryReadArchive readArchive(writeArchive);
	QVERIFY(readModel.Serialize(readArchive));

	// Verify
	QVERIFY(readModel == writeModel);
	QCOMPARE(readModel.boolValue, true);
	QCOMPARE(readModel.intValue, 42);
}


I_ADD_TEST(CBitMemoryArchiveTest);
