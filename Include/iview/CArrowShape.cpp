#include "iview/CArrowShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"
#include "i2d/CPosition2d.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CArrowShape::CArrowShape()
{
	m_arrowLinesProportion = 0.4;
	m_maxArrowLines = 10;
	m_arePointsValid = false;
	m_referenceIndex = -1;
}


// reimplemented (iview::CInteractiveShapeBase)

void CArrowShape::InvalidateBoundingBox()
{
	m_arePointsValid = false;
	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CArrowShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	bool isEditablePosition = IsEditablePosition();
	const IColorSchema& colorSchema = GetColorSchema();

	if (IsSelected() && isEditablePosition){

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

		if (tickerBox.IsInside(position - m_points[1])){
			return TS_TICKER;
		}
		else  if (tickerBox.IsInside(position - m_points[0])){
			return TS_TICKER;
		}
	}
	double distance = colorSchema.GetLogicalLineWidth();

	if (			CheckLine(m_points[0], m_points[1], position, distance) ||
					CheckLine(m_points[1], m_points[2], position, distance) ||
					CheckLine(m_points[1], m_points[3], position, distance)){
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	return TS_NONE;
}


// reimplemented (iview::IVisualizable)

void CArrowShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const IColorSchema& colorSchema = GetColorSchema();

	if (IsSelected()){
		drawContext.save();
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
	}
	else{
		drawContext.save();
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
	}

	drawContext.drawLine(iqt::GetQPoint(m_points[0]), iqt::GetQPoint(m_points[1]));
	drawContext.drawLine(iqt::GetQPoint(m_points[1]), iqt::GetQPoint(m_points[2]));
	drawContext.drawLine(iqt::GetQPoint(m_points[1]), iqt::GetQPoint(m_points[3]));

	drawContext.restore();

	if (IsSelected() && IsEditablePosition()){
		colorSchema.DrawTicker(drawContext, m_points[0], IColorSchema::TT_MOVE);
		colorSchema.DrawTicker(drawContext, m_points[1], IColorSchema::TT_MOVE);
	}
}


// reimplemented (imod::IObserver)

bool CArrowShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CLine2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CArrowShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(GetModelPtr());
	if (linePtr != NULL){
		if (downFlag && IsSelected() && IsEditablePosition()){
			const i2d::ICalibration2d* calibrationPtr = linePtr->GetCalibration();

			const IColorSchema& colorSchema = GetColorSchema();

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

			if (tickerBox.IsInside(position - m_points[1])){
				m_referencePosition = linePtr->GetPoint2() - GetLogPosition(position, calibrationPtr);
				m_referenceIndex = 1;

				BeginModelChanges();
				return true;
			}
			else  if (tickerBox.IsInside(position - m_points[0])){
				m_referencePosition = linePtr->GetPoint1() - GetLogPosition(position, calibrationPtr);
				m_referenceIndex = 0;

				BeginModelChanges();
				return true;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CArrowShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();

	if (modelPtr != NULL){
		i2d::CLine2d& line = *dynamic_cast<i2d::CLine2d*>(modelPtr);
		Q_ASSERT(&line != NULL);

		const i2d::ICalibration2d* calibrationPtr = line.GetCalibration();

		switch (m_referenceIndex){
		case 0:
			line.SetPoint1(m_referencePosition + GetLogPosition(position, calibrationPtr));
			break;

		case 1:
			line.SetPoint2(m_referencePosition + GetLogPosition(position, calibrationPtr));
			break;
		}

		UpdateModelChanges();

		return true;
	}
	else{
		return false;
	}
}


// protected methods

void CArrowShape::CalcPoints(const i2d::CLine2d& line, const i2d::ICalibration2d* calibrationPtr) const
{
	i2d::CLine2d screenLine(
					GetScreenPosition(line.GetPoint1(), calibrationPtr),
					GetScreenPosition(line.GetPoint2(), calibrationPtr));
	i2d::CVector2d delta = screenLine.GetDiffVector() * m_arrowLinesProportion;
	if (delta.GetLength2() > m_maxArrowLines * m_maxArrowLines){
		delta.Normalize(m_maxArrowLines);
	}
	i2d::CVector2d orthogonal = delta.GetOrthogonal();

	m_points[0] = screenLine.GetPoint1().ToIndex2d();
	m_points[1] = screenLine.GetPoint2().ToIndex2d();
	m_points[2] = (screenLine.GetPoint2() - delta + orthogonal).ToIndex2d();
	m_points[3] = (screenLine.GetPoint2() - delta - orthogonal).ToIndex2d();
}


void CArrowShape::ResetPoints() const
{
	istd::CIndex2d point(-100, -100);

	m_points[0] = point;
	m_points[1] = point;
	m_points[2] = point;
	m_points[3] = point;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CArrowShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const IColorSchema& colorSchema = GetColorSchema();

	i2d::CRect boundingBox(m_points[0], ibase::CSize(1, 1));
	for (int i = 1; i < 4; ++i){
		boundingBox.Union(m_points[i]);
	}

	if (IsSelected() && IsEditablePosition()){
		boundingBox.Expand(colorSchema.GetTickerBox(IColorSchema::TT_MOVE));
	}

	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


// reimplemented (iview::CInteractiveShapeBase)

void CArrowShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;
}


void CArrowShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	if (IsEditablePosition()){
		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			i2d::CLine2d& line = *dynamic_cast<i2d::CLine2d*>(modelPtr);
			Q_ASSERT(&line != NULL);

			BeginModelChanges();

			line.SetPoint1(line.GetPoint1() + position - m_referencePosition);
			line.SetPoint2(line.GetPoint2() + position - m_referencePosition);
			m_referencePosition = position;

			EndModelChanges();
		}
	}
}


// static methods

bool CArrowShape::CheckLine(
				const istd::CIndex2d& firstPoint,
				const istd::CIndex2d& secondPoint,
				const istd::CIndex2d& point,
				double maxDistance)
{
	i2d::CLine2d line = i2d::CLine2d(i2d::CVector2d(firstPoint), i2d::CVector2d(secondPoint));
	double distance = line.GetDistance(i2d::CVector2d(point));
	return (distance < maxDistance);
}


} // namespace iview


