#include "iipr/CImageProjection.h"

#include "i2d/CRectangle.h"


namespace iipr
{


CImageProjection::CImageProjection()
{
}


CImageProjection::CImageProjection(const iimg::IBitmap& bitmap, const istd::CIndex2d& startPoint, const istd::CIndex2d& endPoint)
{
	CreateProjection(bitmap, startPoint, endPoint);
}


const CImageProjection::ProjectionData& CImageProjection::GetProjectionData() const
{
	return m_projectionData;
}


void CImageProjection::CreateProjection(
			const iimg::IBitmap& bitmap, 
			const istd::CIndex2d& startPoint, 
			const istd::CIndex2d& endPoint)
{
	int imageWidth = bitmap.GetImageSize().GetX();
	int imageHeight = bitmap.GetImageSize().GetY();

	i2d::CRectangle imageRect(0, 0, imageHeight, imageWidth);

	m_projectionData.clear();

	if (!imageRect.Contains(startPoint) || !imageRect.Contains(endPoint)){
		I_CRITICAL();

		return;
	}

	const I_BYTE* imageDataPtr = (I_BYTE*)bitmap.GetLinePtr(0);
	int imageLineDistance = bitmap.GetLinesDifference();
		
	if (startPoint.GetY() == endPoint.GetY()){
		// horizontal projection:
		int minX = istd::Min(startPoint.GetX(), endPoint.GetX());
		int maxX = minX + labs(startPoint.GetX() - endPoint.GetX());

		for(int x = minX; x < maxX; ++x){
			m_projectionData.push_back(imageDataPtr[startPoint.GetY() * imageLineDistance + x]);
		}
	} 
	else if (startPoint.GetX() == endPoint.GetX()){
		// vertical projection:
		int minY = istd::Min(startPoint.GetY(), endPoint.GetY());
		int maxY = minY + labs(startPoint.GetY() - endPoint.GetY());
		for(int y = minY; y < maxY; ++y){
			m_projectionData.push_back(imageDataPtr[y * imageLineDistance + startPoint.GetX()]);
		}
	} 
	else{
		// any direction
		int dx = endPoint.GetX() - startPoint.GetX();
		int dy = endPoint.GetY() - startPoint.GetY();
		int ix = abs(dx);
		int iy = abs(dy);

		int stepY = (dy > 0)? 1: (dy == 0)? 0: -1;
		int stepX = (dx > 0)? 1: (dx == 0)? 0: -1;

		int imageX = startPoint.GetX();
		int imageY = startPoint.GetY();

		int x = 0;
		int y = 0;

		m_projectionData.push_back(imageDataPtr[imageY * imageLineDistance + imageX]);

		int increment = istd::Max(ix, iy);
		for (int i = 0; i <= increment; i++){
			x += ix;
			y += iy;
			bool isDataPoint = false;

			if (x > increment){
				isDataPoint = true;
				x -= increment;
				imageX += stepX;
			}
			if (y > increment){
				isDataPoint = true;
				y -= increment;
				imageY += stepY;
			}

			if (isDataPoint){
				m_projectionData.push_back(imageDataPtr[imageY * imageWidth + imageX]);
			}
		}
	}
}


} // namespace iipr

