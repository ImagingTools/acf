#include <iimg/CScanlineMask.h>


// STL includes
#include <set>
#include <vector>

// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include <iser/CPrimitiveTypesSerializer.h>


namespace iimg
{


// public methods

CScanlineMask::CScanlineMask()
:	m_firstLinePos(0),
	m_isBoundingBoxValid(false)
{
}


bool CScanlineMask::IsBitmapRegionEmpty() const
{
	return m_rangesContainer.empty();
}


i2d::CRect CScanlineMask::GetBoundingRect() const
{
	EnsureBoundingBoxValid();

	return m_boundingBox;
}


void CScanlineMask::ResetScanlines(const istd::CIntRange& verticalRange)
{
	Q_ASSERT(verticalRange.IsValid());

	m_firstLinePos = verticalRange.GetMinValue();

	int linesCount = verticalRange.GetLength();
	Q_ASSERT(linesCount >= 0);

	m_rangesContainer.clear();
	m_scanlines.resize(linesCount);

	for (int i = 0; i < linesCount; ++i){
		m_scanlines[i] = -1;
	}

	m_isBoundingBoxValid = false;
}


const istd::CIntRanges* CScanlineMask::GetPixelRanges(int lineIndex) const
{
	int rangeIndex = lineIndex - m_firstLinePos;

	if ((rangeIndex >= 0) && (rangeIndex < int(m_scanlines.size()))){
		int containerIndex = m_scanlines[rangeIndex];
		if (containerIndex >= 0){
			Q_ASSERT(containerIndex < m_rangesContainer.size());

			return &m_rangesContainer[containerIndex];
		}
	}

	return NULL;
}


void CScanlineMask::CreateFilled(const i2d::CRect& clipArea)
{
	m_firstLinePos = clipArea.GetTop();
	int endLinePos = clipArea.GetBottom();
	int linesCount = qMax(endLinePos - m_firstLinePos, 0);
	Q_ASSERT(linesCount >= 0);

	m_rangesContainer.clear();
	m_rangesContainer.push_back(istd::CIntRanges());
	istd::CIntRanges& rangeList = m_rangesContainer.back();
	rangeList.InsertSwitchPoint(clipArea.GetLeft());
	rangeList.InsertSwitchPoint(clipArea.GetRight());

	m_scanlines.resize(linesCount);
	for (int i = 0; i < linesCount; ++i){
		m_scanlines[i] = 0;	// set all lines to the same range
	}

	m_boundingBox = clipArea;
	m_isBoundingBoxValid = true;
}


bool CScanlineMask::CreateFromGeometry(const i2d::IObject2d& geometry, const i2d::CRect* clipAreaPtr)
{
	const i2d::CTubePolyline* polylinePtr = dynamic_cast<const i2d::CTubePolyline*>(&geometry);
	if (polylinePtr != NULL){
		CreateFromTube(*polylinePtr, clipAreaPtr);

		return true;
	}

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

	const CScanlineMask* scanLinePtr = dynamic_cast<const CScanlineMask*>(&geometry);
	if (scanLinePtr != NULL){
		if ((clipAreaPtr != NULL) && !clipAreaPtr->IsInside(scanLinePtr->GetBoundingRect())){
			// we need to clip
			CreateFilled(*clipAreaPtr);
			Intersection(*scanLinePtr);
		}
		else{
			// we dont need to clip
			*this = *scanLinePtr;
		}

		return true;
	}

	return false;
}


void CScanlineMask::CreateFromCircle(const i2d::CCircle& circle, const i2d::CRect* clipAreaPtr)
{
	i2d::CCircle recalibratedCircle;
	recalibratedCircle.SetCalibration(GetCalibration());
	recalibratedCircle.CopyFrom(circle, istd::IChangeable::CM_CONVERT);

	InitFromBoudingBox(recalibratedCircle.GetBoundingBox(), clipAreaPtr);

	int linesCount = int(m_scanlines.size());

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = recalibratedCircle.GetPosition();
	double radius = recalibratedCircle.GetRadius();
	double radius2 = radius * radius;

#if QT_VERSION >= 0x040700
	m_rangesContainer.reserve(linesCount);
#endif

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_scanlines[lineIndex] = -1;

		double y = (lineIndex + m_firstLinePos - center.GetY());
		double radiusDiff2 = radius2 - y * y;

		if (radiusDiff2 >= 0){
			double radiusDiff = qSqrt(radiusDiff2);
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
				m_rangesContainer.push_back(istd::CIntRanges());
				istd::CIntRanges& rangeList = m_rangesContainer.back();

				rangeList.InsertSwitchPoint(left);
				rangeList.InsertSwitchPoint(right);

				m_scanlines[lineIndex] = m_rangesContainer.size() - 1;
			}
		}
	}
}


