// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/Test/CIlluminantTest.h>


// ACF includes
#include <iser/CCompactXmlMemReadArchive.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/CJsonMemWriteArchive.h>
#include <itest/CStandardTestExecutor.h>
#include <icmm/CIlluminant.h>
#include <icmm/CVarColor.h>


namespace
{

//! Relative radiant power of CIE illuminant D65, sampled from 380nm to 730nm in 10nm steps.
icmm::CSpectrum MakeD65Spd()
{
	return icmm::CSpectrum(
			380,
			730,
			{49.98, 54.65, 82.75, 91.49, 93.43, 86.68, 104.87, 117.01,
			 117.81, 114.86, 115.92, 108.81, 109.35, 107.80, 104.79, 107.69,
			 104.41, 104.05, 100.00, 96.33, 95.79, 88.69, 90.01, 89.60,
			 87.70, 83.29, 83.70, 80.03, 80.21, 82.28, 78.28, 69.72,
			 71.61, 74.35, 61.60, 69.89});
}


//! Relative radiant power of CIE illuminant A, on the same grid as MakeD65Spd().
icmm::CSpectrum MakeIlluminantASpd()
{
	return icmm::CSpectrum(
			380,
			730,
			{9.80, 12.09, 14.71, 17.68, 20.99, 24.67, 28.70, 33.09,
			 37.81, 42.87, 48.24, 53.91, 59.86, 66.06, 72.50, 79.13,
			 85.95, 92.91, 100.00, 107.18, 114.44, 121.73, 129.04, 136.35,
			 143.62, 150.84, 157.98, 165.03, 171.96, 178.77, 185.43, 191.93,
			 198.26, 204.41, 210.36, 216.12});
}

}


void CIlluminantTest::StandardIlluminantConstructorTest()
{
	// Default constructor creates a D50 illuminant
	icmm::CIlluminant defaultIlluminant;
	QCOMPARE(defaultIlluminant.GetIlluminantType(), icmm::StandardIlluminant::D50);
	QCOMPARE(defaultIlluminant.GetIlluminantName(), QString("D50"));

	// The illuminant name is derived from the standard type
	struct
	{
		icmm::StandardIlluminant type;
		QString name;
	} testCases[] = {
		{icmm::StandardIlluminant::A, "A"},
		{icmm::StandardIlluminant::B, "B"},
		{icmm::StandardIlluminant::C, "C"},
		{icmm::StandardIlluminant::D50, "D50"},
		{icmm::StandardIlluminant::D55, "D55"},
		{icmm::StandardIlluminant::D60, "D60"},
		{icmm::StandardIlluminant::D65, "D65"},
		{icmm::StandardIlluminant::D75, "D75"},
		{icmm::StandardIlluminant::E, "E"}};

	for (const auto& testCase: testCases){
		icmm::CIlluminant illuminant(testCase.type);
		QCOMPARE(illuminant.GetIlluminantType(), testCase.type);
		QCOMPARE(illuminant.GetIlluminantName(), testCase.name);
	}
}


void CIlluminantTest::CustomIlluminantConstructorTest()
{
	icmm::CSpectrum spd = MakeD65Spd();

	icmm::CIlluminant illuminant("MyIlluminant", spd);
	QCOMPARE(illuminant.GetIlluminantType(), icmm::StandardIlluminant::Custom);
	QCOMPARE(illuminant.GetIlluminantName(), QString("MyIlluminant"));
	QVERIFY(illuminant.GetSpectralPowerDistribution().IsEqual(spd));
	QCOMPARE(illuminant.GetSpectralPowerDistribution().GetSamplesCount(), 36);
	QCOMPARE(illuminant.GetSpectralPowerDistribution().GetSpectralRange(), istd::CIntRange(380, 730));
	QCOMPARE(illuminant.GetSpectralPowerDistribution().GetStep(), 10);

	// Custom illuminant with an empty name and empty spectrum
	icmm::CIlluminant emptyIlluminant{QString(), icmm::CSpectrum()};
	QCOMPARE(emptyIlluminant.GetIlluminantType(), icmm::StandardIlluminant::Custom);
	QVERIFY(emptyIlluminant.GetIlluminantName().isEmpty());
	QCOMPARE(emptyIlluminant.GetSpectralPowerDistribution().GetSamplesCount(), 0);
}


void CIlluminantTest::CopyConstructorTest()
{
	icmm::CIlluminant source("Source", MakeD65Spd());

	icmm::CIlluminant copy(source);
	QCOMPARE(copy.GetIlluminantType(), source.GetIlluminantType());
	QCOMPARE(copy.GetIlluminantName(), source.GetIlluminantName());
	QVERIFY(copy.GetSpectralPowerDistribution().IsEqual(source.GetSpectralPowerDistribution()));
	QVERIFY(copy.IsEqual(source));
}


void CIlluminantTest::SettersTest()
{
	icmm::CIlluminant illuminant;

	illuminant.SetIlluminantType(icmm::StandardIlluminant::D65);
	QCOMPARE(illuminant.GetIlluminantType(), icmm::StandardIlluminant::D65);

	illuminant.SetIlluminantName("NewName");
	QCOMPARE(illuminant.GetIlluminantName(), QString("NewName"));

	icmm::CSpectrum spd = MakeD65Spd();
	illuminant.SetSpectralPowerDistribution(spd);
	QVERIFY(illuminant.GetSpectralPowerDistribution().IsEqual(spd));

	icmm::CSpectrum otherSpd = MakeIlluminantASpd();
	illuminant.SetSpectralPowerDistribution(otherSpd);
	QVERIFY(illuminant.GetSpectralPowerDistribution().IsEqual(otherSpd));
}


