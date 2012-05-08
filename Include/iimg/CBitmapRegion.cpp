#include "iimg/CBitmapRegion.h"


// Qt includes
#include <QtCore/qmath.h>


namespace iimg
{


// public methods

CBitmapRegion::CBitmapRegion(const iimg::IBitmap* bitmapPtr)
:	m_bitmapPtr(bitmapPtr),
	m_isEmpty(true)
{
}


bool CBitmapRegion::CreateFromGeometry(const i2d::IObject2d& geometry)
{
	m_isEmpty = true;

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(&geometry);
	if (annulusPtr != NULL){
		CreateFromAnnulus(*annulusPtr);

		return true;
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&geometry);
	if (circlePtr != NULL){
		CreateFromCircle(*circlePtr);

		return true;
	}

	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(&geometry);
	if (rectanglePtr != NULL){
		CreateFromRectangle(*rectanglePtr);

		return true;
	}

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(&geometry);
	if (polygonPtr != NULL){
		CreateFromPolygon(*polygonPtr);

		return true;
	}

	return false;
}


const CBitmapRegion::PixelRanges* CBitmapRegion::GetPixelRanges(int lineIndex) const
{
	int rangeIndex = lineIndex - int(m_boundingBox.GetTop());

	if (rangeIndex >= 0 && rangeIndex < int(m_lineRangePtr.size())){
		return m_lineRangePtr[rangeIndex];
	}

	return NULL;
}


const i2d::CRectangle& CBitmapRegion::GetBoundingBox() const
{
	return m_boundingBox;
}


void CBitmapRegion::ResetBitmapRegion()
{
	m_rangesContainer.clear();
	m_lineRangePtr.clear();

	m_boundingBox = i2d::CRectangle();
	m_isEmpty = true;
}


bool CBitmapRegion::IsBitmapRegionEmpty() const
{
	return m_isEmpty;
}


// private methods

void CBitmapRegion::CalculateRegionBoundingBox(const i2d::CRectangle& objectBoundingBox)
{
	I_ASSERT(m_bitmapPtr != NULL);
	istd::CIndex2d imageSize = m_bitmapPtr->GetImageSize();

	m_boundingBox.SetLeft(qFloor(objectBoundingBox.GetLeft()));
	m_boundingBox.SetRight(qCeil(objectBoundingBox.GetRight()));
	m_boundingBox.SetTop(qFloor(objectBoundingBox.GetTop()));
	m_boundingBox.SetBottom(qCeil(objectBoundingBox.GetBottom()));

	m_boundingBox = m_boundingBox.GetIntersection(i2d::CRectangle(imageSize));
}


void CBitmapRegion::CreateFromCircle(const i2d::CCircle& circle)
{
	I_ASSERT(m_bitmapPtr != NULL);

	CalculateRegionBoundingBox(circle.GetBoundingBox());
	if (m_boundingBox.IsEmpty() || !m_boundingBox.IsValid()){
		ResetBitmapRegion();

		return;
	}

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();
	double radius2 = radius * radius;

	int boundingTop = int(m_boundingBox.GetTop());
	int boundingHeight = int(m_boundingBox.GetHeight());
		
	int linesCount = boundingHeight;
	int lineSize = m_bitmapPtr->GetLinesDifference();
	int bytesPerPixel = m_bitmapPtr->GetPixelBitsCount() / 8;

	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);
	istd::CRange imageLineRange(0, m_bitmapPtr->GetImageSize().GetX() - 1);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		const quint8* basePtr = (const quint8*)m_bitmapPtr->GetLinePtr(lineIndex) + lineSize * boundingTop;
	
		PixelRanges& rangeList = m_rangesContainer[lineIndex];
		PixelRange pixelRange;

		m_lineRangePtr[lineIndex] =  NULL;

		double y = (lineIndex + boundingTop - center.GetY());
		double radiusDiff = radius2 - y * y;

		if (radiusDiff >= 0){
			radiusDiff = ::sqrt(radiusDiff);
			double left = center.GetX() - radiusDiff;
			double right = center.GetX() + radiusDiff;

			istd::CRange outerRadiusRange(qFloor(left), qCeil(right));
			outerRadiusRange.Intersection(imageLineRange);
			
			if (outerRadiusRange.IsValid() && !outerRadiusRange.IsEmpty()){
				pixelRange.pixelBufferPtr = basePtr + int(outerRadiusRange.GetMinValue()) * bytesPerPixel;
				pixelRange.range = outerRadiusRange;
				rangeList.push_back(pixelRange);
				m_lineRangePtr[lineIndex] = &rangeList;
				m_isEmpty = false;
			}
		}
	}
}


