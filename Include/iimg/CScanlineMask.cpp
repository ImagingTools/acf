#include "iimg/CScanlineMask.h"


// STL includes
#include <set>

// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include "iser/CPrimitiveTypesSerializer.h"


namespace iimg
{


// public methods

CScanlineMask::CScanlineMask()
{
}


bool CScanlineMask::IsBitmapRegionEmpty() const
{
	return m_rangesContainer.empty();
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
		m_scanlines[i] = NULL;
	}

	m_isBoundingBoxValid = false;
}


const istd::CIntRanges* CScanlineMask::GetPixelRanges(int lineIndex) const
{
	int rangeIndex = lineIndex - m_firstLinePos;

	if ((rangeIndex >= 0) && (rangeIndex < int(m_scanlines.size()))){
		return m_scanlines[rangeIndex];
	}

	return NULL;
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

	return false;
}


void CScanlineMask::CreateFromCircle(const i2d::CCircle& circle, const i2d::CRect* clipAreaPtr)
{
	InitFromBoudingBox(circle.GetBoundingBox(), clipAreaPtr);

	int linesCount = m_scanlines.size();

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();
	double radius2 = radius * radius;

#if QT_VERSION >= 0x040700
    m_rangesContainer.reserve(linesCount);
#endif

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_rangesContainer.push_back(istd::CIntRanges());
		istd::CIntRanges& rangeList = m_rangesContainer.back();

		m_scanlines[lineIndex] =  NULL;

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
				rangeList.InsertSwitchPoint(left);
				rangeList.InsertSwitchPoint(right);

				m_scanlines[lineIndex] = &rangeList;
			}
		}
	}
}


void CScanlineMask::CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr)
{
	InitFromBoudingBox(rect, clipAreaPtr);

	int linesCount = m_scanlines.size();

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	m_rangesContainer.push_back(istd::CIntRanges());
	istd::CIntRanges& rangeList = m_rangesContainer.back();
	rangeList.InsertSwitchPoint(int(rect.GetLeft() + 0.5));
	rangeList.InsertSwitchPoint(int(rect.GetRight() + 0.5));


	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_scanlines[lineIndex] = &rangeList;	// set all lines to the same range
	}
}


void CScanlineMask::CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr)
{
	InitFromBoudingBox(annulus.GetBoundingBox(), clipAreaPtr);
	int linesCount = m_scanlines.size();

	if (linesCount <= 0){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = annulus.GetCenter();
	double outerRadius = annulus.GetOuterRadius();
	double outerRadius2 = outerRadius * outerRadius;

	double innerRadius = annulus.GetInnerRadius();
	double innerRadius2 = innerRadius * innerRadius;

	double centerX = center.GetX();
	double centerY = center.GetY();

	m_scanlines.resize(linesCount);

#if QT_VERSION >= 0x040700
    m_rangesContainer.reserve(linesCount);
#endif

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_rangesContainer.push_back(istd::CIntRanges());
		istd::CIntRanges& rangeList = m_rangesContainer.back();

		double y = (lineIndex + m_firstLinePos - centerY);

		double outputRadiusDiff2 = outerRadius2 - y * y;

		if (outputRadiusDiff2 < 0){
			m_scanlines[lineIndex] =  NULL;

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
			m_scanlines[lineIndex] = &rangeList;
		}
		else{
			m_scanlines[lineIndex] =  NULL;
		}
	}
}


void CScanlineMask::CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr)
{
	InitFromBoudingBox(polygon.GetBoundingBox(), clipAreaPtr);
	int linesCount = m_scanlines.size();

	if (linesCount <= 0){
		ResetImage();

		return;
	}

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
		m_rangesContainer.push_back(istd::CIntRanges());
		istd::CIntRanges& rangeList = m_rangesContainer.back();

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
			m_scanlines[lineIndex] = &rangeList;
		}
		else{
			m_scanlines[lineIndex] =  NULL;
		}
	}
}


