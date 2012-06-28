#include "iimg/CScanlineMask.h"


// STL includes
#include <set>

// Qt includes
#include <QtCore/qmath.h>


namespace iimg
{


// public methods

CScanlineMask::CScanlineMask()
:	m_isEmpty(true)
{
}


bool CScanlineMask::IsBitmapRegionEmpty() const
{
	return m_isEmpty;
}


i2d::CRect CScanlineMask::GetBoundingBox() const
{
	return m_boundingBox;
}


const CScanlineMask::PixelRanges* CScanlineMask::GetPixelRanges(int lineIndex) const
{
	int rangeIndex = lineIndex - m_boundingBox.GetTop();

	if (rangeIndex >= 0 && rangeIndex < int(m_lineRangePtr.size())){
		return m_lineRangePtr[rangeIndex];
	}

	return NULL;
}


void CScanlineMask::ResetBitmapRegion()
{
	m_rangesContainer.clear();
	m_lineRangePtr.clear();

	m_boundingBox = i2d::CRect::GetEmpty();

	m_isEmpty = true;
}


bool CScanlineMask::CreateFromGeometry(const i2d::IObject2d& geometry, const i2d::CRect* clipAreaPtr)
{
	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(&geometry);
	if (annulusPtr != NULL){
		CreateFromAnnulus(*annulusPtr, clipAreaPtr);

		return true;
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&geometry);
	if (circlePtr != NULL){
		CreateFromCircle(*circlePtr, clipAreaPtr);

		return true;
	}

	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(&geometry);
	if (rectanglePtr != NULL){
		CreateFromRectangle(*rectanglePtr, clipAreaPtr);

		return true;
	}

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(&geometry);
	if (polygonPtr != NULL){
		CreateFromPolygon(*polygonPtr, clipAreaPtr);

		return true;
	}

	return false;
}


void CScanlineMask::CreateFromCircle(const i2d::CCircle& circle, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(circle.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetBitmapRegion();

		return;
	}

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();
	double radius2 = radius * radius;

	int linesCount = m_boundingBox.GetHeight();

	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		PixelRanges& rangeList = m_rangesContainer[lineIndex];

		m_lineRangePtr[lineIndex] =  NULL;

		double y = (lineIndex + m_boundingBox.GetTop() - center.GetY());
		double radiusDiff2 = radius2 - y * y;

		if (radiusDiff2 >= 0){
			double radiusDiff = std::sqrt(radiusDiff2);
			int left = int(center.GetX() - radiusDiff);
			int right = int(center.GetX() + radiusDiff);

			if (clipAreaPtr != NULL){
				if (left < clipAreaPtr->GetLeft()){
					left = clipAreaPtr->GetLeft();
				}

				if (right > clipAreaPtr->GetRight()){
					right = clipAreaPtr->GetRight();
				}
			}

			if (left < right){
				rangeList.push_back(istd::CIntRange(left, right));

				m_lineRangePtr[lineIndex] = &rangeList;

				m_isEmpty = false;
			}
		}
	}
}


void CScanlineMask::CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(rect, clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetBitmapRegion();

		return;
	}

	m_isEmpty = false;

	int linesCount = m_boundingBox.GetHeight();

	m_lineRangePtr.resize(linesCount);

	m_rangesContainer.resize(1);
	PixelRanges& rangeList = m_rangesContainer.front();
	rangeList.push_back(m_boundingBox.GetHorizontalRange());

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_lineRangePtr[lineIndex] = &rangeList;	// set all lines to the same range
	}
}


