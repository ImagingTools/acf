#include <iview/CArcShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>
#include <i2d/CArc.h>
#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


CArcShape::CArcShape()
	:	m_editMode(EM_NONE)
{
	m_isEditableRadius = true;
	m_isEditableStartAngle = true;
	m_isEditableAngleWidth = true;
	m_isCenterVisible = true;
}


bool CArcShape::IsEditableRadius() const
{
	return m_isEditableRadius;
}


void CArcShape::SetEditableRadius(bool editable)
{
	if (m_isEditableRadius != editable){
		m_isEditableRadius = editable;
		Invalidate();
	}
}


bool CArcShape::IsEditableStartAngle() const
{
	return m_isEditableStartAngle;
}


void CArcShape::SetEditableStartAngle(bool editable)
{
	if (m_isEditableStartAngle != editable){
		m_isEditableStartAngle = editable;
		Invalidate();
	}
}


bool CArcShape::IsEditableAngleWidth() const
{
	return m_isEditableAngleWidth;
}


void CArcShape::SetEditableAngleWidth(bool editable)
{
	if (m_isEditableAngleWidth != editable){
		m_isEditableAngleWidth = editable;
		Invalidate();
	}
}

bool CArcShape::IsCenterVisible() const
{
	return m_isCenterVisible;
}


void CArcShape::SetCenterVisible(bool state)
{
	if (m_isCenterVisible != state){
		m_isCenterVisible = state;

		Invalidate();
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CArcShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CArc* arcPtr = dynamic_cast<const i2d::CArc*>(GetObservedModel());
	if (arcPtr != NULL){
		if (downFlag && m_isEditableRadius){
			const IColorSchema& colorSchema = GetColorSchema();
			i2d::CVector2d center = arcPtr->GetPosition();
			double radius = arcPtr->GetRadius();
			double startAngle = arcPtr->GetStartAngle();
			double angleWidth = arcPtr->GetEndAngle() - startAngle;

			QLineF line(center.GetX(), center.GetY(), center.GetX()+radius, center.GetY());
			line.setAngle(startAngle);
			i2d::CVector2d startTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
			line.setAngle(startAngle + (angleWidth/2));
			i2d::CVector2d radiusTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
			line.setAngle(arcPtr->GetEndAngle());
			i2d::CVector2d endTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (tickerBox.IsInside(position - radiusTicker.ToIndex2d())){
					m_editMode = EM_RADIUS;

					BeginTickerDrag();

					return true;
			}
			else if(tickerBox.IsInside(position - startTicker.ToIndex2d())){
				m_editMode = EM_STARTANGLE;

				BeginTickerDrag();

				return true;
			}
			else if (tickerBox.IsInside(position - endTicker.ToIndex2d())){
				m_editMode = EM_ANGLEWIDTH;

				BeginTickerDrag();

				return true;
			}
		}
		m_editMode = EM_NONE;

		if (m_isCenterVisible){
			return BaseClass::OnMouseButton(position, buttonType, downFlag);
		}
		else{
			EndTickerDrag();
		}
	}
	return false;
}


bool CArcShape::OnMouseMove(istd::CIndex2d position)
{
	if (m_editMode == EM_RADIUS){
		imod::IModel* modelPtr = GetObservedModel();
		i2d::CArc& arc = *dynamic_cast<i2d::CArc*>(modelPtr);
		Q_ASSERT(&arc != NULL);

		i2d::CVector2d cp = GetLogPosition(position);
		const i2d::CVector2d& center = arc.GetPosition();

		double newRadius = center.GetDistance(cp);

		if (arc.GetRadius() != newRadius){
			arc.SetRadius(newRadius);

			UpdateModelChanges();
		}

		return true;
	}
	else if (m_editMode == EM_STARTANGLE){
		imod::IModel* modelPtr = GetObservedModel();
		i2d::CArc& arc = *dynamic_cast<i2d::CArc*>(modelPtr);
		Q_ASSERT(&arc != NULL);

		i2d::CVector2d cp = GetLogPosition(position);
		const i2d::CVector2d& center = arc.GetPosition();

		QLineF line(center.GetX(), center.GetY(), cp.GetX(), cp.GetY());

		double newStartAngle = line.angle();

		if (arc.GetStartAngle() != newStartAngle){
			arc.SetStartAngle(newStartAngle);

			UpdateModelChanges();
		}

		return true;
	}
	else if (m_editMode == EM_ANGLEWIDTH){
		imod::IModel* modelPtr = GetObservedModel();
		i2d::CArc& arc = *dynamic_cast<i2d::CArc*>(modelPtr);
		Q_ASSERT(&arc != NULL);

		i2d::CVector2d cp = GetLogPosition(position);
		const i2d::CVector2d& center = arc.GetPosition();

		QLineF line(center.GetX(), center.GetY(), cp.GetX(), cp.GetY());

		double newEndAngle = line.angle();

		if (arc.GetEndAngle() != newEndAngle){
			arc.SetEndAngle(newEndAngle);

			UpdateModelChanges();
		}

		return true;
	}

	if (m_isCenterVisible){
		return BaseClass::OnMouseMove(position);
	}

	return false;
}


// reimplemented (iview::IVisualizable)

void CArcShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	if (IsCenterVisible()){
		BaseClass::Draw(drawContext);
	}

	const imod::IModel* modelPtr = GetObservedModel();
	const i2d::CArc& arc = *dynamic_cast<const i2d::CArc*>(modelPtr);
	Q_ASSERT(&arc != NULL);

	const i2d::CVector2d& center = arc.GetPosition();
	double radius = arc.GetRadius();

	i2d::CVector2d screenCenter = GetScreenPosition(center);

	QLineF line(center.GetX(), center.GetY(), center.GetX()+radius, center.GetY());

	line.setAngle(arc.GetStartAngle());
	i2d::CVector2d startTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
	line.setAngle(arc.GetStartAngle() + (arc.GetEndAngle() - arc.GetStartAngle()) / 2);
	i2d::CVector2d radiusTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
	line.setAngle(arc.GetEndAngle());
	i2d::CVector2d endTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));

	double screenRadius = (
		screenCenter.GetDistance(startTicker) +
		screenCenter.GetDistance(radiusTicker) +
		screenCenter.GetDistance(endTicker)) / 3;

	const IColorSchema& colorSchema = GetColorSchema();

	if (IsSelected()){
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
	}
	else{
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
	}

	QBrush emptyBrush(QColor(0, 0, 0), Qt::NoBrush);
	drawContext.setBrush(emptyBrush);

	drawContext.drawArc(QRect(
		QPoint(int(screenCenter.GetX() - screenRadius), int(screenCenter.GetY() - screenRadius)),
		QPoint(int(screenCenter.GetX() + screenRadius), int(screenCenter.GetY() + screenRadius))),
		arc.GetStartAngle() * 16, (arc.GetEndAngle() - arc.GetStartAngle()) * 16);

	if (IsEditableRadius() && IsSelected()){
		colorSchema.DrawTicker(drawContext, startTicker.ToIndex2d(), IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, radiusTicker.ToIndex2d(), IColorSchema::TT_NORMAL);
		colorSchema.DrawTicker(drawContext, endTicker.ToIndex2d(), IColorSchema::TT_NORMAL);
	}
}