void CScanlineMask::CreateFromTube(const i2d::CTubePolyline& tube, const i2d::CRect* clipAreaPtr)
{
	i2d::CPolygon polygon;

	int nodesCount = tube.GetNodesCount();
	if (nodesCount >= 1){
		polygon.SetNodesCount(nodesCount * 2);
		int leftIndex = 0;
		int rightIndex = nodesCount * 2 - 1;

		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			i2d::CVector2d kneeVector = tube.GetKneeVector(nodeIndex);
			const i2d::CVector2d& nodePosition = tube.GetNode(nodeIndex);
			const i2d::CTubeNode& tubeNode = tube.GetTNodeData(nodeIndex);
			istd::CRange range = tubeNode.GetTubeRange();

			i2d::CVector2d leftPos = nodePosition - kneeVector * range.GetMinValue();
			i2d::CVector2d rightPos = nodePosition - kneeVector * range.GetMaxValue();
		
			polygon.SetNode(leftIndex++, leftPos);
			polygon.SetNode(rightIndex--, rightPos);
		}
	}

	return CreateFromPolygon(polygon, clipAreaPtr);
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
	int endLineY = qMax(m_firstLinePos + m_scanlines.size(), mask.m_firstLinePos + mask.m_scanlines.size());

	result.m_scanlines.resize(endLineY - m_firstLinePos);

	for (int resultLineIndex = 0; resultLineIndex < result.m_scanlines.size(); ++resultLineIndex){
		int y = resultLineIndex + result.m_firstLinePos;

		const istd::CIntRanges* rangesPtr = NULL;
		const istd::CIntRanges* maskRangesPtr = NULL;

		int lineIndex = y - m_firstLinePos;
		if ((lineIndex >= 0) && (lineIndex < m_scanlines.size())){
			rangesPtr = m_scanlines[lineIndex];
		}

		int maskLineIndex = y - mask.m_firstLinePos;
		if ((maskLineIndex >= 0) && (maskLineIndex < mask.m_scanlines.size())){
			maskRangesPtr = mask.m_scanlines[lineIndex];
		}

		if (rangesPtr != NULL){
			result.m_rangesContainer.push_back(istd::CIntRanges());
			istd::CIntRanges& unionRanges = result.m_rangesContainer.back();

			if (maskRangesPtr != NULL){
				rangesPtr->GetUnion(*maskRangesPtr, unionRanges);
			}
			else{
				unionRanges = *rangesPtr;
			}

			result.m_scanlines[resultLineIndex] = &unionRanges;
		}
		else if (maskRangesPtr != NULL){
			result.m_rangesContainer.push_back(istd::CIntRanges());
			istd::CIntRanges& unionRanges = result.m_rangesContainer.back();

			unionRanges = *maskRangesPtr;

			result.m_scanlines[resultLineIndex] = &unionRanges;
		}
		else{
			result.m_scanlines[resultLineIndex] = NULL;
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
	int endLineY = qMin(m_firstLinePos + m_scanlines.size(), mask.m_firstLinePos + mask.m_scanlines.size());

	if (endLineY >= result.m_firstLinePos){
		result.ResetImage();

		return;
	}

	result.m_scanlines.resize(endLineY - m_firstLinePos);

	for (int resultLineIndex = 0; resultLineIndex < result.m_scanlines.size(); ++resultLineIndex){
		int y = resultLineIndex + result.m_firstLinePos;

		int lineIndex = y - m_firstLinePos;
		int maskLineIndex = y - mask.m_firstLinePos;
		if (		(lineIndex >= 0) && (lineIndex < m_scanlines.size()) &&
					(maskLineIndex >= 0) && (maskLineIndex < mask.m_scanlines.size())){
			const istd::CIntRanges* rangesPtr = m_scanlines[lineIndex];
			const istd::CIntRanges* maskRangesPtr = mask.m_scanlines[lineIndex];

			istd::CIntRanges resultRanges;
			rangesPtr->GetIntersection(*maskRangesPtr, resultRanges);

			if (!resultRanges.IsEmpty()){
				result.m_rangesContainer.push_back(resultRanges);
				istd::CIntRanges& intersectedRanges = result.m_rangesContainer.back();

				result.m_scanlines[y - result.m_firstLinePos] = &intersectedRanges;
			}
			else{
				result.m_scanlines[y - result.m_firstLinePos] = NULL;
			}
		}
		else{
			result.m_scanlines[y - result.m_firstLinePos] = NULL;
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


// reimplemented (i2d::IObject2d)

i2d::CVector2d CScanlineMask::GetCenter() const
{
	return i2d::CRectangle(m_boundingBox).GetCenter();
}


void CScanlineMask::MoveCenterTo(const i2d::CVector2d& position)
{
	i2d::CVector2d diff = position - GetCenter();

	Translate(int(diff.GetX() + 0.5), int(diff.GetY() + 0.5));
}


i2d::CRectangle CScanlineMask::GetBoundingBox() const
{
	if (!m_isBoundingBoxValid){
		istd::CIntRange rangeX = istd::CIntRange::GetInvalid();

		for (int i = 0; i < m_scanlines.size(); ++i){
			const istd::CIntRanges* scanLinePtr = m_scanlines[i];

			if (scanLinePtr != NULL){
				const istd::CIntRanges::SwitchPoints& points = scanLinePtr->GetSwitchPoints();

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

		if (rangeX.IsEmpty()){
			m_boundingBox = i2d::CRect::GetEmpty();
		}
		else{
			m_boundingBox.SetTop(m_firstLinePos);
			m_boundingBox.SetLeft(rangeX.GetMinValue());
			m_boundingBox.SetBottom(m_firstLinePos + m_scanlines.size());
			m_boundingBox.SetRight(rangeX.GetMaxValue());
		}

		m_isBoundingBoxValid = true;
	}

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
	if ((scanLine >= 0) && (scanLine < m_scanlines.size())){
		const istd::CIntRanges* rangesPtr = m_scanlines[scanLine];
		if (rangesPtr != NULL){
			if (rangesPtr->IsInside(position.GetX())){
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
	static iser::CArchiveTag minYTag("MinY", "Minimal Y");
	static iser::CArchiveTag maxYTag("MaxY", "Maximal Y (exclusive)");
	static iser::CArchiveTag scanlinesTag("Scanelines", "List of mask scan lines");
	static iser::CArchiveTag scanLineTag("Line", "Single scan line");

	bool retVal = true;

	if (archive.IsStoring()){
		int minY = m_firstLinePos;

		retVal = retVal && archive.BeginTag(minYTag);
		retVal = retVal && archive.Process(minY);
		retVal = retVal && archive.EndTag(minYTag);

		int scanLinesCount = m_scanlines.size();

		istd::CIntRanges emptyRanges;

		retVal = retVal && archive.BeginMultiTag(scanlinesTag, scanLineTag, scanLinesCount);
		for (		Scanlines::ConstIterator lineIter = m_scanlines.constBegin();
					lineIter != m_scanlines.constEnd();
					++lineIter){
			retVal = retVal && archive.BeginTag(scanLineTag);

			const istd::CIntRanges* rangesPtr = *lineIter;
			if (rangesPtr != NULL){
				retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeIntRanges(archive, *const_cast<istd::CIntRanges*>(rangesPtr));
			}
			else{
				retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeIntRanges(archive, emptyRanges);
			}

			retVal = retVal && archive.EndTag(scanLineTag);
		}
	}
	else{
		int minY = 0;

		retVal = retVal && archive.BeginTag(minYTag);
		retVal = retVal && archive.Process(minY);
		retVal = retVal && archive.EndTag(minYTag);

		m_rangesContainer.clear();
		m_scanlines.clear();

		int scanLinesCount = 0;

		retVal = retVal && archive.BeginMultiTag(scanlinesTag, scanLineTag, scanLinesCount);

		if (!retVal || (scanLinesCount < 0)){
			return false;
		}

		m_scanlines.resize(scanLinesCount);

		for (int lineIndex = 0; lineIndex < scanLinesCount; ++lineIndex){
			retVal = retVal && archive.BeginTag(scanLineTag);

			istd::CIntRanges ranges;
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeIntRanges(archive, ranges);

			if (!retVal){
				CScanlineMask::ResetImage();

				return false;
			}

			if (!ranges.IsEmpty()){
				m_rangesContainer.push_back(ranges);

				m_scanlines[lineIndex] = &m_rangesContainer.back();
			}
			else{
				m_scanlines[lineIndex] = NULL;
			}

			retVal = retVal && archive.EndTag(scanLineTag);
		}
	}

	return retVal;
}


// protected methods

void CScanlineMask::InitFromBoudingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr)
{
	int firstLinePos = int(qFloor(objectBoundingBox.GetTop()));
	int endLinePos = int(qCeil(objectBoundingBox.GetBottom()));

	if (clipAreaPtr != NULL){
		firstLinePos = qMax(clipAreaPtr->GetTop(), firstLinePos);
		endLinePos = qMin(clipAreaPtr->GetBottom(), endLinePos);
	}

	ResetScanlines(istd::CIntRange(firstLinePos, endLinePos));
}


} // namespace iimg