void CBitmapRegion::CreateFromRectangle(const i2d::CRectangle& rect)
{
	I_ASSERT(m_bitmapPtr != NULL);

	CalculateRegionBoundingBox(rect);
	if (m_boundingBox.IsEmpty() || !m_boundingBox.IsValid()){
		ResetBitmapRegion();

		return;
	}

	m_isEmpty = false;

	int linesCount = int(m_boundingBox.GetHeight());
	int lineSize = m_bitmapPtr->GetLinesDifference();
	int bytesPerPixel = m_bitmapPtr->GetPixelBitsCount() / 8;
	
	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	int boundingTop = int(m_boundingBox.GetTop());

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		const quint8* basePtr = (const quint8*)m_bitmapPtr->GetLinePtr(lineIndex) + lineSize * boundingTop;
	
		PixelRanges& rangeList = m_rangesContainer[lineIndex];
		PixelRange pixelRange;
		pixelRange.range = istd::CRange(m_boundingBox.GetLeft(), m_boundingBox.GetRight());
		pixelRange.pixelBufferPtr = basePtr + int(m_boundingBox.GetLeft()) * bytesPerPixel;
	
		rangeList.push_back(pixelRange);
		m_lineRangePtr[lineIndex] = &rangeList;
	}
}


void CBitmapRegion::CreateFromAnnulus(const i2d::CAnnulus& annulus)
{
	I_ASSERT(m_bitmapPtr != NULL);

	CalculateRegionBoundingBox(annulus.GetBoundingBox());
	if (m_boundingBox.IsEmpty() || !m_boundingBox.IsValid()){
		ResetBitmapRegion();

		return;
	}

	const i2d::CVector2d& center = annulus.GetCenter();
	double outerRadius = annulus.GetOuterRadius();
	double outerRadius2 = outerRadius * outerRadius;

	double innerRadius = annulus.GetInnerRadius();
	double innerRadius2 = innerRadius * innerRadius;

	int linesCount = int(m_boundingBox.GetHeight());
	int lineSize = m_bitmapPtr->GetLinesDifference();
	int bytesPerPixel = m_bitmapPtr->GetPixelBitsCount() / 8;

	int boundingTop = int(m_boundingBox.GetTop());
	double centerX = center.GetX();
	double centerY = center.GetY();

	istd::CRange imageLineRange(0, m_bitmapPtr->GetImageSize().GetX() - 1);
	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		const quint8* basePtr = (const quint8*)m_bitmapPtr->GetLinePtr(lineIndex) + lineSize * boundingTop;
	
		PixelRanges& rangeList = m_rangesContainer[lineIndex];
		double y = (lineIndex + boundingTop - centerY);

		double outputRadiusDiff = outerRadius2 - y * y;

		if (outputRadiusDiff < 0){
			m_lineRangePtr[lineIndex] =  NULL;

			continue;
		}

		outputRadiusDiff = ::sqrt(outerRadius2 - y * y);
		double left = centerX - outputRadiusDiff;
		double right = centerX + outputRadiusDiff;

		istd::CRange outerRadiusRange(qFloor(left), qCeil(right));
		outerRadiusRange.Intersection(imageLineRange);

		double innerRadiusDiff = innerRadius2 - y * y;

		if (innerRadius2 - y * y >= 0 && innerRadius2 != outerRadius){
			innerRadiusDiff = ::sqrt(innerRadiusDiff);
			double left = centerX - innerRadiusDiff;
			double right = centerX + innerRadiusDiff;

			istd::CRange innerRadiusRange(qFloor(left), qCeil(right));
			innerRadiusRange.Intersection(imageLineRange);

			if (innerRadiusRange.IsValid() && !innerRadiusRange.IsEmpty()){
				double bigRaduisRangeMax = outerRadiusRange.GetMaxValue();
				outerRadiusRange.SetMaxValue(innerRadiusRange.GetMinValue());
				innerRadiusRange.SetMinValue(innerRadiusRange.GetMaxValue());
				innerRadiusRange.SetMaxValue(bigRaduisRangeMax);

				if (!innerRadiusRange.IsEmpty()){
					PixelRange range;
					range.pixelBufferPtr = basePtr + int(innerRadiusRange.GetMinValue()) * bytesPerPixel;
					range.range = innerRadiusRange;
					rangeList.push_back(range);
				}
			}
		}

		if (outerRadiusRange.IsValid() && !outerRadiusRange.IsEmpty()){
			PixelRange range;
			range.pixelBufferPtr = basePtr + int(outerRadiusRange.GetMinValue()) * bytesPerPixel;
			range.range = outerRadiusRange;
			rangeList.push_back(range);
		}

		if (rangeList.size() != 0){
			m_lineRangePtr[lineIndex] = &rangeList;
			m_isEmpty = false;
		}
		else{
			m_lineRangePtr[lineIndex] =  NULL;
		}
	}
}


