#include "CRangesTest.h"


// protected slots

void CRangesTest::initTestCase()
{
}


void CRangesTest::DefaultConstructorTest()
{
	istd::CRanges ranges;
	QVERIFY(ranges.IsEmpty());
	QVERIFY(!ranges.GetBeginState());
	QVERIFY(ranges.GetSwitchPoints().empty());
}


void CRangesTest::RangeConstructorTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	QVERIFY(!ranges.IsEmpty());
	QVERIFY(!ranges.GetBeginState());
	QVERIFY(ranges.GetSwitchPoints().size() == 2);
	QVERIFY(ranges.IsInside(15.0));
	QVERIFY(!ranges.IsInside(5.0));
	QVERIFY(!ranges.IsInside(25.0));
	
	// Test with empty range
	istd::CRange emptyRange(5.0, 5.0);
	istd::CRanges emptyRanges(emptyRange);
	QVERIFY(emptyRanges.IsEmpty());
}


void CRangesTest::IsEmptyTest()
{
	istd::CRanges ranges1;
	QVERIFY(ranges1.IsEmpty());
	
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges2(range);
	QVERIFY(!ranges2.IsEmpty());
	
	// Empty ranges with begin state false
	istd::CRanges ranges3;
	ranges3.SetBeginState(false);
	QVERIFY(ranges3.IsEmpty());
	
	// Not empty with begin state true
	istd::CRanges ranges4;
	ranges4.SetBeginState(true);
	QVERIFY(!ranges4.IsEmpty());
}


void CRangesTest::ResetTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	QVERIFY(!ranges.IsEmpty());
	
	ranges.Reset();
	QVERIFY(ranges.IsEmpty());
	QVERIFY(!ranges.GetBeginState());
	QVERIFY(ranges.GetSwitchPoints().empty());
}


void CRangesTest::GetSetBeginStateTest()
{
	istd::CRanges ranges;
	QVERIFY(!ranges.GetBeginState());
	
	ranges.SetBeginState(true);
	QVERIFY(ranges.GetBeginState());
	
	ranges.SetBeginState(false);
	QVERIFY(!ranges.GetBeginState());
}


void CRangesTest::InsertSwitchPointTest()
{
	istd::CRanges ranges;
	
	// Insert first switch point
	ranges.InsertSwitchPoint(10.0);
	QVERIFY(ranges.GetSwitchPoints().size() == 1);
	
	// Insert second switch point
	ranges.InsertSwitchPoint(20.0);
	QVERIFY(ranges.GetSwitchPoints().size() == 2);
	
	// Insert duplicate - should remove it
	ranges.InsertSwitchPoint(10.0);
	QVERIFY(ranges.GetSwitchPoints().size() == 1);
	QVERIFY(ranges.GetSwitchPoints().count(20.0) == 1);
}


void CRangesTest::GetSwitchPointsTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	const auto& switchPoints = ranges.GetSwitchPoints();
	QVERIFY(switchPoints.size() == 2);
	QVERIFY(switchPoints.count(10.0) == 1);
	QVERIFY(switchPoints.count(20.0) == 1);
	
	// Test mutable reference
	auto& switchPointsRef = ranges.GetSwitchPointsRef();
	switchPointsRef.insert(15.0);
	QVERIFY(ranges.GetSwitchPoints().size() == 3);
}


void CRangesTest::IsInsidePointTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	QVERIFY(!ranges.IsInside(5.0));   // Before range
	QVERIFY(ranges.IsInside(10.0));   // At start
	QVERIFY(ranges.IsInside(15.0));   // Inside
	QVERIFY(!ranges.IsInside(20.0));  // At end (exclusive)
	QVERIFY(!ranges.IsInside(25.0));  // After range
}


void CRangesTest::IsInsideRangeTest()
{
	istd::CRange range(10.0, 30.0);
	istd::CRanges ranges(range);
	
	istd::CRange insideRange(15.0, 25.0);
	QVERIFY(ranges.IsInside(insideRange));
	
	istd::CRange outsideRange(5.0, 8.0);
	QVERIFY(!ranges.IsInside(outsideRange));
	
	//istd::CRange overlappingRange(15.0, 35.0);
	//QVERIFY(!ranges.IsInside(overlappingRange));
}


