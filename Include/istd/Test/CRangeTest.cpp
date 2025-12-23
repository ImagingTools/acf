#include "CRangeTest.h"


// protected slots

void CRangeTest::initTestCase()
{
}


void CRangeTest::ConstructorTest()
{
	// Test default constructor
	istd::CRange range1;
	QVERIFY(range1.GetMinValue() == 0.0);
	QVERIFY(range1.GetMaxValue() == 0.0);
	QVERIFY(range1.IsEmpty());
	
	// Test constructor with min/max values
	istd::CRange range2(10.0, 20.0);
	QVERIFY(range2.GetMinValue() == 10.0);
	QVERIFY(range2.GetMaxValue() == 20.0);
	QVERIFY(!range2.IsEmpty());
	QVERIFY(range2.IsValid());
}


void CRangeTest::CopyConstructorTest()
{
	istd::CRange range1(5.0, 15.0);
	istd::CRange range2(range1);
	
	QVERIFY(range2.GetMinValue() == 5.0);
	QVERIFY(range2.GetMaxValue() == 15.0);
	QVERIFY(range1 == range2);
}


void CRangeTest::IsValidTest()
{
	istd::CRange validRange(10.0, 20.0);
	QVERIFY(validRange.IsValid());
	
	istd::CRange invalidRange(20.0, 10.0);
	QVERIFY(!invalidRange.IsValid());
	
	istd::CRange emptyRange(15.0, 15.0);
	QVERIFY(emptyRange.IsValid());
}


void CRangeTest::IsEmptyTest()
{
	istd::CRange emptyRange1;
	QVERIFY(emptyRange1.IsEmpty());
	
	istd::CRange emptyRange2(5.0, 5.0);
	QVERIFY(emptyRange2.IsEmpty());
	
	istd::CRange nonEmptyRange(5.0, 10.0);
	QVERIFY(!nonEmptyRange.IsEmpty());
}


void CRangeTest::IsValidNonEmptyTest()
{
	istd::CRange validNonEmpty(5.0, 10.0);
	QVERIFY(validNonEmpty.IsValidNonEmpty());
	
	istd::CRange empty(5.0, 5.0);
	QVERIFY(!empty.IsValidNonEmpty());
	
	istd::CRange invalid(10.0, 5.0);
	QVERIFY(!invalid.IsValidNonEmpty());
}


void CRangeTest::GetSetMinMaxTest()
{
	istd::CRange range;
	
	range.SetMinValue(10.0);
	QVERIFY(range.GetMinValue() == 10.0);
	
	range.SetMaxValue(20.0);
	QVERIFY(range.GetMaxValue() == 20.0);
	
	// Test reference getters
	range.GetMinValueRef() = 15.0;
	QVERIFY(range.GetMinValue() == 15.0);
	
	range.GetMaxValueRef() = 25.0;
	QVERIFY(range.GetMaxValue() == 25.0);
}


void CRangeTest::GetLengthTest()
{
	istd::CRange range(10.0, 30.0);
	QVERIFY(range.GetLength() == 20.0);
	
	istd::CRange emptyRange(5.0, 5.0);
	QVERIFY(emptyRange.GetLength() == 0.0);
	
	istd::CRange invalidRange(30.0, 10.0);
	QVERIFY(invalidRange.GetLength() == 0.0);
}


void CRangeTest::GetAbsMinMaxValueTest()
{
	istd::CRange range1(-5.0, 10.0);
	QVERIFY(range1.GetAbsMinValue() == 5.0);
	QVERIFY(range1.GetAbsMaxValue() == 10.0);
	
	istd::CRange range2(-15.0, -5.0);
	QVERIFY(range2.GetAbsMinValue() == 5.0);
	QVERIFY(range2.GetAbsMaxValue() == 15.0);
	
	istd::CRange range3(5.0, 15.0);
	QVERIFY(range3.GetAbsMinValue() == 5.0);
	QVERIFY(range3.GetAbsMaxValue() == 15.0);
}


void CRangeTest::GetValidatedTest()
{
	istd::CRange invalidRange(20.0, 10.0);
	istd::CRange validated = invalidRange.GetValidated();
	
	QVERIFY(validated.GetMinValue() == 10.0);
	QVERIFY(validated.GetMaxValue() == 20.0);
	QVERIFY(validated.IsValid());
	
	// Test with result parameter
	istd::CRange result;
	invalidRange.GetValidated(result);
	QVERIFY(result.GetMinValue() == 10.0);
	QVERIFY(result.GetMaxValue() == 20.0);
}