void CIlluminantTest::FactoryIdTest()
{
	icmm::CIlluminant illuminant;
	QCOMPARE(illuminant.GetFactoryId(), icmm::CIlluminant::GetTypeId());
	QVERIFY(!icmm::CIlluminant::GetTypeId().isEmpty());
}


void CIlluminantTest::SerializeTest()
{
	// Standard illuminant round trip
	{
		icmm::CIlluminant source(icmm::StandardIlluminant::D65);

		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(source.Serialize(writeArchive));

		icmm::CIlluminant restored(icmm::StandardIlluminant::A);
		iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
		QVERIFY(restored.Serialize(readArchive));

		QVERIFY(restored.IsEqual(source));
		QCOMPARE(restored.GetIlluminantType(), icmm::StandardIlluminant::D65);
		QCOMPARE(restored.GetIlluminantName(), QString("D65"));
	}

	// Custom illuminant with spectral power distribution round trip
	{
		icmm::CIlluminant source("Custom Illuminant", MakeD65Spd());

		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(source.Serialize(writeArchive));

		icmm::CIlluminant restored;
		iser::CJsonMemReadArchive readArchive(writeArchive.GetData());
		QVERIFY(restored.Serialize(readArchive));

		QVERIFY(restored.IsEqual(source));
		QCOMPARE(restored.GetIlluminantType(), icmm::StandardIlluminant::Custom);
		QCOMPARE(restored.GetIlluminantName(), QString("Custom Illuminant"));
		QVERIFY(restored.GetSpectralPowerDistribution().IsEqual(source.GetSpectralPowerDistribution()));
	}
}


void CIlluminantTest::SerializeLegacyWhitePointTest()
{
	// Data written before Acf commit 1f0b426b3 
	// ("icmm: replace illuminant white point by spectral power distribution", revision 6526) 
	// stored not used "WhitePoint" tag. Loading it should succeed and discard the white point,
	// new "SpectralPowerDistribution" field remains empty.
	static const QByteArray legacyData = R"(<?xml version="1.0" encoding="UTF-8"?>
<Acf>
    <AcfHeader>
        <VersionInfos>
            <Version Id="0" Number="6525" Description="ACF"/>
        </VersionInfos>
    </AcfHeader>
    <IlluminantType>D50</IlluminantType>
    <IlluminantName>D50</IlluminantName>
    <WhitePoint>
        <ColorComponents/>
    </WhitePoint>
</Acf>)";

	iser::CCompactXmlMemReadArchive readArchive(legacyData);
	icmm::CIlluminant illuminant;
	QVERIFY(illuminant.Serialize(readArchive));
	QCOMPARE(illuminant.GetIlluminantType(), icmm::StandardIlluminant::D50);
	QCOMPARE(illuminant.GetIlluminantName(), QString("D50"));
	QCOMPARE(illuminant.GetSpectralPowerDistribution().GetSamplesCount(), 0);
}


void CIlluminantTest::IsEqualTest()
{
	icmm::CIlluminant illuminant1(icmm::StandardIlluminant::D50);
	icmm::CIlluminant illuminant2(icmm::StandardIlluminant::D50);
	QVERIFY(illuminant1.IsEqual(illuminant2));
	QVERIFY(illuminant1.IsEqual(illuminant1));

	// Different type
	icmm::CIlluminant differentType(icmm::StandardIlluminant::D65);
	QVERIFY(!illuminant1.IsEqual(differentType));

	// Different name
	icmm::CIlluminant differentName(icmm::StandardIlluminant::D50);
	differentName.SetIlluminantName("Other");
	QVERIFY(!illuminant1.IsEqual(differentName));

	// Spectral power distribution set against none
	icmm::CIlluminant withD65Spd(icmm::StandardIlluminant::D50);
	withD65Spd.SetSpectralPowerDistribution(MakeD65Spd());
	QVERIFY(!illuminant1.IsEqual(withD65Spd));

	// Different values on the same wavelength grid
	icmm::CIlluminant withASpd(icmm::StandardIlluminant::D50);
	withASpd.SetSpectralPowerDistribution(MakeIlluminantASpd());
	QVERIFY(!withD65Spd.IsEqual(withASpd));

	// Comparison with an incompatible object type
	icmm::CVarColor otherObject(3);
	QVERIFY(!illuminant1.IsEqual(otherObject));
}


void CIlluminantTest::CopyFromTest()
{
	icmm::CIlluminant source("Source", MakeD65Spd());

	icmm::CIlluminant target;
	QVERIFY(target.CopyFrom(source));
	QVERIFY(target.IsEqual(source));

	// Copying replaces an already set distribution
	icmm::CIlluminant other("Other", MakeIlluminantASpd());
	QVERIFY(target.CopyFrom(other));
	QVERIFY(target.IsEqual(other));

	// Copying from an incompatible object fails
	icmm::CVarColor otherObject(3);
	QVERIFY(!target.CopyFrom(otherObject));
}


void CIlluminantTest::CloneMeTest()
{
	icmm::CIlluminant source("Source", MakeD65Spd());

	istd::IChangeableUniquePtr clonePtr = source.CloneMe();
	QVERIFY(clonePtr.GetPtr() != nullptr);
	QVERIFY(clonePtr->IsEqual(source));

	// The clone is an independent instance
	icmm::CIlluminant* clonedIlluminantPtr = dynamic_cast<icmm::CIlluminant*>(clonePtr.GetPtr());
	QVERIFY(clonedIlluminantPtr != nullptr);
	QVERIFY(clonedIlluminantPtr != &source);

	clonedIlluminantPtr->SetIlluminantName("Changed");
	QCOMPARE(source.GetIlluminantName(), QString("Source"));
}


I_ADD_TEST(CIlluminantTest);
