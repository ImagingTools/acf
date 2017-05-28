#include <iview/CLineShape.h>


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include <imod/IModel.h>
#include <i2d/CPosition2d.h>

#include <iqt/iqt.h>

#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


CLineShape::CLineShape()
:	m_lineDisplayMode(LDM_SIMPLE),
	m_arrowLinesProportion(0.4),
	m_maxArrowLineLength(10),
	m_referencePosition(0.0, 0.0),
	m_referenceIndex(-1),
	m_arePointsValid(false)
{
}


int CLineShape::GetLineDisplayMode() const
{
	return m_lineDisplayMode;
}


void CLineShape::SetLineDisplayMode(int mode)
{
	if (mode != m_lineDisplayMode){
		m_lineDisplayMode = mode;

		Invalidate();
	}
}


double CLineShape::GetArrowLinesProportion() const
{
	return m_arrowLinesProportion;
}


void CLineShape::SetArrowLinesProportion(double value)
{
	if (value != m_arrowLinesProportion){
		m_arrowLinesProportion = value;

		Invalidate();
	}
}


double CLineShape::GetMaxArrowLineLength() const
{
	return m_maxArrowLineLength;
}


void CLineShape::SetMaxArrowLineLength(double value)
{
	if (value != m_maxArrowLineLength){
		m_maxArrowLineLength = value;

		Invalidate();
	}
}


// reimplemented (iview::CInteractiveShapeBase)

void CLineShape::InvalidateBoundingBox()
{
	m_arePointsValid = false;

	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CLineShape::IsTouched(istd::CIndex2d position) const
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
		else if (tickerBox.IsInside(position - m_points[0])){
			return TS_TICKER;
		}
	}

	double distance = colorSchema.GetLogicalLineWidth();

	if (CheckLine(m_points[0], m_points[1], position, distance)){
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	if (m_lineDisplayMode == LDM_ARROW){
		if (		CheckLine(m_points[1], m_points[2], position, distance) ||
					CheckLine(m_points[1], m_points[3], position, distance)){
			return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	return TS_NONE;
}


// reimplemented (iview::IVisualizable)

void CLineShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const IColorSchema& colorSchema = GetColorSchema();

	drawContext.save();

	if (IsSelected()){
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
	}
	else{
		drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
	}

	drawContext.drawLine(iqt::GetQPoint(m_points[0]), iqt::GetQPoint(m_points[1]));

	if (m_lineDisplayMode == LDM_ARROW){
		drawContext.drawLine(iqt::GetQPoint(m_points[1]), iqt::GetQPoint(m_points[2]));
		drawContext.drawLine(iqt::GetQPoint(m_points[1]), iqt::GetQPoint(m_points[3]));
	}

	drawContext.restore();

	if (IsSelected() && IsEditablePosition()){
		colorSchema.DrawTicker(drawContext, m_points[0], IColorSchema::TT_MOVE);
		colorSchema.DrawTicker(drawContext, m_points[1], IColorSchema::TT_MOVE);
	}
}


// reimplemented (imod::IObserver)

bool CLineShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CLine2d*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::IMouseActionObserver)

bool CLineShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	BaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(GetObservedModel());
	if (linePtr != NULL){
		EnsurePointsAreValid(*linePtr);

		if (downFlag && IsSelected() && IsEditablePosition()){
			const IColorSchema& colorSchema = GetColorSchema();

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);

			if (tickerBox.IsInside(position - m_points[1])){
				m_referencePosition = linePtr->GetPoint2() - GetLogPosition(position);
				m_referenceIndex = 1;

				BeginTickerDrag();

				return true;
			}
			else if (tickerBox.IsInside(position - m_points[0])){
				m_referencePosition = linePtr->GetPoint1() - GetLogPosition(position);
				m_referenceIndex = 0;

				BeginTickerDrag();

				return true;
			}
		}
	}

	EndTickerDrag();

	return false;
}


bool CLineShape::OnMouseMove(istd::CIndex2d position)
{
	if (!IsEditablePosition()){
		return false;
	}

	imod::IModel* modelPtr = GetObservedModel();

	if (modelPtr != NULL){
		i2d::CLine2d& line = *dynamic_cast<i2d::CLine2d*>(modelPtr);
		Q_ASSERT(&line != NULL);

		switch (m_referenceIndex){
		case 0:
			line.SetPoint1(m_referencePosition + GetLogPosition(position));
			break;

		case 1:
			line.SetPoint2(m_referencePosition + GetLogPosition(position));
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

void CLineShape::CalcPoints(const i2d::CLine2d& line) const
{
	i2d::CLine2d screenLine(
					GetScreenPosition(line.GetPoint1()),
					GetScreenPosition(line.GetPoint2()));

	m_points[0] = screenLine.GetPoint1().ToIndex2d();
	m_points[1] = screenLine.GetPoint2().ToIndex2d();

	if (m_lineDisplayMode == LDM_ARROW){
		i2d::CVector2d delta = screenLine.GetDiffVector() * m_arrowLinesProportion;
		if (delta.GetLength2() > m_maxArrowLineLength * m_maxArrowLineLength){
			delta.Normalize(m_maxArrowLineLength);
		}
		i2d::CVector2d orthogonal = delta.GetOrthogonal();

		m_points[2] = (screenLine.GetPoint2() - delta + orthogonal).ToIndex2d();
		m_points[3] = (screenLine.GetPoint2() - delta - orthogonal).ToIndex2d();
	}
}


void CLineShape::ResetPoints() const
{
	istd::CIndex2d point(-100, -100);

	m_points[0] = point;
	m_points[1] = point;
	m_points[2] = point;
	m_points[3] = point;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CLineShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	EnsurePointsAreValid();

	const IColorSchema& colorSchema = GetColorSchema();

	i2d::CRect boundingBox(m_points[0], ibase::CSize(1, 1));

	int pointsCount = (m_lineDisplayMode == LDM_ARROW)? 4: 2;
	for (int i = 1; i < pointsCount; ++i){
		boundingBox.Union(m_points[i]);
	}

	if (IsSelected() && IsEditablePosition()){
		boundingBox.Expand(colorSchema.GetTickerBox(IColorSchema::TT_MOVE));
	}

	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


// reimplemented (iview::CInteractiveShapeBase)

void CLineShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	m_referencePosition = reference;
}


void CLineShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	if (IsEditablePosition()){
		imod::IModel* modelPtr = GetObservedModel();
		if (modelPtr != NULL){
			i2d::CLine2d& line = *dynamic_cast<i2d::CLine2d*>(modelPtr);
			Q_ASSERT(&line != NULL);

			line.SetPoint1(line.GetPoint1() + position - m_referencePosition);
			line.SetPoint2(line.GetPoint2() + position - m_referencePosition);
			m_referencePosition = position;

			UpdateModelChanges();
		}
	}
}


// static methods

bool CLineShape::CheckLine(
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