void CScanlineMask::CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(annulus.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetBitmapRegion();

		return;
	}

	const i2d::CVector2d& center = annulus.GetCenter();
	double outerRadius = annulus.GetOuterRadius();
	double outerRadius2 = outerRadius * outerRadius;

	double innerRadius = annulus.GetInnerRadius();
	double innerRadius2 = innerRadius * innerRadius;

	int linesCount = int(m_boundingBox.GetHeight());

	double centerX = center.GetX();
	double centerY = center.GetY();

	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		PixelRanges& rangeList = m_rangesContainer[lineIndex];
		double y = (lineIndex + m_boundingBox.GetTop() - centerY);

		double outputRadiusDiff2 = outerRadius2 - y * y;

		if (outputRadiusDiff2 < 0){
			m_lineRangePtr[lineIndex] =  NULL;

			continue;
		}

		double outputRadiusDiff = std::sqrt(outputRadiusDiff2);
		int outerLeft = int(centerX - outputRadiusDiff);
		int outerRight = int(centerX + outputRadiusDiff);

		if (clipAreaPtr != NULL){
			if (outerLeft < clipAreaPtr->GetLeft()){
				outerLeft = clipAreaPtr->GetLeft();
			}

			if (outerRight > clipAreaPtr->GetRight()){
				outerRight = clipAreaPtr->GetRight();
			}
		}

		double innerRadiusDiff2 = innerRadius2 - y * y;
		if (innerRadiusDiff2 >= 0){
			double innerRadiusDiff = std::sqrt(innerRadiusDiff2);

			int innerLeft = int(centerX - innerRadiusDiff);
			int innerRight = int(centerX + innerRadiusDiff);

			if (innerLeft < innerRight){
				if (outerLeft < innerLeft){
					rangeList.push_back(istd::CIntRange(outerLeft, innerLeft));
				}

				if (innerRight < outerRight){
					rangeList.push_back(istd::CIntRange(innerRight, outerRight));
				}

				continue;
			}
		}
		
		if (outerLeft < outerRight){
			rangeList.push_back(istd::CIntRange(outerLeft, outerRight));
		}

		if (!rangeList.isEmpty()){
			m_lineRangePtr[lineIndex] = &rangeList;

			m_isEmpty = false;
		}
		else{
			m_lineRangePtr[lineIndex] =  NULL;
		}
	}
}


void CScanlineMask::CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(polygon.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetBitmapRegion();

		return;
	}

	int linesCount = m_boundingBox.GetHeight();

	// QVector of lines by Y coordinates;
	// every line is QList of X coordinates of the polygon lines points 
	QVector< std::set<int> > scanVector(linesCount);

	int nodesCount = polygon.GetNodesCount();
	for (int i = 0; i < nodesCount; i++){
		int nextIndex = i + 1;
		if (nextIndex >= nodesCount){
			nextIndex = 0;
		}

		i2d::CVector2d startPoint = polygon.GetNode(i);
		i2d::CVector2d endPoint = polygon.GetNode(nextIndex);

		double y1 = startPoint.GetY() - m_boundingBox.GetTop();
		double y2 = endPoint.GetY() - m_boundingBox.GetTop();

		double x1 = startPoint.GetX();
		double x2 = endPoint.GetX();

		if (y2 < y1){
			qSwap(y1, y2);
			qSwap(x1, x2);
		}

		int firstLine = qMax(int(y1), 0);
		int lastLine = qMin(int(y2), linesCount);

		if (firstLine < lastLine){
			double deltaX = (x2 - x1) / (y2 - y1);

			// add line to list of points
			double positionX = x1 + (firstLine - y1) * deltaX;
			for (int lineIndex = firstLine; lineIndex < lastLine; ++lineIndex){
				int x = int(positionX);

				std::set<int> points = scanVector[lineIndex];

				std::set<int>::iterator foundIter = points.find(x);
				if (foundIter != points.end()){
					points.erase(foundIter);
				}
				else{
					points.insert(x);
				}

				positionX += deltaX;
			}
		}
	}

	// build the scan ranges
	m_lineRangePtr.resize(linesCount);
	m_rangesContainer.resize(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		PixelRanges& rangeList = m_rangesContainer[lineIndex];

		const std::set<int>& lineList = scanVector.at(lineIndex);
		Q_ASSERT((lineList.size() % 2) == 0);	// pair number of points should be calculated

		int rangesCount = int(lineList.size()) / 2;

		std::set<int>::const_iterator iter = lineList.begin();
		for (int i = 0; i < rangesCount; ++i){
			int left = *(iter++);
			Q_ASSERT(iter != lineList.end());
			int right = *(iter++);
			Q_ASSERT(iter != lineList.end());

			if (clipAreaPtr != NULL){
				if (left < clipAreaPtr->GetLeft()){
					left = clipAreaPtr->GetLeft();
				}

				if (right > clipAreaPtr->GetRight()){
					right = clipAreaPtr->GetRight();
				}
			}

			if (left < right){
				rangeList.push_back(istd::CIntRange(left, right));
			}
		}

		if (!rangeList.isEmpty()){
			m_lineRangePtr[lineIndex] = &rangeList;
			m_isEmpty = false;
		}
		else{
			m_lineRangePtr[lineIndex] =  NULL;
		}
	}
}