void CScanlineMask::CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr)
{
	i2d::CRectangle recalibratedRect;
	recalibratedRect.SetCalibration(GetCalibration());
	recalibratedRect.CopyFrom(rect, istd::IChangeable::CM_CONVERT);

	InitFromBoudingBox(recalibratedRect, clipAreaPtr);

	int linesCount = int(m_scanlines.size());

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	m_rangesContainer.push_back(istd::CIntRanges());
	istd::CIntRanges& rangeList = m_rangesContainer.back();
	rangeList.InsertSwitchPoint(int(recalibratedRect.GetLeft() + 0.5));
	rangeList.InsertSwitchPoint(int(recalibratedRect.GetRight() + 0.5));


	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_scanlines[lineIndex] = 0;	// set all lines to the same range
	}
}


void CScanlineMask::CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr)
{
	i2d::CAnnulus recalibratedAnnulus;
	recalibratedAnnulus.SetCalibration(GetCalibration());
	recalibratedAnnulus.CopyFrom(annulus, istd::IChangeable::CM_CONVERT);

	InitFromBoudingBox(recalibratedAnnulus.GetBoundingBox(), clipAreaPtr);
	int linesCount = int(m_scanlines.size());

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = recalibratedAnnulus.GetCenter();
	double outerRadius = recalibratedAnnulus.GetOuterRadius();
	double outerRadius2 = outerRadius * outerRadius;

	double innerRadius = recalibratedAnnulus.GetInnerRadius();
	double innerRadius2 = innerRadius * innerRadius;

	double centerX = center.GetX();
	double centerY = center.GetY();

	m_scanlines.resize(linesCount);

#if QT_VERSION >= 0x040700
	m_rangesContainer.reserve(linesCount);
#endif

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		istd::CIntRanges rangeList;

		double y = (lineIndex + m_firstLinePos - centerY);

		double outputRadiusDiff2 = outerRadius2 - y * y;

		if (outputRadiusDiff2 < 0){
			m_scanlines[lineIndex] = -1;

			continue;
		}

		double outputRadiusDiff = qSqrt(outputRadiusDiff2);
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
			double innerRadiusDiff = qSqrt(innerRadiusDiff2);

			int innerLeft = int(centerX - innerRadiusDiff + 0.5);
			int innerRight = int(centerX + innerRadiusDiff + 0.5);

			rangeList.InsertSwitchPoint(outerLeft);
			rangeList.InsertSwitchPoint(innerLeft);

			rangeList.InsertSwitchPoint(innerRight);
			rangeList.InsertSwitchPoint(outerRight);
		}
		else{
			rangeList.InsertSwitchPoint(outerLeft);
			rangeList.InsertSwitchPoint(outerRight);
		}

		if (!rangeList.IsEmpty()){
			m_rangesContainer.push_back(rangeList);

			m_scanlines[lineIndex] = m_rangesContainer.size() - 1;
		}
		else{
			m_scanlines[lineIndex] = -1;
		}
	}
}