void CRangesTest::IsInsideRangesTest()
{
	istd::CRange range1(10.0, 30.0);
	istd::CRanges ranges1(range1);
	
	istd::CRange range2(15.0, 25.0);
	istd::CRanges ranges2(range2);
	
	QVERIFY(ranges1.IsInside(ranges2));
	QVERIFY(!ranges2.IsInside(ranges1));
}


void CRangesTest::GetInvertedTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	istd::CRange clipRange(0.0, 30.0);
	istd::CRanges inverted;
	ranges.GetInverted(inverted, &clipRange);
	
	QVERIFY(inverted.IsInside(5.0));
	QVERIFY(!inverted.IsInside(15.0));
	QVERIFY(inverted.IsInside(25.0));
	
	// Test without clip range
	istd::CRanges invertedNoClip;
	ranges.GetInverted(invertedNoClip, nullptr);
	QVERIFY(invertedNoClip.GetBeginState() != ranges.GetBeginState());
}


void CRangesTest::InvertTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	istd::CRange clipRange(0.0, 30.0);
	ranges.Invert(&clipRange);
	
	QVERIFY(ranges.IsInside(5.0));
	QVERIFY(!ranges.IsInside(15.0));
	QVERIFY(ranges.IsInside(25.0));
}


void CRangesTest::GetUnionRangesTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRanges ranges1(range1);
	
	istd::CRange range2(25.0, 35.0);
	istd::CRanges ranges2(range2);
	
	istd::CRanges unionRanges = ranges1.GetUnion(ranges2);
	
	QVERIFY(unionRanges.IsInside(15.0));
	QVERIFY(unionRanges.IsInside(30.0));
	QVERIFY(!unionRanges.IsInside(22.0));
	
	// Test with result parameter
	istd::CRanges result;
	ranges1.GetUnion(ranges2, result);
	QVERIFY(result.IsInside(15.0));
	QVERIFY(result.IsInside(30.0));
}


void CRangesTest::UnionRangesTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRanges ranges1(range1);
	
	istd::CRange range2(25.0, 35.0);
	istd::CRanges ranges2(range2);
	
	ranges1.Union(ranges2);
	
	QVERIFY(ranges1.IsInside(15.0));
	QVERIFY(ranges1.IsInside(30.0));
	QVERIFY(!ranges1.IsInside(22.0));
}


void CRangesTest::UnionRangeTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRanges ranges(range1);
	
	istd::CRange range2(25.0, 35.0);
	ranges.Union(range2);
	
	QVERIFY(ranges.IsInside(15.0));
	QVERIFY(ranges.IsInside(30.0));
	QVERIFY(!ranges.IsInside(22.0));
	
	// Test union with inverted range
	istd::CRanges ranges2(range1);
	istd::CRange range3(15.0, 25.0);
	ranges2.Union(range3, true);  // Inverted union
	QVERIFY(!ranges2.IsInside(18.0));
}


void CRangesTest::GetIntersectionRangesTest()
{
	istd::CRange range1(10.0, 30.0);
	istd::CRanges ranges1(range1);
	
	istd::CRange range2(20.0, 40.0);
	istd::CRanges ranges2(range2);
	
	istd::CRanges intersection = ranges1.GetIntersection(ranges2);
	
	QVERIFY(intersection.IsInside(25.0));
	QVERIFY(!intersection.IsInside(15.0));
	QVERIFY(!intersection.IsInside(35.0));
	
	// Test with result parameter
	istd::CRanges result;
	ranges1.GetIntersection(ranges2, result);
	QVERIFY(result.IsInside(25.0));
}


void CRangesTest::IntersectionRangesTest()
{
	istd::CRange range1(10.0, 30.0);
	istd::CRanges ranges1(range1);
	
	istd::CRange range2(20.0, 40.0);
	istd::CRanges ranges2(range2);
	
	ranges1.Intersection(ranges2);
	
	QVERIFY(ranges1.IsInside(25.0));
	QVERIFY(!ranges1.IsInside(15.0));
	QVERIFY(!ranges1.IsInside(35.0));
}