// static methods

void CScanlineMask::UnionLine(const PixelRanges& line, PixelRanges& result)
{
	PixelRanges::Iterator resultIter = result.begin();

	for (		PixelRanges::ConstIterator iter = line.constBegin();
				iter != line.constEnd();
				++iter){
		const istd::CIntRange& range = *iter;

		for (; (resultIter != result.end()) && (range.GetMinValue() > resultIter->GetMaxValue()); ++resultIter);	// skip non colliding ranges

		if (resultIter != result.end()){
			Q_ASSERT(range.GetMinValue() <= resultIter->GetMaxValue());

			if (range.GetMaxValue() < resultIter->GetMinValue()){	// our new range is before
				resultIter = result.insert(resultIter, range);
			}
			else{
				if (range.GetMinValue() < resultIter->GetMinValue()){	// our new range extends the range on the left side
					resultIter->SetMinValue(range.GetMinValue());
				}

				if (range.GetMaxValue() > resultIter->GetMaxValue()){	// our new range extends the range on the right side
					int maxRight = range.GetMaxValue();

					// remove contained ranges
					PixelRanges::Iterator removeIter = resultIter;
					for (		++resultIter;
								(removeIter != result.end()) && (range.GetMaxValue() > removeIter->GetMinValue());
								removeIter = result.erase(removeIter)){
						if (removeIter->GetMaxValue() > maxRight){
							maxRight = removeIter->GetMaxValue();
						}
					}

					resultIter->SetMaxValue(maxRight);

					resultIter = removeIter;
				}
			}
		}
		else{
			// at the end we can simply insert elements
			resultIter = result.insert(resultIter, range);
		}
	}
}


void CScanlineMask::IntersectLine(const PixelRanges& line, PixelRanges& result)
{
	PixelRanges::Iterator resultIter = result.begin();

	for (		PixelRanges::ConstIterator iter = line.constBegin();
				iter != line.constEnd();){
		const istd::CIntRange& range = *iter;

		// remove non colliding ranges
		for (; (resultIter != result.end()) && (range.GetMinValue() > resultIter->GetMaxValue()); resultIter = result.erase(resultIter));

		if (resultIter == result.end()){
			// no more elements in result - finish
			return;
		}

		Q_ASSERT(range.GetMinValue() <= resultIter->GetMaxValue());

		if (range.GetMaxValue() > resultIter->GetMinValue()){	// our new range overlaps range in result
			if (range.GetMaxValue() < resultIter->GetMaxValue()){	// our new range cuts the range on the left side
				// divide the result range into 2 segments
				resultIter = result.insert(resultIter, istd::CRange(resultIter->GetMinValue(), range.GetMaxValue()));
				++resultIter;
				resultIter->SetMinValue(range.GetMaxValue() + 1);

				++iter;
			}
			else if (range.GetMaxValue() > resultIter->GetMaxValue()){	// our new range contains the result range
				++resultIter;
			}
		}
		else{
			++iter;
		}
	}
}


// protected methods

void CScanlineMask::SetBoundingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr)
{
	m_boundingBox.SetLeft(int(qFloor(objectBoundingBox.GetLeft())));
	m_boundingBox.SetRight(int(qCeil(objectBoundingBox.GetRight())));
	m_boundingBox.SetTop(int(qFloor(objectBoundingBox.GetTop())));
	m_boundingBox.SetBottom(int(qCeil(objectBoundingBox.GetBottom())));

	if (clipAreaPtr != NULL){
		m_boundingBox.Intersection(*clipAreaPtr);
	}
}


} // namespace iimg


