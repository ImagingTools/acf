#include <icmm/Test/CIcmmTest.h>


// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <icmm/icmm.h>


void CIcmmTest::IsCmykTest()
{
	// Test CMYK colorants - should return true
	QVERIFY(icmm::IsCmyk(icmm::CU_CYAN));
	QVERIFY(icmm::IsCmyk(icmm::CU_DARK_CYAN));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_CYAN));
	QVERIFY(icmm::IsCmyk(icmm::CU_MAGENTA));
	QVERIFY(icmm::IsCmyk(icmm::CU_DARK_MAGENTA));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_MAGENTA));
	QVERIFY(icmm::IsCmyk(icmm::CU_YELLOW));
	QVERIFY(icmm::IsCmyk(icmm::CU_DARK_YELLOW));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_YELLOW));
	QVERIFY(icmm::IsCmyk(icmm::CU_BLACK));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_BLACK));
	QVERIFY(icmm::IsCmyk(icmm::CU_LIGHT_LIGHT_BLACK));

	// Test non-CMYK colorants - should return false
	QVERIFY(!icmm::IsCmyk(icmm::CU_NONE));
	QVERIFY(!icmm::IsCmyk(icmm::CU_ECG));
	QVERIFY(!icmm::IsCmyk(icmm::CU_SPOT));
}


I_ADD_TEST(CIcmmTest);


