#include "iview/CInteractiveLineShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"
#include "i2d/CPosition2d.h"

#include "iqt/iqt.h"

#include "iview/IColorShema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveLineShape::CInteractiveLineShape()
{
	m_arePointsValid = false;
	m_referenceIndex = -1;
}


// reimplemented (iview::CInteractiveShapeBase)

void CInteractiveLineShape::InvalidateBoundingBox()
{
	m_arePointsValid = false;

	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveLineShape::IsTouched(istd::CIndex2d position) const
{
	I_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	bool isEditablePosition = IsEditablePosition();
	const IColorShema& colorShema = GetColorShema();

	if (IsSelected() && isEditablePosition){

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);

		if (tickerBox.IsInside(position - m_points[1])){
			return TS_TICKER;
		}
		else  if (tickerBox.IsInside(position - m_points[0])){
			return TS_TICKER;
		}
	}
	double distance = colorShema.GetLogicalLineWidth();

	if (CheckLine(m_points[0], m_points[1], position, distance)){
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	return TS_NONE;
}


// reimplemented (iview::IVisualizable)

void CInteractiveLineShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const IColorShema& colorShema = GetColorShema();

	if (IsSelected()){
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
	}
	else{
		drawContext.save();
		drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
	}

	drawContext.drawLine(iqt::GetQPoint(m_points[0]), iqt::GetQPoint(m_points[1]));

	drawContext.restore();

	if (IsSelected() && IsEditablePosition()){
		colorShema.DrawTicker(drawContext, m_points[0], IColorShema::TT_MOVE);
		colorShema.DrawTicker(drawContext, m_points[1], IColorShema::TT_MOVE);
	}
}


// reimplemented (imod::IObserver)

bool CInteractiveLineShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CLine2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveLineShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(GetModelPtr());
	if (linePtr != NULL){
		EnsurePointsAreValid(*linePtr);

		if (downFlag && IsSelected() && IsEditablePosition()){
			const IColorShema& colorShema = GetColorShema();

			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_MOVE);
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			if (tickerBox.IsInside(position - m_points[1])){
				m_referencePosition = linePtr->GetPoint2() - transform.GetClientPosition(position);
				m_referenceIndex = 1;

				BeginModelChanges();
				return true;
			}
			else  if (tickerBox.IsInside(position - m_points[0])){
				m_referencePosition = linePtr->GetPoint1() - transform.GetClientPosition(position);
				m_referenceIndex = 0;

				BeginModelChanges();
				return true;
			}
		}
	}

	EndModelChanges();

	return false;
}


bool CInteractiveLineShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetModelPtr();

	if (modelPtr != NULL){
		i2d::CLine2d& line = *dynamic_cast<i2d::CLine2d*>(modelPtr);
		I_ASSERT(&line != NULL);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		
		switch (m_referenceIndex){
		case 0:
			line.SetPoint1(m_referencePosition + transform.GetClientPosition(position));
			break;

		case 1:
			line.SetPoint2(m_referencePosition + transform.GetClientPosition(position));
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

void CInteractiveLineShape::CalcPoints(const i2d::CLine2d& line) const
{
	const i2d::ICalibration2d* calibrationPtr = line.GetCalibration();

	i2d::CLine2d screenLine(
					GetScreenPosition(line.GetPoint1(), calibrationPtr),
					GetScreenPosition(line.GetPoint2(), calibrationPtr));

	m_points[0] = screenLine.GetPoint1().ToIndex2d();
	m_points[1] = screenLine.GetPoint2().ToIndex2d();
}


void CInteractiveLineShape::ResetPoints() const
{
	istd::CIndex2d point(-100, -100);

	m_points[0] = point;
	m_points[1] = point;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CInteractiveLineShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const IColorShema& colorShema = GetColorShema();

	i2d::CRect boundingBox(m_points[0], ibase::CSize(1, 1));
	for (int i = 1; i < 2; ++i){
		boundingBox.Union(m_points[i]);
	}

	if (IsSelected() && IsEditablePosition()){
		boundingBox.Expand(colorShema.GetTickerBox(IColorShema::TT_MOVE));
	}

	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


// reimplemented (iview::CInteractiveShapeBase)

void CInteractiveLineShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;
}


void CInteractiveLineShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	if (IsEditablePosition()){
		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			i2d::CLine2d& line = *dynamic_cast<i2d::CLine2d*>(modelPtr);
			I_ASSERT(&line != NULL);

			BeginModelChanges();

			line.SetPoint1(line.GetPoint1() + position - m_referencePosition);
			line.SetPoint2(line.GetPoint2() + position - m_referencePosition);
			m_referencePosition = position;

			EndModelChanges();
		}
	}
}


// static methods

bool CInteractiveLineShape::CheckLine(
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


