#include "iipr/CLineProjectionProcessor.h"


#include "i2d/CRectangle.h"

#include "iimg/TPixelConversion.h"

#include "iipr/CProjectionData.h"



namespace iipr
{


// local functions

template <class PixelConversion>
bool ProjectionFunction(
			const I_BYTE* firstPixelAddress,
			const istd::CIndex2d axisSizes,
			const istd::CIndex2d addressDiffs,
			const i2d::CLine2d& projectionLine,
			const i2d::CLine2d& clippedLine,
			const PixelConversion& conversion,
			CProjectionData& results)
{
	I_ASSERT(projectionLine.GetPoint1().GetX() <= projectionLine.GetPoint2().GetX());
	I_ASSERT(clippedLine.GetPoint1().GetX() <= clippedLine.GetPoint2().GetX());
	I_ASSERT(axisSizes[0] != 0);
	I_ASSERT(axisSizes[1] != 0);
	I_ASSERT(addressDiffs[0] != 0);
	I_ASSERT(addressDiffs[1] != 0);

	i2d::CVector2d delta = clippedLine.GetDiffVector();
	i2d::CVector2d beginPoint = clippedLine.GetPoint1();
	I_ASSERT(delta.GetX() >= 0);

	int axis1Begin = int(beginPoint.GetX() + I_BIG_EPSILON);
	int axis1End = int(beginPoint.GetX() + delta.GetX() - I_BIG_EPSILON) + 1;
	I_ASSERT(axis1Begin <= axis1End);
	I_ASSERT(axis1Begin >= 0);
	I_ASSERT(axis1End <= axisSizes[0]);

	iimg::IBitmap& projectionImage = results.GetProjectionImage();
	if (projectionImage.CreateBitmap(istd::CIndex2d(axis1End - axis1Begin, 1))){
		PixelConversion::DestPixelType* projectionPtr = (I_BYTE*)projectionImage.GetLinePtr(0);

		double axis2Delta = delta.GetY() / delta.GetX();
		double axis2Position = axis2Delta * (axis1Begin - beginPoint.GetX() + 0.5) + beginPoint.GetY() + 0.5;

		for (int axis1Offset = axis1Begin * addressDiffs[0]; axis1Offset < axis1End * addressDiffs[0]; axis1Offset += addressDiffs[0]){
			int axis2Index = int(axis2Position);
			PixelConversion::CalcPixelType value;
			if (axis2Index <= 0){
				value = conversion.GetCalc(*(const PixelConversion::SourcePixelType*)(firstPixelAddress[axis1Offset]));
			}
			else{
				if (axis2Index >= axisSizes[1]){
					int axis2Offset = (axisSizes[1] - 1) * addressDiffs[1];
					value = conversion.GetCalc(*(const PixelConversion::SourcePixelType*)(firstPixelAddress[axis1Offset + axis2Offset]));
				}
				else{
					double alpha = axis2Position - axis2Index;
					I_ASSERT(alpha >= 0);
					I_ASSERT(alpha <= 1);

					int axis2Offset = axis2Index * addressDiffs[1];
					const I_BYTE* linePtr = (firstPixelAddress + axis1Offset + axis2Offset);
					value =	PixelConversion::CalcPixelType(
								conversion.GetCalc(*(const PixelConversion::SourcePixelType*)(linePtr + axis1Offset)) * alpha +
								conversion.GetCalc(*(const PixelConversion::SourcePixelType*)(linePtr + axis1Offset - addressDiffs[1])) * (1 - alpha));
				}
			}
			*(projectionPtr++) = conversion.GetDest(value);
			axis2Position += axis2Delta;
		}

		istd::CRange axis1CutLineRange(projectionLine.GetPoint1().GetX(), projectionLine.GetPoint2().GetX());
		results.SetProportionRangeX(istd::CRange(
					axis1CutLineRange.GetAlphaFromValue(axis1Begin),
					axis1CutLineRange.GetAlphaFromValue(axis1End)));
		results.SetProportionRangeY(istd::CRange(0, 1));

		return true;
	}

	return false;
}


// public methods

bool CLineProjectionProcessor::DoAutosizeProjection(
			const iimg::IBitmap& bitmap,
			const i2d::CLine2d& projectionLine,
			CProjectionData& results) const
{
	istd::CIndex2d axisSizes = bitmap.GetImageSize();
	if (axisSizes.IsSizeEmpty()){
		return false;
	}

	int bytesPerPixel = bitmap.GetPixelBitsCount() >> 3;
	if (bytesPerPixel <= 0){
		return false;
	}

	istd::CIndex2d addressDiffs(bitmap.GetLinesDifference(), bytesPerPixel);
	const I_BYTE* firstPixelAddress = (const I_BYTE*)bitmap.GetLinePtr(0);
	i2d::CLine2d transformedLine = projectionLine;
	i2d::CVector2d diffVector = projectionLine.GetDiffVector();

	if (fabs(diffVector.GetX()) > fabs(diffVector.GetY())){	// switch X and Y axis
		axisSizes = istd::CIndex2d(axisSizes[1], axisSizes[0]);
		addressDiffs = istd::CIndex2d(addressDiffs[1], addressDiffs[0]);
		transformedLine.SetPoint1(i2d::CVector2d(
					transformedLine.GetPoint1().GetY(),
					transformedLine.GetPoint1().GetX()));
		transformedLine.SetPoint2(i2d::CVector2d(
					transformedLine.GetPoint2().GetY(),
					transformedLine.GetPoint2().GetX()));

		diffVector = i2d::CVector2d(diffVector[1], diffVector[0]);
	}

	if (diffVector[0] < 0){	// mirror X axis
		firstPixelAddress += addressDiffs[0] * (axisSizes[0] - 1);
		addressDiffs[0] = -addressDiffs[0];
		transformedLine.SetPoint1(i2d::CVector2d(
					-transformedLine.GetPoint1().GetX(),
					transformedLine.GetPoint1().GetY()));
		transformedLine.SetPoint2(i2d::CVector2d(
					-transformedLine.GetPoint2().GetX(),
					transformedLine.GetPoint2().GetY()));
	}

	i2d::CLine2d clippedLine = projectionLine.GetClipped(i2d::CRectangle(axisSizes));

	iimg::CGrayGrayPixelConversion conversion;
	return ProjectionFunction(
				firstPixelAddress,
				axisSizes,
				addressDiffs,
				transformedLine,
				clippedLine,
				conversion,
				results);
}


// reimplemented (iipr::ILineProjectionProcessor)

bool CLineProjectionProcessor::DoProjection(
			const iimg::IBitmap& bitmap,
			const i2d::CLine2d& projectionLine,
			const IProjectionParams* /*paramsPtr*/,
			CProjectionData& results)
{
	return DoAutosizeProjection(bitmap, projectionLine, results);
}


imath::CVarVector CLineProjectionProcessor::GetBitmapPosition(
			const imath::CVarVector& projectionPosition,
			const iprm::IParamsSet* paramsPtr) const
{
	if ((projectionPosition.GetElementsCount() >= 1) && (paramsPtr != NULL) && !m_lineParamId.empty()){
		const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(paramsPtr->GetParameter(m_lineParamId));
		if (linePtr != NULL){
			return linePtr->GetPositionFromAlpha(projectionPosition[0]);
		}
	}

	return imath::CVarVector();
}


// reimplemented (iproc::TIProcessor)

int CLineProjectionProcessor::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			CProjectionData* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	if ((inputPtr == NULL) || (paramsPtr == NULL) || m_lineParamId.empty()){
		return TS_INVALID;
	}

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(paramsPtr->GetParameter(m_lineParamId));
	if (linePtr == NULL){
		return TS_INVALID;
	}

	return DoAutosizeProjection(*inputPtr, *linePtr, *outputPtr)? TS_OK: TS_INVALID;
}


// reimplemented (iipr::IProjectionConstraints)

istd::CRange CLineProjectionProcessor::GetLineWidthRange() const
{
	return istd::CRange(-1, -1);
}


int CLineProjectionProcessor::GetMinProjectionSize() const
{
	return -1;
}


int CLineProjectionProcessor::GetMaxProjectionSize() const
{
	return -1;
}


bool CLineProjectionProcessor::IsAutoProjectionSizeSupported() const
{
	return true;
}


} // namespace iipr