// reimplemented (imod::IObserver)

bool CArcShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CArc*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CArcShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CArc* arcPtr = dynamic_cast<const i2d::CArc*>(GetObservedModel());
	if (IsDisplayConnected() && (arcPtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d center = arcPtr->GetPosition();
		double radius = arcPtr->GetRadius();
		double startAngle = arcPtr->GetStartAngle();
		double angleWidth = arcPtr->GetEndAngle() - startAngle;

		i2d::CVector2d screenCenter = GetScreenPosition(center);

		QLineF line(center.GetX(), center.GetY(), center.GetX()+radius, center.GetY());

		line.setAngle(startAngle);
		i2d::CVector2d startTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
		line.setAngle(startAngle + (angleWidth/2));
		i2d::CVector2d radiusTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
		line.setAngle(arcPtr->GetEndAngle());
		i2d::CVector2d endTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));

		double screenRadius = (
			screenCenter.GetDistance(startTicker) +
			screenCenter.GetDistance(radiusTicker) +
			screenCenter.GetDistance(endTicker)) / 3;

		if (m_isEditableRadius && m_isEditableStartAngle && m_isEditableAngleWidth && IsSelected()){
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

			if (			tickerBox.IsInside(position - startTicker.ToIndex2d()) ||
				tickerBox.IsInside(position - radiusTicker.ToIndex2d()) ||
				tickerBox.IsInside(position - endTicker.ToIndex2d())){
					return TS_TICKER;
			}
		}

		double screenDistance = screenCenter.GetDistance(i2d::CVector2d(position));
		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		if (qAbs(screenRadius - screenDistance) < logicalLineWidth){
			bool isEditablePosition = IsEditablePosition();
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
		else{
			if (m_isCenterVisible){
				return BaseClass::IsTouched(position);
			}
		}
	}

	return TS_NONE;
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CArcShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CArc* arcPtr = dynamic_cast<const i2d::CArc*>(GetObservedModel());
	if (arcPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CVector2d& center = arcPtr->GetPosition();
		double radius = arcPtr->GetRadius();
		double startAngle = arcPtr->GetStartAngle();
		double angleWidth = arcPtr->GetEndAngle() - startAngle;

		i2d::CVector2d screenCenter = GetScreenPosition(center);

		QLineF line(center.GetX(), center.GetY(), center.GetX()+radius, center.GetY());

		line.setAngle(startAngle);
		i2d::CVector2d startTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
		line.setAngle(startAngle + (angleWidth / 2));
		i2d::CVector2d radiusTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));
		line.setAngle(arcPtr->GetEndAngle());
		i2d::CVector2d endTicker = GetScreenPosition(i2d::CVector2d(line.p2().x(), line.p2().y()));

		double screenRadius = (
			screenCenter.GetDistance(startTicker) +
			screenCenter.GetDistance(radiusTicker) +
			screenCenter.GetDistance(endTicker)) / 3;

		i2d::CRect boundingBox(
			int(screenCenter.GetX() - screenRadius - 1), int(screenCenter.GetY() - screenRadius - 1),
			int(screenCenter.GetX() + screenRadius + 2), int(screenCenter.GetY() + screenRadius + 2));

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

		boundingBox.Union(tickerBox.GetTranslated(startTicker.ToIndex2d()));
		boundingBox.Union(tickerBox.GetTranslated(radiusTicker.ToIndex2d()));
		boundingBox.Union(tickerBox.GetTranslated(endTicker.ToIndex2d()));

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


}