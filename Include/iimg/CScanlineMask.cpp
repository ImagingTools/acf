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


const istd::CIntRanges* CScanlineMask::GetPixelRanges(int lineIndex) const
{
	int rangeIndex = lineIndex - m_boundingBox.GetTop();

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
	SetBoundingBox(circle.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

		return;
	}

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();
	double radius2 = radius * radius;

	int linesCount = m_boundingBox.GetHeight();

	m_scanlines.resize(linesCount);
	m_rangesContainer.reserve(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_rangesContainer.push_back(istd::CIntRanges());
		istd::CIntRanges& rangeList = m_rangesContainer.back();

		m_scanlines[lineIndex] =  NULL;

		double y = (lineIndex + m_boundingBox.GetTop() - center.GetY());
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

				m_isEmpty = false;
			}
		}
	}
}


void CScanlineMask::CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(rect, clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

		return;
	}

	m_isEmpty = false;

	int linesCount = m_boundingBox.GetHeight();

	m_scanlines.resize(linesCount);

	m_rangesContainer.push_back(istd::CIntRanges());
	istd::CIntRanges& rangeList = m_rangesContainer.back();
	rangeList.InsertSwitchPoint(m_boundingBox.GetLeft());
	rangeList.InsertSwitchPoint(m_boundingBox.GetRight());


	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_scanlines[lineIndex] = &rangeList;	// set all lines to the same range
	}
}


void CScanlineMask::CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(annulus.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

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

	m_scanlines.resize(linesCount);
	m_rangesContainer.reserve(linesCount);

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		m_rangesContainer.push_back(istd::CIntRanges());
		istd::CIntRanges& rangeList = m_rangesContainer.back();

		double y = (lineIndex + m_boundingBox.GetTop() - centerY);

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

			m_isEmpty = false;
		}
		else{
			m_scanlines[lineIndex] =  NULL;
		}
	}
}


void CScanlineMask::CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr)
{
	SetBoundingBox(polygon.GetBoundingBox(), clipAreaPtr);
	if (!m_boundingBox.IsValidNonEmpty()){
		ResetImage();

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
	m_rangesContainer.reserve(linesCount);

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
			m_isEmpty = false;
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
	result.m_boundingBox = m_boundingBox.GetUnion(mask.m_boundingBox);
	result.m_isEmpty = m_isEmpty || mask.m_isEmpty;

	result.m_scanlines.resize(result.m_boundingBox.GetHeight());

	for (int y = result.m_boundingBox.GetTop(); y < result.m_boundingBox.GetBottom(); ++y){
		const istd::CIntRanges* rangesPtr = NULL;
		const istd::CIntRanges* maskRangesPtr = NULL;

		int lineIndex = y - m_boundingBox.GetTop();
		if ((lineIndex >= 0) && (lineIndex < m_scanlines.size())){
			rangesPtr = m_scanlines[lineIndex];
		}

		int maskLineIndex = y - mask.m_boundingBox.GetTop();
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

			result.m_scanlines[y - result.m_boundingBox.GetTop()] = &unionRanges;
		}
		else if (maskRangesPtr != NULL){
			result.m_rangesContainer.push_back(istd::CIntRanges());
			istd::CIntRanges& unionRanges = result.m_rangesContainer.back();

			unionRanges = *maskRangesPtr;

			result.m_scanlines[y - result.m_boundingBox.GetTop()] = &unionRanges;
		}
		else{
			result.m_scanlines[y - result.m_boundingBox.GetTop()] = NULL;
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
	result.m_boundingBox = m_boundingBox.GetUnion(mask.m_boundingBox);
	result.m_isEmpty = true;

	result.m_scanlines.resize(result.m_boundingBox.GetHeight());

	for (int y = result.m_boundingBox.GetTop(); y < result.m_boundingBox.GetBottom(); ++y){
		int lineIndex = y - m_boundingBox.GetTop();
		int maskLineIndex = y - mask.m_boundingBox.GetTop();
		if (		(lineIndex >= 0) && (lineIndex < m_scanlines.size()) &&
					(maskLineIndex >= 0) && (maskLineIndex < mask.m_scanlines.size())){
			const istd::CIntRanges* rangesPtr = m_scanlines[lineIndex];
			const istd::CIntRanges* maskRangesPtr = mask.m_scanlines[lineIndex];

			istd::CIntRanges resultRanges;
			rangesPtr->GetIntersection(*maskRangesPtr, resultRanges);

			if (!resultRanges.IsEmpty()){
				result.m_rangesContainer.push_back(resultRanges);
				istd::CIntRanges& intersectedRanges = result.m_rangesContainer.back();

				result.m_scanlines[y - result.m_boundingBox.GetTop()] = &intersectedRanges;

				result.m_isEmpty = false;
			}
			else{
				result.m_scanlines[y - result.m_boundingBox.GetTop()] = NULL;
			}
		}
		else{
			result.m_scanlines[y - result.m_boundingBox.GetTop()] = NULL;
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


// reimplemented (iimg::IRasterImage)

bool CScanlineMask::IsEmpty() const
{
	for (		Scanlines::ConstIterator iter = m_scanlines.constBegin();
				iter != m_scanlines.constEnd();
				++iter){
		if (*iter != NULL){
			return false;
		}
	}

	return true;
}


void CScanlineMask::ResetImage()
{
	m_rangesContainer.clear();
	m_scanlines.clear();

	m_boundingBox = i2d::CRect::GetEmpty();

	m_isEmpty = true;
}


istd::CIndex2d CScanlineMask::GetImageSize() const
{
	return m_boundingBox.GetRightBottom();
}


int CScanlineMask::GetComponentsCount() const
{
	return 1;
}


icmm::CVarColor CScanlineMask::GetColorAt(const istd::CIndex2d& position) const
{
	int scanLine = position.GetY() - m_boundingBox.GetTop();
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
		int minY = m_boundingBox.GetTop();

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