void CBitmapRegion::CreateFromPolygon(const i2d::CPolygon& polygon)
{
	I_ASSERT(m_bitmapPtr != NULL);

	CalculateRegionBoundingBox(polygon.GetBoundingBox());
	if (m_boundingBox.IsEmpty() || !m_boundingBox.IsValid()){
		ResetBitmapRegion();

		return;
	}

	int bitmapWidth = m_bitmapPtr->GetImageSize().GetX();

	int linesCount = int(m_boundingBox.GetHeight());
	int lineSize = m_bitmapPtr->GetLinesDifference();
	int bytesPerPixel = m_bitmapPtr->GetPixelBitsCount() / 8;

	int boundingTop = int(m_boundingBox.GetTop());

	// QVector of lines by Y coordinates;
	// every line is QList of X coordinates of the polygon lines points 
	QVector<QList<int> > scanVector(linesCount);

	int nodesCount = polygon.GetNodesCount();
	for (int i = 0; i < nodesCount; i++){
		int nextIndex = i + 1;
		if (nextIndex >= nodesCount){
			nextIndex = 0;
		}

		i2d::CVector2d startPoint = polygon.GetNode(i);
		i2d::CVector2d endPoint = polygon.GetNode(nextIndex);

		double y1 = startPoint.GetY() - boundingTop;
		double y2 = endPoint.GetY() - boundingTop;

		double x1 = startPoint.GetX();
		double x2 = endPoint.GetX();

		// special case: line is parallel to X
		if (y1 == y2){
			continue;
		}

		if (y2 < y1){
			qSwap(y1, y2);
			qSwap(x1, x2);
		}

		double deltaX = (x2 - x1) / (y2 - y1);
		double x = x1;

		for (int y = y1; y < (int)y2 && y < linesCount; y++){
			// check if we are inside the image
			if (y >= 0){
				InsertVectorPoint(scanVector[y], x);
			}

			x += deltaX;
		}
	}

	// build the scan ranges
	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		const quint8* basePtr = (const quint8*)m_bitmapPtr->GetLinePtr(lineIndex) + lineSize * boundingTop;

		PixelRanges& rangeList = m_rangesContainer[lineIndex];

		const QList<int>& lineList = scanVector.at(lineIndex);
		int count = lineList.count() - (lineList.count() % 2);
		for (int i = 0; i < count; i += 2){
			int x1 = lineList.at(i);
			int x2 = lineList.at(i+1);

			x1 = qMax(0, qMin(x1, bitmapWidth));
			x2 = qMax(0, qMin(x2, bitmapWidth));
			
			PixelRange pixelRange;
			pixelRange.range = istd::CRange(x1, x2);
			pixelRange.pixelBufferPtr = basePtr + x1 * bytesPerPixel;
			rangeList.push_back(pixelRange);
		}

		if (rangeList.size() != 0){
			m_lineRangePtr[lineIndex] = &rangeList;
			m_isEmpty = false;
		}
		else{
			m_lineRangePtr[lineIndex] =  NULL;
		}
	}
}


void CBitmapRegion::InsertVectorPoint(QList<int>& list, int value)
{
	if (list.isEmpty()){
		list.append(value);
		return;
	}

	if (list.first() >= value){
		list.prepend(value);
		return;
	}

	if (list.last() <= value){
		list.append(value);
		return;
	}

	for (int i = 1; i < list.count(); i++){
		if (value <= list.at(i)){
			list.insert(i, value);
			return;
		}
	}

	// that should not happen!
	I_CRITICAL();
}


} // namespace iimg