void CRangeTest::ValidateTest()
{
	istd::CRange invalidRange(20.0, 10.0);
	invalidRange.Validate();
	
	QVERIFY(invalidRange.GetMinValue() == 10.0);
	QVERIFY(invalidRange.GetMaxValue() == 20.0);
	QVERIFY(invalidRange.IsValid());
}


void CRangeTest::ContainsValueTest()
{
	istd::CRange range(10.0, 20.0);
	
	QVERIFY(range.Contains(10.0));
	QVERIFY(range.Contains(15.0));
	QVERIFY(range.Contains(20.0));
	QVERIFY(!range.Contains(5.0));
	QVERIFY(!range.Contains(25.0));
}


void CRangeTest::ContainsRangeTest()
{
	istd::CRange range(10.0, 30.0);
	istd::CRange innerRange(15.0, 25.0);
	istd::CRange outerRange(5.0, 35.0);
	istd::CRange overlappingRange(20.0, 40.0);
	
	QVERIFY(range.Contains(innerRange));
	QVERIFY(!range.Contains(outerRange));
	QVERIFY(!range.Contains(overlappingRange));
}


void CRangeTest::IsIntersectedByTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange intersecting1(15.0, 25.0);
	istd::CRange intersecting2(5.0, 15.0);
	istd::CRange nonIntersecting1(0.0, 5.0);
	istd::CRange nonIntersecting2(25.0, 30.0);
	
	QVERIFY(range.IsIntersectedBy(intersecting1));
	QVERIFY(range.IsIntersectedBy(intersecting2));
	QVERIFY(!range.IsIntersectedBy(nonIntersecting1));
	QVERIFY(!range.IsIntersectedBy(nonIntersecting2));
}


void CRangeTest::IsOutsideOfTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange outside1(0.0, 5.0);
	istd::CRange outside2(25.0, 30.0);
	istd::CRange inside(12.0, 18.0);
	istd::CRange overlapping(15.0, 25.0);
	
	QVERIFY(range.IsOutsideOf(outside1));
	QVERIFY(range.IsOutsideOf(outside2));
	QVERIFY(!range.IsOutsideOf(inside));
	QVERIFY(!range.IsOutsideOf(overlapping));
}


void CRangeTest::GetIntersectionTest()
{
	istd::CRange range1(10.0, 30.0);
	istd::CRange range2(20.0, 40.0);
	istd::CRange intersection = range1.GetIntersection(range2);
	
	QVERIFY(intersection.GetMinValue() == 20.0);
	QVERIFY(intersection.GetMaxValue() == 30.0);
	
	// Test with non-intersecting ranges
	istd::CRange range3(50.0, 60.0);
	istd::CRange noIntersection = range1.GetIntersection(range3);
	QVERIFY(!noIntersection.IsValid());
}


void CRangeTest::IntersectionTest()
{
	istd::CRange range1(10.0, 30.0);
	istd::CRange range2(20.0, 40.0);
	range1.Intersection(range2);
	
	QVERIFY(range1.GetMinValue() == 20.0);
	QVERIFY(range1.GetMaxValue() == 30.0);
}


void CRangeTest::GetUnionTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRange range2(15.0, 30.0);
	istd::CRange unionRange = range1.GetUnion(range2);
	
	QVERIFY(unionRange.GetMinValue() == 10.0);
	QVERIFY(unionRange.GetMaxValue() == 30.0);
	
	// Test union with value
	istd::CRange range3(10.0, 20.0);
	istd::CRange unionWithValue = range3.GetUnion(25.0);
	QVERIFY(unionWithValue.GetMinValue() == 10.0);
	QVERIFY(unionWithValue.GetMaxValue() == 25.0);
}


void CRangeTest::UniteTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRange range2(15.0, 30.0);
	range1.Unite(range2);
	
	QVERIFY(range1.GetMinValue() == 10.0);
	QVERIFY(range1.GetMaxValue() == 30.0);
	
	// Test unite with value
	istd::CRange range3(10.0, 20.0);
	range3.Unite(25.0);
	QVERIFY(range3.GetMinValue() == 10.0);
	QVERIFY(range3.GetMaxValue() == 25.0);
}


