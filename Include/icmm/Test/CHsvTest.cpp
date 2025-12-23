#include "CHsvTest.h"


// protected slots

void CHsvTest::initTestCase()
{
}


void CHsvTest::ConstructorTest()
{
	// Test default constructor
	icmm::CHsv color1;
	QVERIFY(qAbs(color1.GetHue()) < 0.001);
	QVERIFY(qAbs(color1.GetSaturation()) < 0.001);
	QVERIFY(qAbs(color1.GetValue()) < 0.001);
	
	// Test constructor with values
	icmm::CHsv color2(0.5, 0.8, 1.0);
	QVERIFY(qAbs(color2.GetHue() - 0.5) < 0.001);
	QVERIFY(qAbs(color2.GetSaturation() - 0.8) < 0.001);
	QVERIFY(qAbs(color2.GetValue() - 1.0) < 0.001);
	
	// Test copy constructor
	icmm::CHsv color3(color2);
	QVERIFY(qAbs(color3.GetHue() - 0.5) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 0.8) < 0.001);
	QVERIFY(qAbs(color3.GetValue() - 1.0) < 0.001);
}


void CHsvTest::GetSetTest()
{
	icmm::CHsv color;
	
	// Test SetHue and GetHue
	color.SetHue(0.3);
	QVERIFY(qAbs(color.GetHue() - 0.3) < 0.001);
	
	// Test SetSaturation and GetSaturation
	color.SetSaturation(0.6);
	QVERIFY(qAbs(color.GetSaturation() - 0.6) < 0.001);
	
	// Test SetValue and GetValue
	color.SetValue(0.9);
	QVERIFY(qAbs(color.GetValue() - 0.9) < 0.001);
}


void CHsvTest::ArithmeticOperatorsTest()
{
	icmm::CHsv color1(0.5, 0.6, 0.7);
	icmm::CHsv color2(0.1, 0.2, 0.3);
	
	// Test addition
	icmm::CHsv sum = color1 + color2;
	QVERIFY(qAbs(sum.GetHue() - 0.6) < 0.001);
	QVERIFY(qAbs(sum.GetSaturation() - 0.8) < 0.001);
	QVERIFY(qAbs(sum.GetValue() - 1.0) < 0.001);
	
	// Test subtraction
	icmm::CHsv diff = color1 - color2;
	QVERIFY(qAbs(diff.GetHue() - 0.4) < 0.001);
	QVERIFY(qAbs(diff.GetSaturation() - 0.4) < 0.001);
	QVERIFY(qAbs(diff.GetValue() - 0.4) < 0.001);
	
	// Test multiplication
	icmm::CHsv prod = color1 * color2;
	QVERIFY(qAbs(prod.GetHue() - 0.05) < 0.001);
	QVERIFY(qAbs(prod.GetSaturation() - 0.12) < 0.001);
	QVERIFY(qAbs(prod.GetValue() - 0.21) < 0.001);
	
	// Test division
	icmm::CHsv quot = color1 / color2;
	QVERIFY(qAbs(quot.GetHue() - 5.0) < 0.001);
	QVERIFY(qAbs(quot.GetSaturation() - 3.0) < 0.001);
	QVERIFY(qAbs(quot.GetValue() - 2.333) < 0.01);
	
	// Test scalar multiplication
	icmm::CHsv scaled = color1 * 2.0;
	QVERIFY(qAbs(scaled.GetHue() - 1.0) < 0.001);
	QVERIFY(qAbs(scaled.GetSaturation() - 1.2) < 0.001);
	QVERIFY(qAbs(scaled.GetValue() - 1.4) < 0.001);
	
	// Test scalar division
	icmm::CHsv divided = color1 / 2.0;
	QVERIFY(qAbs(divided.GetHue() - 0.25) < 0.001);
	QVERIFY(qAbs(divided.GetSaturation() - 0.3) < 0.001);
	QVERIFY(qAbs(divided.GetValue() - 0.35) < 0.001);
}


void CHsvTest::CompoundOperatorsTest()
{
	icmm::CHsv color1(0.5, 0.6, 0.7);
	icmm::CHsv color2(0.1, 0.2, 0.3);
	
	// Test +=
	icmm::CHsv color3 = color1;
	color3 += color2;
	QVERIFY(qAbs(color3.GetHue() - 0.6) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 0.8) < 0.001);
	QVERIFY(qAbs(color3.GetValue() - 1.0) < 0.001);
	
	// Test -=
	color3 = color1;
	color3 -= color2;
	QVERIFY(qAbs(color3.GetHue() - 0.4) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 0.4) < 0.001);
	QVERIFY(qAbs(color3.GetValue() - 0.4) < 0.001);
	
	// Test *=
	color3 = color1;
	color3 *= color2;
	QVERIFY(qAbs(color3.GetHue() - 0.05) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 0.12) < 0.001);
	QVERIFY(qAbs(color3.GetValue() - 0.21) < 0.001);
	
	// Test /=
	color3 = color1;
	color3 /= color2;
	QVERIFY(qAbs(color3.GetHue() - 5.0) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 3.0) < 0.001);
	
	// Test *= with scalar
	color3 = color1;
	color3 *= 2.0;
	QVERIFY(qAbs(color3.GetHue() - 1.0) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 1.2) < 0.001);
	QVERIFY(qAbs(color3.GetValue() - 1.4) < 0.001);
	
	// Test /= with scalar
	color3 = color1;
	color3 /= 2.0;
	QVERIFY(qAbs(color3.GetHue() - 0.25) < 0.001);
	QVERIFY(qAbs(color3.GetSaturation() - 0.3) < 0.001);
	QVERIFY(qAbs(color3.GetValue() - 0.35) < 0.001);
}


void CHsvTest::cleanupTestCase()
{
}


I_ADD_TEST(CHsvTest);
