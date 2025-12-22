#include "CMemoryArchiveTest.h"


// Qt includes
#include <QtCore/QString>


// protected slots

void CMemoryArchiveTest::initTestCase()
{
}


void CMemoryArchiveTest::WriteReadIntTest()
{
	// Write some integers to memory archive
	iser::CMemoryWriteArchive writeArchive(nullptr);
	
	int value1 = 42;
	int value2 = -123;
	int value3 = 0;
	
	QVERIFY(writeArchive.ProcessData(&value1, sizeof(int)));
	QVERIFY(writeArchive.ProcessData(&value2, sizeof(int)));
	QVERIFY(writeArchive.ProcessData(&value3, sizeof(int)));
	
	// Read the integers back
	iser::CMemoryReadArchive readArchive(writeArchive);
	
	int readValue1 = 0;
	int readValue2 = 0;
	int readValue3 = 0;
	
	QVERIFY(readArchive.ProcessData(&readValue1, sizeof(int)));
	QVERIFY(readArchive.ProcessData(&readValue2, sizeof(int)));
	QVERIFY(readArchive.ProcessData(&readValue3, sizeof(int)));
	
	// Verify the values
	QVERIFY(readValue1 == value1);
	QVERIFY(readValue2 == value2);
	QVERIFY(readValue3 == value3);
}


void CMemoryArchiveTest::WriteReadDoubleTest()
{
	// Write some doubles to memory archive
	iser::CMemoryWriteArchive writeArchive(nullptr);
	
	double value1 = 3.14159;
	double value2 = -2.71828;
	double value3 = 0.0;
	
	QVERIFY(writeArchive.ProcessData(&value1, sizeof(double)));
	QVERIFY(writeArchive.ProcessData(&value2, sizeof(double)));
	QVERIFY(writeArchive.ProcessData(&value3, sizeof(double)));
	
	// Read the doubles back
	iser::CMemoryReadArchive readArchive(writeArchive);
	
	double readValue1 = 0.0;
	double readValue2 = 0.0;
	double readValue3 = 0.0;
	
	QVERIFY(readArchive.ProcessData(&readValue1, sizeof(double)));
	QVERIFY(readArchive.ProcessData(&readValue2, sizeof(double)));
	QVERIFY(readArchive.ProcessData(&readValue3, sizeof(double)));
	
	// Verify the values
	QVERIFY(qAbs(readValue1 - value1) < 0.00001);
	QVERIFY(qAbs(readValue2 - value2) < 0.00001);
	QVERIFY(qAbs(readValue3 - value3) < 0.00001);
}


void CMemoryArchiveTest::WriteReadStringTest()
{
	// Write a string to memory archive
	iser::CMemoryWriteArchive writeArchive(nullptr);
	
	QString originalString = "Hello, World!";
	QVERIFY(writeArchive.ProcessString(originalString));
	
	// Read the string back
	iser::CMemoryReadArchive readArchive(writeArchive);
	
	QString readString;
	QVERIFY(readArchive.ProcessString(readString));
	
	// Verify the string
	QVERIFY(readString == originalString);
}


void CMemoryArchiveTest::WriteReadBoolTest()
{
	// Write booleans to memory archive
	iser::CMemoryWriteArchive writeArchive(nullptr);
	
	bool value1 = true;
	bool value2 = false;
	
	QVERIFY(writeArchive.ProcessData(&value1, sizeof(bool)));
	QVERIFY(writeArchive.ProcessData(&value2, sizeof(bool)));
	
	// Read the booleans back
	iser::CMemoryReadArchive readArchive(writeArchive);
	
	bool readValue1 = false;
	bool readValue2 = true;
	
	QVERIFY(readArchive.ProcessData(&readValue1, sizeof(bool)));
	QVERIFY(readArchive.ProcessData(&readValue2, sizeof(bool)));
	
	// Verify the values
	QVERIFY(readValue1 == value1);
	QVERIFY(readValue2 == value2);
}


void CMemoryArchiveTest::ResetPositionTest()
{
	// Write some data
	iser::CMemoryWriteArchive writeArchive(nullptr);
	int value = 12345;
	QVERIFY(writeArchive.ProcessData(&value, sizeof(int)));
	
	// Create read archive
	iser::CMemoryReadArchive readArchive(writeArchive);
	
	// Read the value
	int readValue1 = 0;
	QVERIFY(readArchive.ProcessData(&readValue1, sizeof(int)));
	QVERIFY(readValue1 == value);
	
	// Reset position
	readArchive.ResetPosition();
	
	// Read again
	int readValue2 = 0;
	QVERIFY(readArchive.ProcessData(&readValue2, sizeof(int)));
	QVERIFY(readValue2 == value);
}


void CMemoryArchiveTest::IsValidTest()
{
	iser::CMemoryWriteArchive writeArchive(nullptr);
	int value = 42;
	QVERIFY(writeArchive.ProcessData(&value, sizeof(int)));
	
	iser::CMemoryReadArchive readArchive(writeArchive);
	
	// Should be valid initially
	QVERIFY(readArchive.IsValid());
	
	// Read the data
	int readValue = 0;
	QVERIFY(readArchive.ProcessData(&readValue, sizeof(int)));
	
	// Should still be valid after reading valid data
	QVERIFY(readArchive.IsValid());
}


void CMemoryArchiveTest::cleanupTestCase()
{
}


I_ADD_TEST(CMemoryArchiveTest);