void CRangeTest::GetTranslatedTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange translated = range.GetTranslated(5.0);
	
	QVERIFY(translated.GetMinValue() == 15.0);
	QVERIFY(translated.GetMaxValue() == 25.0);
	
	// Original should be unchanged
	QVERIFY(range.GetMinValue() == 10.0);
	QVERIFY(range.GetMaxValue() == 20.0);
}


void CRangeTest::TranslateTest()
{
	istd::CRange range(10.0, 20.0);
	range.Translate(5.0);
	
	QVERIFY(range.GetMinValue() == 15.0);
	QVERIFY(range.GetMaxValue() == 25.0);
}


void CRangeTest::GetExpandedTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRange range2(-2.0, 3.0);
	istd::CRange expanded = range1.GetExpanded(range2);
	
	QVERIFY(expanded.GetMinValue() == 8.0);
	QVERIFY(expanded.GetMaxValue() == 23.0);
}


void CRangeTest::ExpandTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRange range2(-2.0, 3.0);
	range1.Expand(range2);
	
	QVERIFY(range1.GetMinValue() == 8.0);
	QVERIFY(range1.GetMaxValue() == 23.0);
}


void CRangeTest::GetDistanceTest()
{
	istd::CRange range(10.0, 20.0);
	
	QVERIFY(range.GetDistance(15.0) == 0.0);  // Inside
	QVERIFY(range.GetDistance(5.0) == 5.0);   // Below
	QVERIFY(range.GetDistance(25.0) == 5.0);  // Above
}


void CRangeTest::GetNearestInsideTest()
{
	istd::CRange range(10.0, 20.0);
	
	QVERIFY(range.GetNearestInside(15.0) == 15.0);  // Inside
	QVERIFY(range.GetNearestInside(5.0) == 10.0);   // Below
	QVERIFY(range.GetNearestInside(25.0) == 20.0);  // Above
}


void CRangeTest::GetClippedValueTest()
{
	istd::CRange range(10.0, 20.0);
	
	QVERIFY(range.GetClipped(15.0) == 15.0);  // Inside
	QVERIFY(range.GetClipped(5.0) == 10.0);   // Closer to min
	QVERIFY(range.GetClipped(25.0) == 20.0);  // Closer to max
}


void CRangeTest::GetClippedRangeTest()
{
	istd::CRange range(10.0, 30.0);
	istd::CRange toClip(5.0, 35.0);
	istd::CRange clipped = range.GetClipped(toClip);
	
	QVERIFY(clipped.GetMinValue() == 10.0);
	QVERIFY(clipped.GetMaxValue() == 30.0);
}


void CRangeTest::GetValueFromAlphaTest()
{
	istd::CRange range(10.0, 20.0);
	
	QVERIFY(range.GetValueFromAlpha(0.0) == 10.0);
	QVERIFY(range.GetValueFromAlpha(0.5) == 15.0);
	QVERIFY(range.GetValueFromAlpha(1.0) == 20.0);
}


void CRangeTest::GetAlphaFromValueTest()
{
	istd::CRange range(10.0, 20.0);
	
	QVERIFY(range.GetAlphaFromValue(10.0) == 0.0);
	QVERIFY(range.GetAlphaFromValue(15.0) == 0.5);
	QVERIFY(range.GetAlphaFromValue(20.0) == 1.0);
}


void CRangeTest::GetMappedToTest()
{
	istd::CRange range1(0.0, 100.0);
	istd::CRange range2(0.0, 10.0);
	
	QVERIFY(range1.GetMappedTo(0.0, range2) == 0.0);
	QVERIFY(range1.GetMappedTo(50.0, range2) == 5.0);
	QVERIFY(range1.GetMappedTo(100.0, range2) == 10.0);
}


void CRangeTest::GetApplyTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange alpha(0.0, 1.0);
	istd::CRange applied = range.GetApply(alpha);
	
	QVERIFY(applied.GetMinValue() == 10.0);
	QVERIFY(applied.GetMaxValue() == 20.0);
}


