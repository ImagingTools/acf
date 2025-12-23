#ifndef i2d_CGraphData2dTest_included
#define i2d_CGraphData2dTest_included


#include <QtCore/QObject>


namespace i2d
{


class CGraphData2dTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();
	void testConstruction();
	void testAddCurve();
	void testRemoveCurve();
	void testClearCurves();
	void testAxisLabels();
	void testAxisRanges();
	void testLegendVisibility();
	void testGridVisibility();
	void testBoundingBox();
	void cleanupTestCase();
};


} // namespace i2d


#endif // !i2d_CGraphData2dTest_included
