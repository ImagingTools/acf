#include <icmm/Test/CSubstraactiveColorModelTest.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CSubstractiveColorModel.h>


void CSubstraactiveColorModelTest::DoSerializeTest()
{
	icmm::CSubstractiveColorModel model;
	model.InsertColorant("Cyan", icmm::CU_CYAN);
	model.InsertColorant("Magenta", icmm::CU_MAGENTA);
	model.InsertColorant("Yellow", icmm::CU_YELLOW);
	model.InsertColorant("Black", icmm::CU_BLACK);

	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(model.Serialize(writeArchive));
	QByteArray data = writeArchive.GetData();

	icmm::CSubstractiveColorModel model2;

	iser::CJsonMemReadArchive readArchive(data);
	QVERIFY(model2.Serialize(readArchive));
	QVERIFY(model2.GetColorantIds() == model.GetColorantIds());
}


void CSubstraactiveColorModelTest::DoAppendModelTest()
{
	icmm::CSubstractiveColorModel model;
	model.InsertColorant("Cyan", icmm::CU_CYAN);
	model.InsertColorant("Magenta", icmm::CU_MAGENTA);
	model.InsertColorant("Yellow", icmm::CU_YELLOW);
	model.InsertColorant("Black", icmm::CU_BLACK);

	icmm::CSubstractiveColorModel ecgColors;
	ecgColors.InsertColorant("Orange", icmm::CU_ECG);
	ecgColors.InsertColorant("Violet", icmm::CU_ECG);

	QVERIFY(model.AppendColorModel(ecgColors));

	iser::CJsonMemWriteArchive writeArchive;
	QVERIFY(model.Serialize(writeArchive));
	QByteArray data = writeArchive.GetData();

	icmm::CSubstractiveColorModel targetModel;
	targetModel.InsertColorant("Cyan", icmm::CU_CYAN);
	targetModel.InsertColorant("Magenta", icmm::CU_MAGENTA);
	targetModel.InsertColorant("Yellow", icmm::CU_YELLOW);
	targetModel.InsertColorant("Black", icmm::CU_BLACK);
	targetModel.InsertColorant("Orange", icmm::CU_ECG);
	targetModel.InsertColorant("Violet", icmm::CU_ECG);

	iser::CJsonMemWriteArchive targetWriteArchive;
	QVERIFY(targetModel.Serialize(targetWriteArchive));

	QVERIFY(data == targetWriteArchive.GetData());
}


I_ADD_TEST(CSubstraactiveColorModelTest);