void CRangeTest::GetInvertApplyTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange values(10.0, 20.0);
	istd::CRange inverted = range.GetInvertApply(values);
	
	QVERIFY(qAbs(inverted.GetMinValue() - 0.0) < 0.001);
	QVERIFY(qAbs(inverted.GetMaxValue() - 1.0) < 0.001);
}


void CRangeTest::GetInvertedTest()
{
	istd::CRange range(0.5, 1.0);
	istd::CRange inverted = range.GetInverted();
	
	double length = 2.0;
	QVERIFY(inverted.GetLength() == length);
}


void CRangeTest::SetInterpolatedTest()
{
	istd::CRange range1(0.0, 10.0);
	istd::CRange range2(20.0, 30.0);
	istd::CRange result;
	
	result.SetInterpolated(range1, range2, 0.5);
	QVERIFY(result.GetMinValue() == 10.0);
	QVERIFY(result.GetMaxValue() == 20.0);
}


void CRangeTest::EqualityOperatorTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRange range2(10.0, 20.0);
	istd::CRange range3(15.0, 25.0);
	
	QVERIFY(range1 == range2);
	QVERIFY(range1 != range3);
}


void CRangeTest::AssignmentOperatorTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRange range2;
	
	range2 = range1;
	QVERIFY(range2.GetMinValue() == 10.0);
	QVERIFY(range2.GetMaxValue() == 20.0);
	QVERIFY(range1 == range2);
}


void CRangeTest::MultiplicationOperatorTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange multiplied = range * 2.0;
	
	QVERIFY(multiplied.GetMinValue() == 20.0);
	QVERIFY(multiplied.GetMaxValue() == 40.0);
	
	// Test *= operator
	istd::CRange range2(10.0, 20.0);
	range2 *= 2.0;
	QVERIFY(range2.GetMinValue() == 20.0);
	QVERIFY(range2.GetMaxValue() == 40.0);
}


void CRangeTest::DivisionOperatorTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRange divided = range / 2.0;
	
	QVERIFY(divided.GetMinValue() == 5.0);
	QVERIFY(divided.GetMaxValue() == 10.0);
	
	// Test /= operator
	istd::CRange range2(10.0, 20.0);
	range2 /= 2.0;
	QVERIFY(range2.GetMinValue() == 5.0);
	QVERIFY(range2.GetMaxValue() == 10.0);
	
	// Test division by zero
	istd::CRange range3(10.0, 20.0);
	istd::CRange divided2 = range3 / 0.0;
	QVERIFY(divided2.GetMinValue() == 0.0);
	QVERIFY(divided2.GetMaxValue() == 0.0);
}


void CRangeTest::GetNullTest()
{
	const istd::CRange& nullRange = istd::CRange::GetNull();
	QVERIFY(nullRange.GetMinValue() == 0.0);
	QVERIFY(nullRange.GetMaxValue() == 0.0);
	QVERIFY(nullRange.IsEmpty());
}


void CRangeTest::GetInvalidTest()
{
	const istd::CRange& invalidRange = istd::CRange::GetInvalid();
	QVERIFY(!invalidRange.IsValid());
}


void CRangeTest::GetValidTest()
{
	istd::CRange range = istd::CRange::GetValid(20.0, 10.0);
	QVERIFY(range.GetMinValue() == 10.0);
	QVERIFY(range.GetMaxValue() == 20.0);
	QVERIFY(range.IsValid());
}


void CRangeTest::ResetTest()
{
	istd::CRange range(10.0, 20.0);
	range.Reset();
	
	QVERIFY(range.GetMinValue() == 0.0);
	QVERIFY(range.GetMaxValue() == 0.0);
	QVERIFY(range.IsEmpty());
}


void CRangeTest::CIntRangeTest()
{
	// Test CIntRange (integer version)
	istd::CIntRange intRange(5, 15);
	QVERIFY(intRange.GetMinValue() == 5);
	QVERIFY(intRange.GetMaxValue() == 15);
	QVERIFY(intRange.GetLength() == 10);
	QVERIFY(intRange.Contains(10));
	QVERIFY(!intRange.Contains(20));
	
	// Test translation
	intRange.Translate(5);
	QVERIFY(intRange.GetMinValue() == 10);
	QVERIFY(intRange.GetMaxValue() == 20);
}


void CRangeTest::cleanupTestCase()
{
}


I_ADD_TEST(CRangeTest);