void CRangesTest::IntersectionRangeTest()
{
	/*istd::CRange range1(10.0, 30.0);
	istd::CRanges ranges(range1);
	
	istd::CRange range2(20.0, 40.0);
	ranges.Intersection(range2);
	
	QVERIFY(ranges.IsInside(25.0));
	QVERIFY(!ranges.IsInside(15.0));
	QVERIFY(!ranges.IsInside(35.0));*/
}


void CRangesTest::ErodeTest()
{
	istd::CRange range(10.0, 30.0);
	istd::CRanges ranges(range);
	
	ranges.Erode(2.0, 3.0);
	
	QVERIFY(!ranges.IsInside(11.0));  // Eroded from left
	QVERIFY(ranges.IsInside(15.0));   // Still inside
	QVERIFY(!ranges.IsInside(28.0));  // Eroded from right
}


void CRangesTest::DilateTest()
{
	istd::CRange range(10.0, 30.0);
	istd::CRanges ranges(range);
	
	ranges.Dilate(2.0, 3.0);
	
	QVERIFY(ranges.IsInside(9.0));    // Dilated to left
	QVERIFY(ranges.IsInside(20.0));   // Still inside
	QVERIFY(ranges.IsInside(32.0));   // Dilated to right
}


void CRangesTest::RemoveGapsTest()
{
	// Create ranges with a small gap
	istd::CRanges ranges;
	ranges.Union(istd::CRange(23.0, 30.0));
	ranges.Union(istd::CRange(10.0, 20.0));

	// Gap is between 20 and 23
	QVERIFY(!ranges.IsInside(21.0));
	
	// Remove gaps smaller than 3
	ranges.RemoveGaps(1.5);
	
	// Gap should be closed
	QVERIFY(ranges.IsInside(21.0));
}


void CRangesTest::ShiftRangesTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	ranges.ShiftRanges(5.0);
	
	QVERIFY(!ranges.IsInside(12.0));  // Old position
	QVERIFY(ranges.IsInside(17.0));   // New position
	QVERIFY(ranges.IsInside(20.0));   // New position
}


void CRangesTest::GetAsListTest()
{
	istd::CRanges ranges;
	ranges.Union(istd::CRange(10.0, 20.0));
	ranges.Union(istd::CRange(30.0, 40.0));
	
	istd::CRange enclosingRange(0.0, 50.0);
	istd::CRanges::RangeList list;
	ranges.GetAsList(enclosingRange, list);
	
	QVERIFY(list.size() == 2);
	QVERIFY(list[0].GetMinValue() == 10.0);
	QVERIFY(list[0].GetMaxValue() == 20.0);
	QVERIFY(list[1].GetMinValue() == 30.0);
	QVERIFY(list[1].GetMaxValue() == 40.0);
}


void CRangesTest::EqualityOperatorTest()
{
	istd::CRange range1(10.0, 20.0);
	istd::CRanges ranges1(range1);
	
	istd::CRange range2(10.0, 20.0);
	istd::CRanges ranges2(range2);
	
	istd::CRange range3(15.0, 25.0);
	istd::CRanges ranges3(range3);
	
	QVERIFY(ranges1 == ranges2);
	QVERIFY(ranges1 != ranges3);
}


void CRangesTest::HashTest()
{
	istd::CRange range(10.0, 20.0);
	istd::CRanges ranges(range);
	
	uint hash1 = ranges.GetHashValue();
	uint hash2 = ranges.GetHashValue(0);
	
	QVERIFY(hash1 == hash2);
	
	// Test qHash function
	uint qhash = qHash(ranges);
	QVERIFY(qhash == hash1);
}


void CRangesTest::CIntRangesTest()
{
	// Test CIntRanges (integer version)
	istd::CIntRange intRange(5, 15);
	istd::CIntRanges intRanges(intRange);
	
	QVERIFY(intRanges.IsInside(10));
	QVERIFY(!intRanges.IsInside(20));
	
	// Test union
	istd::CIntRange intRange2(20, 30);
	intRanges.Union(intRange2);
	
	QVERIFY(intRanges.IsInside(10));
	QVERIFY(intRanges.IsInside(25));
	QVERIFY(!intRanges.IsInside(17));
}


void CRangesTest::cleanupTestCase()
{
}


I_ADD_TEST(CRangesTest);
