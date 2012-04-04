#include "iimg/CBitmapRegion.h"


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
	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&geometry);
	const i2d::CRectangle* rectanglePtr = dynamic_cast<const i2d::CRectangle*>(&geometry);

	if (annulusPtr != NULL){
		CreateFromAnnulus(*annulusPtr);

		return true;
	}

	if (circlePtr != NULL){
		CreateFromCircle(*circlePtr);

		return true;
	}

	if (rectanglePtr != NULL){
		CreateFromRectangle(*rectanglePtr);

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

	m_boundingBox.SetLeft(::floor(objectBoundingBox.GetLeft()));
	m_boundingBox.SetRight(::ceil(objectBoundingBox.GetRight()));
	m_boundingBox.SetTop(::floor(objectBoundingBox.GetTop()));
	m_boundingBox.SetBottom(::ceil(objectBoundingBox.GetBottom()));

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

			istd::CRange outerRadiusRange(::floor(left), ::ceil(right));
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

		istd::CRange outerRadiusRange(::floor(left), ::ceil(right));
		outerRadiusRange.Intersection(imageLineRange);

		double innerRadiusDiff = innerRadius2 - y * y;

		if (innerRadius2 - y * y >= 0 && innerRadius2 != outerRadius){
			innerRadiusDiff = ::sqrt(innerRadiusDiff);
			double left = centerX - innerRadiusDiff;
			double right = centerX + innerRadiusDiff;

			istd::CRange innerRadiusRange(::floor(left), ::ceil(right));
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


} // namespace iimg