void CScanlineMask::CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr)
{
	i2d::CPolygon recalibratedPolygon;
	recalibratedPolygon.SetCalibration(GetCalibration());
	recalibratedPolygon.CopyFrom(polygon, istd::IChangeable::CM_CONVERT);

	InitFromBoudingBox(recalibratedPolygon.GetBoundingBox(), clipAreaPtr);
	int linesCount = int(m_scanlines.size());

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	// QVector of lines by Y coordinates;
	// every line is QList of X coordinates of the polygon lines points 
	std::vector< std::set<int> > scanVector(linesCount);

	int nodesCount = recalibratedPolygon.GetNodesCount();
	for (int i = 0; i < nodesCount; i++){
		int nextIndex = i + 1;
		if (nextIndex >= nodesCount){
			nextIndex = 0;
		}

		i2d::CVector2d startPoint = recalibratedPolygon.GetNodePos(i);
		i2d::CVector2d endPoint = recalibratedPolygon.GetNodePos(nextIndex);

		double y1 = startPoint.GetY() - m_firstLinePos;
		double y2 = endPoint.GetY() - m_firstLinePos;

		double x1 = startPoint.GetX();
		double x2 = endPoint.GetX();

		if (y2 < y1){
			qSwap(y1, y2);
			qSwap(x1, x2);
		}

		int firstLine = qMax(int(y1 + 0.5), 0);
		int lastLine = qMin(int(y2 + 0.5), linesCount);

		if (firstLine < lastLine){
			double deltaX = (x2 - x1) / (y2 - y1);

			// add line to list of points
			double positionX = x1 + (firstLine + 0.5 - y1) * deltaX;
			for (int lineIndex = firstLine; lineIndex < lastLine; ++lineIndex){
				int x = int(positionX + 0.5);

				std::set<int>& points = scanVector[lineIndex];

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
	m_scanlines.resize(linesCount);

#if QT_VERSION >= 0x040700
	m_rangesContainer.reserve(linesCount);
#endif

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		istd::CIntRanges rangeList;

		const std::set<int>& lineList = scanVector.at(lineIndex);
		Q_ASSERT((lineList.size() % 2) == 0);	// pair number of points should be calculated

		int rangesCount = int(lineList.size()) / 2;

		std::set<int>::const_iterator iter = lineList.begin();
		for (int i = 0; i < rangesCount; ++i){
			int left = *(iter++);
			Q_ASSERT(iter != lineList.end());
			int right = *(iter++);

			if (clipAreaPtr != NULL){
				if (left < clipAreaPtr->GetLeft()){
					left = clipAreaPtr->GetLeft();
				}

				if (right > clipAreaPtr->GetRight()){
					right = clipAreaPtr->GetRight();
				}
			}

			if (left < right){
				rangeList.InsertSwitchPoint(left);
				rangeList.InsertSwitchPoint(right);
			}
		}

		if (!rangeList.IsEmpty()){
			m_rangesContainer.push_back(rangeList);

			m_scanlines[lineIndex] = m_rangesContainer.size() - 1;
		}
		else{
			m_scanlines[lineIndex] =  -1;
		}
	}
}


void CScanlineMask::CreateFromTube(const i2d::CTubePolyline& tube, const i2d::CRect* clipAreaPtr)
{
	i2d::CPolygon polygon;
	polygon.SetCalibration(tube.GetCalibration());

	int nodesCount = tube.GetNodesCount();
	if (nodesCount >= 2){
		int segmentsCount = tube.GetSegmentsCount();
		Q_ASSERT(segmentsCount >= 1);

		int polygonNodesCount = (segmentsCount + 1) * 2;

		polygon.SetNodesCount(polygonNodesCount);
		int leftIndex = 0;
		int rightIndex = polygonNodesCount - 1;

		for (int segmentIndex = 0; segmentIndex <= segmentsCount; ++segmentIndex){
			int nodeIndex = segmentIndex % nodesCount;

			i2d::CVector2d kneeVector = tube.GetKneeVector(nodeIndex);
			const i2d::CVector2d& nodePosition = tube.GetNodePos(nodeIndex);
			const i2d::CTubeNode& tubeNode = tube.GetTNodeData(nodeIndex);
			istd::CRange range = tubeNode.GetTubeRange();

			i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
			i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
		
			polygon.SetNodePos(leftIndex++, leftPos);
			polygon.SetNodePos(rightIndex--, rightPos);
		}
	}

	return CreateFromPolygon(polygon, clipAreaPtr);
}


void CScanlineMask::GetInverted(const i2d::CRect& clipArea, CScanlineMask& result) const
{
	result.m_rangesContainer.clear();
	result.m_scanlines.clear();
	result.m_firstLinePos = 0;
	result.m_isBoundingBoxValid = false;

	if (clipArea.IsEmpty()){
		return;
	}

	int scanLinesCount = clipArea.GetHeight();
	result.m_scanlines.resize(scanLinesCount, 0);
	result.m_firstLinePos = clipArea.GetTop();

	istd::CIntRanges fullRange(clipArea.GetHorizontalRange());

	result.m_rangesContainer.push_back(fullRange);

	for (int scanLineIndex = 0; scanLineIndex < scanLinesCount; ++scanLineIndex){
		const istd::CIntRanges* lineRangesPtr = this->GetPixelRanges(clipArea.GetTop() + scanLineIndex);
		if (lineRangesPtr != NULL){
			istd::CIntRanges invRanges;
			lineRangesPtr->GetInverted(invRanges, &clipArea.GetHorizontalRange());
			if (invRanges.IsEmpty()){
				result.m_scanlines[scanLineIndex] = -1;
				continue;
			}

			if (invRanges != fullRange){
				result.m_rangesContainer.push_back(invRanges);
				result.m_scanlines[scanLineIndex] = result.m_rangesContainer.size() - 1;
			}
		}
	}
}


CScanlineMask CScanlineMask::GetUnion(const CScanlineMask& mask) const
{
	CScanlineMask result;

	GetUnion(mask, result);

	return result;
}


void CScanlineMask::GetUnion(const CScanlineMask& mask, CScanlineMask& result) const
{
	result.m_isBoundingBoxValid = false;

	result.m_firstLinePos = qMin(m_firstLinePos, mask.m_firstLinePos);
	int endLineY = qMax(m_firstLinePos + int(m_scanlines.size()), mask.m_firstLinePos + int(mask.m_scanlines.size()));

	result.m_scanlines.resize(endLineY - m_firstLinePos);

	for (int resultLineIndex = 0; resultLineIndex < int(result.m_scanlines.size()); ++resultLineIndex){
		int y = resultLineIndex + result.m_firstLinePos;

		const istd::CIntRanges* rangesPtr = NULL;
		const istd::CIntRanges* maskRangesPtr = NULL;

		int lineIndex = y - m_firstLinePos;
		if ((lineIndex >= 0) && (lineIndex < int(m_scanlines.size()))){
			int containerIndex = m_scanlines[lineIndex];
			if (containerIndex >= 0){
				Q_ASSERT(containerIndex < m_rangesContainer.size());

				rangesPtr = &m_rangesContainer[containerIndex];
			}
		}

		int maskLineIndex = y - mask.m_firstLinePos;
		if ((maskLineIndex >= 0) && (maskLineIndex < int(mask.m_scanlines.size()))){
			int containerIndex = mask.m_scanlines[maskLineIndex];
			if (containerIndex >= 0){
				Q_ASSERT(containerIndex < mask.m_rangesContainer.size());

				maskRangesPtr = &mask.m_rangesContainer[containerIndex];
			}
		}

		istd::CIntRanges resultRanges;
		if (rangesPtr != NULL){
			if (maskRangesPtr != NULL){
				rangesPtr->GetUnion(*maskRangesPtr, resultRanges);
			}
			else{
				resultRanges = *rangesPtr;
			}
		}
		else if (maskRangesPtr != NULL){
			resultRanges = *maskRangesPtr;
		}

		if (!resultRanges.IsEmpty()){
			result.m_rangesContainer.push_back(resultRanges);

			result.m_scanlines[resultLineIndex] = result.m_rangesContainer.size() - 1;
		}
		else{
			result.m_scanlines[resultLineIndex] = -1;
		}
	}
}


void CScanlineMask::Union(const CScanlineMask& mask)
{
	*this = GetUnion(mask);
}


CScanlineMask CScanlineMask::GetIntersection(const CScanlineMask& mask) const
{
	CScanlineMask result;

	GetIntersection(mask, result);

	return result;
}


void CScanlineMask::GetIntersection(const CScanlineMask& mask, CScanlineMask& result) const
{
	result.m_isBoundingBoxValid = false;

	result.m_firstLinePos = qMax(m_firstLinePos, mask.m_firstLinePos);
	int endLineY = qMin(m_firstLinePos + int(m_scanlines.size()), mask.m_firstLinePos + int(mask.m_scanlines.size()));

	if (result.m_firstLinePos >= endLineY){
		result.ResetImage();

		return;
	}

	result.m_scanlines.resize(endLineY - m_firstLinePos);

	for (int resultLineIndex = 0; resultLineIndex < int(result.m_scanlines.size()); ++resultLineIndex){
		int y = result.m_firstLinePos + resultLineIndex;

		istd::CIntRanges resultRanges;

		int lineIndex = y - m_firstLinePos;
		int maskLineIndex = y - mask.m_firstLinePos;
		if (		(lineIndex >= 0) && (lineIndex < int(m_scanlines.size())) &&
					(maskLineIndex >= 0) && (maskLineIndex < int(mask.m_scanlines.size()))){
			int containerIndex = m_scanlines[lineIndex];
			int maskContainerIndex = mask.m_scanlines[maskLineIndex];

			if ((containerIndex >= 0) && (maskContainerIndex >= 0)){
				const istd::CIntRanges& ranges = m_rangesContainer[containerIndex];
				const istd::CIntRanges& maskRange = mask.m_rangesContainer[maskContainerIndex];

				ranges.GetIntersection(maskRange, resultRanges);
			}
		}

		if (!resultRanges.IsEmpty()){
			result.m_rangesContainer.push_back(resultRanges);

			result.m_scanlines[resultLineIndex] = result.m_rangesContainer.size() - 1;
		}
		else{
			result.m_scanlines[resultLineIndex] = -1;
		}
	}
}


void CScanlineMask::Intersection(const CScanlineMask& mask)
{
	*this = GetIntersection(mask);
}


CScanlineMask CScanlineMask::GetTranslated(int dx, int dy) const
{
	CScanlineMask result;

	GetTranslated(dx, dy, result);

	return result;
}


void CScanlineMask::GetTranslated(int dx, int dy, CScanlineMask& result) const
{
	result = *this;

	result.Translate(dx, dy);
}


void CScanlineMask::Translate(int dx, int dy)
{
	m_firstLinePos += dy;

	m_boundingBox.SetTop(m_boundingBox.GetTop() + dy);
	m_boundingBox.SetBottom(m_boundingBox.GetBottom() + dy);

	if (dx != 0){
		m_boundingBox.SetLeft(m_boundingBox.GetLeft() + dx);
		m_boundingBox.SetRight(m_boundingBox.GetRight() + dx);

		for (		RangesContainer::Iterator lineIter = m_rangesContainer.begin();
					lineIter != m_rangesContainer.end();
					++lineIter){
			istd::CIntRanges& lineRanges = *lineIter;

			lineRanges.ShiftRanges(dx);
		}
	}
}


void CScanlineMask::Erode(int leftValue, int rightValue, int topValue, int bottomValue)
{
	CScanlineMask::Dilate(-leftValue, -rightValue, -topValue, -bottomValue);
}


void CScanlineMask::Dilate(int leftValue, int rightValue, int topValue, int bottomValue)
{
	int newScanLinesCount = int(m_scanlines.size()) + topValue + bottomValue;
	if (newScanLinesCount <= 0){
		ResetImage();

		return;
	}

	m_isBoundingBoxValid = false;

	for (		RangesContainer::Iterator lineIter = m_rangesContainer.begin();
				lineIter != m_rangesContainer.end();
				++lineIter){
		istd::CIntRanges& lineRanges = *lineIter;

		lineRanges.Dilate(leftValue, rightValue);
	}

	m_firstLinePos -= topValue;

	int dilLines = topValue + bottomValue;

	if (dilLines == 0){
		return;
	}

	if (m_rangesContainer.size() <= 1){	// special case: rectangles
		m_scanlines.resize(newScanLinesCount, -1);

		int downCounter = 0;

		for (		Scanlines::iterator iter = m_scanlines.begin();
					iter != m_scanlines.begin();
					++iter){
			int rangeIndex = *iter;
			if (rangeIndex < 0){
				if (dilLines > 0){
					if (downCounter-- > 0){
						*iter = 0;	// set the range
					}
				}
				else{
					downCounter = -dilLines;
				}
			}
			else{
				if (dilLines > 0){
					downCounter = dilLines;
				}
				else{
					if (downCounter-- > 0){
						*iter = -1;	// remove the range
					}
				}
			}
		}

		return;
	}

	if (dilLines > 0){
		int restDilLines = dilLines;

		for (int shiftY = 1; restDilLines > 0; restDilLines -= shiftY, shiftY <<= 1){
			CScanlineMask shiftedMask;
			GetTranslated(0, qMin(shiftY, restDilLines), shiftedMask);

			Union(shiftedMask);
		}
	}
	else{
		int restErodeLines = -dilLines;

		for (int shiftY = 1; restErodeLines > 0; restErodeLines -= shiftY, shiftY <<= 1){
			CScanlineMask shiftedMask;
			GetTranslated(0, -qMin(shiftY, restErodeLines), shiftedMask);

			Intersection(shiftedMask);
		}
	}
}


// reimplemented (i2d::IObject2d)

i2d::CVector2d CScanlineMask::GetCenter() const
{
	EnsureBoundingBoxValid();

	return i2d::CRectangle(m_boundingBox).GetCenter();
}


void CScanlineMask::MoveCenterTo(const i2d::CVector2d& position)
{
	i2d::CVector2d diff = position - GetCenter();

	Translate(int(diff.GetX() + 0.5), int(diff.GetY() + 0.5));
}


i2d::CRectangle CScanlineMask::GetBoundingBox() const
{
	EnsureBoundingBoxValid();

	return m_boundingBox;
}


// reimplemented (iimg::IRasterImage)

bool CScanlineMask::IsEmpty() const
{
	return CScanlineMask::GetBoundingBox().IsEmpty();
}


void CScanlineMask::ResetImage()
{
	m_rangesContainer.clear();
	m_scanlines.clear();

	m_boundingBox = i2d::CRect::GetEmpty();
	m_isBoundingBoxValid = true;
}


void CScanlineMask::ClearImage()
{
	// placeholder
}


istd::CIndex2d CScanlineMask::GetImageSize() const
{
	return CScanlineMask::GetBoundingBox().GetRightBottom().ToIndex2d();
}


int CScanlineMask::GetComponentsCount() const
{
	return 1;
}


icmm::CVarColor CScanlineMask::GetColorAt(const istd::CIndex2d& position) const
{
	int scanLine = position.GetY() - m_firstLinePos;
	if ((scanLine >= 0) && (scanLine < int(m_scanlines.size()))){
		int containerIndex = m_scanlines[scanLine];

		if (containerIndex >= 0){
			const istd::CIntRanges& scanLine = m_rangesContainer[containerIndex];

			if (scanLine.IsInside(position.GetX())){
				return icmm::CVarColor(1, 1);
			}
		}
	}

	return icmm::CVarColor(1, 0);
}


bool CScanlineMask::SetColorAt(const istd::CIndex2d& /*position*/, const icmm::CVarColor& /*color*/)
{
	return false;	// this is not editable using color set at pixel index
}


// reimplemented (iser::ISerializable)

bool CScanlineMask::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag firstLineTag("FirstLine", "First line (top position)", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag lineContainerTag("LineContainer", "Container of scan lines", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag scanLineTag("Line", "Single scan line", iser::CArchiveTag::TT_GROUP, &lineContainerTag);
	static iser::CArchiveTag containerIndicesTag("ContainerIndices", "List of container indices for each line", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag indexTag("Index", "Container index", iser::CArchiveTag::TT_LEAF, &containerIndicesTag);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(firstLineTag);
	retVal = retVal && archive.Process(m_firstLinePos);
	retVal = retVal && archive.EndTag(firstLineTag);

	if (archive.IsStoring()){
		int containerSize = m_rangesContainer.size();

		retVal = retVal && archive.BeginMultiTag(lineContainerTag, scanLineTag, containerSize);
		for (		RangesContainer::Iterator lineIter = m_rangesContainer.begin();
					lineIter != m_rangesContainer.end();
					++lineIter){
			istd::CIntRanges& scanLine = *lineIter;

			retVal = retVal && archive.BeginTag(scanLineTag);
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeIntRanges(archive, scanLine);
			retVal = retVal && archive.EndTag(scanLineTag);
		}

		retVal = retVal && archive.EndTag(lineContainerTag);

		int indicesCount = int(m_scanlines.size());

		retVal = retVal && archive.BeginMultiTag(containerIndicesTag, indexTag, indicesCount);
		for (		Scanlines::const_iterator indexIter = m_scanlines.begin();
					indexIter != m_scanlines.end();
					++indexIter){
			int containerIndex = *indexIter;

			retVal = retVal && archive.BeginTag(indexTag);
			retVal = retVal && archive.Process(containerIndex);
			retVal = retVal && archive.EndTag(indexTag);
		}

		retVal = retVal && archive.EndTag(containerIndicesTag);
	}
	else{
		m_isBoundingBoxValid = false;
		m_rangesContainer.clear();
		m_scanlines.clear();

		int containerSize = 0;

		retVal = retVal && archive.BeginMultiTag(lineContainerTag, scanLineTag, containerSize);
		if (!retVal){
			return false;
		}

		for (int containerIndex = 0; containerIndex < containerSize; ++containerIndex){
			m_rangesContainer.push_back(istd::CIntRanges());
			istd::CIntRanges& scanLine = m_rangesContainer.last();

			retVal = retVal && archive.BeginTag(scanLineTag);
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeIntRanges(archive, scanLine);
			retVal = retVal && archive.EndTag(scanLineTag);
		}

		retVal = retVal && archive.EndTag(lineContainerTag);

		int indicesCount = 0;

		retVal = retVal && archive.BeginMultiTag(containerIndicesTag, indexTag, indicesCount);
		if (!retVal){
			return false;
		}

		m_scanlines.resize(indicesCount);
		for (int lineIndex = 0; lineIndex < indicesCount; ++lineIndex){
			int containerIndex = -1;
			retVal = retVal && archive.BeginTag(indexTag);
			retVal = retVal && archive.Process(containerIndex);
			retVal = retVal && archive.EndTag(indexTag);

			if (containerIndex >= containerSize){
				ResetImage();
				return false;
			}

			m_scanlines[lineIndex] = containerIndex;
		}

		retVal = retVal && archive.EndTag(containerIndicesTag);
	}

	return retVal;
}


int CScanlineMask::GetSupportedOperations() const
{
	return SO_COPY | SO_RESET;
}


bool CScanlineMask::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const CScanlineMask* maskPtr = dynamic_cast<const CScanlineMask*>(&object);
	if ((maskPtr != NULL) && ((mode != CM_CONVERT) || (GetCalibration() == maskPtr->GetCalibration()))){
		*this = *maskPtr;

		return true;
	}

	return false;
}


bool CScanlineMask::ResetData(CompatibilityMode /*mode*/)
{
	ResetImage();

	return true;
}


bool CScanlineMask::operator==(const CScanlineMask& mask) const
{
	int firstLineIndex = qMin(m_firstLinePos, mask.m_firstLinePos);
	int endLineIndex = qMax(int(m_firstLinePos + m_scanlines.size()), int(mask.m_firstLinePos + mask.m_scanlines.size()));
	for (int lineIndex = firstLineIndex; lineIndex < endLineIndex; ++lineIndex){
		int rangeIndex = -1;
		if ((lineIndex >= m_firstLinePos) && (lineIndex < int(m_firstLinePos + m_scanlines.size()))){
			rangeIndex = m_scanlines[lineIndex - m_firstLinePos];
		}

		int maskRangeIndex = -1;
		if ((lineIndex >= mask.m_firstLinePos) && (lineIndex < int(mask.m_firstLinePos + mask.m_scanlines.size()))){
			maskRangeIndex = mask.m_scanlines[lineIndex - mask.m_firstLinePos];
		}

		if (rangeIndex >= 0){
			if (maskRangeIndex >= 0){
				if (m_rangesContainer[rangeIndex] != mask.m_rangesContainer[maskRangeIndex]){
					return false;
				}
			}
			else{
				if (!m_rangesContainer[rangeIndex].IsEmpty()){
					return false;
				}
			}
		}
		else{
			if (maskRangeIndex >= 0){
				if (!mask.m_rangesContainer[maskRangeIndex].IsEmpty()){
					return false;
				}
			}
		}
	}

	return true;
}


// protected methods

void CScanlineMask::CalcBoundingBox() const
{
	istd::CIntRange rangeX = istd::CIntRange::GetInvalid();

	for (int i = 0; i < int(m_scanlines.size()); ++i){
		int containerIndex = m_scanlines[i];

		if (containerIndex >= 0){
			const istd::CIntRanges& scanLine = m_rangesContainer[containerIndex];

			const istd::CIntRanges::SwitchPoints& points = scanLine.GetSwitchPoints();

			if (!points.empty()){
				int minX = *points.begin();
				int maxX = *points.rbegin();

				if (rangeX.IsValid()){
					rangeX.Unite(istd::CIntRange(minX, maxX));
				}
				else{
					rangeX = istd::CIntRange(minX, maxX);
				}
			}
		}
	}

	if (rangeX.IsValid()){
		m_boundingBox.SetTop(m_firstLinePos);
		m_boundingBox.SetLeft(rangeX.GetMinValue());
		m_boundingBox.SetBottom(m_firstLinePos + int(m_scanlines.size()));
		m_boundingBox.SetRight(rangeX.GetMaxValue());
	}
	else{
		m_boundingBox = i2d::CRect::GetEmpty();
	}

	m_isBoundingBoxValid = true;
}


void CScanlineMask::InitFromBoudingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr)
{
	int firstLinePos = int(qFloor(objectBoundingBox.GetTop()));
	int endLinePos = int(qCeil(objectBoundingBox.GetBottom()));

	if (clipAreaPtr != NULL){
		firstLinePos = qMax(clipAreaPtr->GetTop(), firstLinePos);
		endLinePos = qMin(clipAreaPtr->GetBottom(), endLinePos);
	}

	if (endLinePos < firstLinePos){
		endLinePos = firstLinePos;
	}

	ResetScanlines(istd::CIntRange(firstLinePos, endLinePos));
}


// related global functions

uint qHash(const CScanlineMask& key, uint seed)
{
	uint retVal = seed;

	int lineIndex = key.m_firstLinePos;
	for (CScanlineMask::Scanlines::const_iterator iter = key.m_scanlines.cbegin(); iter != key.m_scanlines.cend(); ++iter, ++lineIndex){
		int containerIndex = *iter;

		if (containerIndex >= 0){
			const istd::CIntRanges& scanLine = key.m_rangesContainer[containerIndex];

			if (!scanLine.IsEmpty()){
				retVal = retVal ^ qHash(scanLine, seed) ^ uint(lineIndex);
			}
		}
	}

	return retVal;
}


} // namespace iimg


