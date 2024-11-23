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

	// Try to add incompatible model:
	icmm::CSubstractiveColorModel ecgColor2;
	ecgColor2.InsertColorant("Green", icmm::CU_ECG);
	ecgColor2.InsertColorant("Violet", icmm::CU_SPOT);
	QVERIFY(!model.AppendColorModel(ecgColor2));

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


void CSubstraactiveColorModelTest::CheckColorantsTest()
{
	icmm::CSubstractiveColorModel model;
	QVERIFY(!model.HasProcessColorants());
	QVERIFY(!model.HasEcg());
	QVERIFY(!model.HasSpot());

	model.InsertColorant("Cyan", icmm::CU_CYAN);
	model.InsertColorant("Magenta", icmm::CU_MAGENTA);
	model.InsertColorant("Yellow", icmm::CU_YELLOW);
	model.InsertColorant("Black", icmm::CU_BLACK);

	QVERIFY(model.ContainsColorant("Cyan"));
	QVERIFY(model.ContainsColorant("Magenta"));
	QVERIFY(model.ContainsColorant("Yellow"));
	QVERIFY(model.ContainsColorant("Black"));

	QVERIFY(model.HasProcessColorants());
	QVERIFY(!model.HasEcg());
	QVERIFY(!model.HasSpot());

	model.InsertColorant("Red", icmm::CU_ECG);
	QVERIFY(model.ContainsColorant("Red"));
	QVERIFY(model.HasProcessColorants());
	QVERIFY(model.HasEcg());
	QVERIFY(!model.HasSpot());

	model.InsertColorant("PANTONE", icmm::CU_SPOT);
	QVERIFY(model.ContainsColorant("PANTONE"));
	QVERIFY(model.HasProcessColorants());
	QVERIFY(model.HasEcg());
	QVERIFY(model.HasSpot());
}


void CSubstraactiveColorModelTest::RemoveColorantTest()
{
	icmm::CSubstractiveColorModel model;
	model.InsertColorant("Cyan", icmm::CU_CYAN);
	model.InsertColorant("Magenta", icmm::CU_MAGENTA);
	model.InsertColorant("Yellow", icmm::CU_YELLOW);
	model.InsertColorant("Black", icmm::CU_BLACK);

	model.RemoveColorant("Cyan");
	QVERIFY(!model.ContainsColorant("Cyan"));
}


void CSubstraactiveColorModelTest::EqualTest()
{
	icmm::CSubstractiveColorModel model;
	model.InsertColorant("Cyan", icmm::CU_CYAN);
	model.InsertColorant("Magenta", icmm::CU_MAGENTA);
	model.InsertColorant("Yellow", icmm::CU_YELLOW);
	model.InsertColorant("Black", icmm::CU_BLACK);

	icmm::CSubstractiveColorModel model2;
	model2.InsertColorant("Cyan", icmm::CU_CYAN);
	model2.InsertColorant("Magenta", icmm::CU_MAGENTA);
	model2.InsertColorant("Yellow", icmm::CU_YELLOW);
	model2.InsertColorant("Black", icmm::CU_BLACK);
	QVERIFY(model2 == model);

	model2.RemoveColorant("Black");
	QVERIFY(model2 != model);

	// Same colorant, but different usage:
	model2.InsertColorant("Black", icmm::CU_LIGHT_BLACK);
	QVERIFY(model2 != model);
}


I_ADD_TEST(CSubstraactiveColorModelTest);


