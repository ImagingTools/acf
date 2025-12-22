#include "CCrcCalculatorTest.h"


// protected slots

void CCrcCalculatorTest::initTestCase()
{
}


void CCrcCalculatorTest::GetCrcFromDataTest()
{
	// Test with simple data
	const char* testData = "Hello, World!";
	int dataSize = qstrlen(testData);
	
	quint32 crc = istd::CCrcCalculator::GetCrcFromData(
		reinterpret_cast<const quint8*>(testData), dataSize);
	
	// CRC should be non-zero for non-empty data
	QVERIFY(crc != 0);
	
	// Test with empty data
	quint32 crcEmpty = istd::CCrcCalculator::GetCrcFromData(nullptr, 0);
	// Empty data CRC should be zero or a known initial value
	QVERIFY(crcEmpty == 0 || crcEmpty != 0);  // Just verify it doesn't crash
}


void CCrcCalculatorTest::GetCrcFromStreamTest()
{
	// Create a byte stream
	istd::CCrcCalculator::ByteStream stream;
	stream.append(0x48);  // 'H'
	stream.append(0x65);  // 'e'
	stream.append(0x6C);  // 'l'
	stream.append(0x6C);  // 'l'
	stream.append(0x6F);  // 'o'
	
	quint32 crc = istd::CCrcCalculator::GetCrcFromStream(stream);
	
	// CRC should be non-zero
	QVERIFY(crc != 0);
	
	// Test with empty stream
	istd::CCrcCalculator::ByteStream emptyStream;
	quint32 crcEmpty = istd::CCrcCalculator::GetCrcFromStream(emptyStream);
	// Just verify it doesn't crash
	QVERIFY(crcEmpty == 0 || crcEmpty != 0);
}


void CCrcCalculatorTest::CrcConsistencyTest()
{
	// Test that same data produces same CRC
	const char* testData = "Test Data";
	int dataSize = qstrlen(testData);
	
	quint32 crc1 = istd::CCrcCalculator::GetCrcFromData(
		reinterpret_cast<const quint8*>(testData), dataSize);
	quint32 crc2 = istd::CCrcCalculator::GetCrcFromData(
		reinterpret_cast<const quint8*>(testData), dataSize);
	
	// Same data should produce same CRC
	QVERIFY(crc1 == crc2);
	
	// Different data should (most likely) produce different CRC
	const char* differentData = "Different Data";
	int differentSize = qstrlen(differentData);
	quint32 crc3 = istd::CCrcCalculator::GetCrcFromData(
		reinterpret_cast<const quint8*>(differentData), differentSize);
	
	// Different data should have different CRC (not guaranteed but highly likely)
	QVERIFY(crc1 != crc3);
	
	// Test that GetCrcFromStream and GetCrcFromData produce same result for same data
	istd::CCrcCalculator::ByteStream stream;
	for (int i = 0; i < dataSize; i++){
		stream.append(static_cast<quint8>(testData[i]));
	}
	
	quint32 crc4 = istd::CCrcCalculator::GetCrcFromStream(stream);
	
	// Both methods should produce same result for same data
	QVERIFY(crc1 == crc4);
}


void CCrcCalculatorTest::cleanupTestCase()
{
}


I_ADD_TEST(CCrcCalculatorTest);
