#include "iview/CInteractiveTransformableRectangleShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "i2d/CLine2d.h"

#include "imod/IModel.h"

#include "iqt/iqt.h"

#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveTransformableRectangleShape::CInteractiveTransformableRectangleShape()
{
	m_isEditableRotation = true;
	m_isEditableWidth = true;
	m_isEditableHeight = true;
	m_isProportionalScaled = false;

	m_areNodesValid = false;
}


// reimplemented (iview::CInteractiveShapeBase)

void CInteractiveTransformableRectangleShape::InvalidateBoundingBox()
{
	m_areNodesValid = false;
	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveTransformableRectangleShape::IsTouched(istd::CIndex2d position) const
{
	if (IsSelected() && IsTickerTouched(position)){
		return TS_TICKER;
	}

	if (IsFigureTouched(position)){
		bool isEditablePosition = IsEditablePosition();
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	return TS_NONE;
}


// reimplemented (iview::IShape)

void CInteractiveTransformableRectangleShape::Draw(QPainter& drawContext) const
{
	DrawFigure(drawContext);
	DrawTickers(drawContext);
}


// reimplemented (imod::IObserver)

bool CInteractiveTransformableRectangleShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CParallelogram*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveTransformableRectangleShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	if (IsDisplayConnected()){
		m_mouseMode = MM_NONE;

		if (downFlag && IsSelected()){
			EnsureValidNodes();
			const istd::CIndex2d* nodes = GetNodes();
			const IColorShema& colorShema = GetColorShema();

			const i2d::CRect& normalBox = colorShema.GetTickerBox(IColorShema::TT_NORMAL);

			if (m_isEditableRotation){
				const i2d::CRect& rotateBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_ROTATE: IColorShema::TT_INACTIVE);

				if (rotateBox.IsInside(position - nodes[EN_ROTATION1])){
					m_mouseMode = MM_ROTATION1;
				}
				else if (rotateBox.IsInside(position - nodes[EN_ROTATION2])){
					m_mouseMode = MM_ROTATION2;
				}
			}
			if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
				if (normalBox.IsInside(position - nodes[EN_NODE1])){
					m_mouseMode = MM_SIZE1;
				}
				else if (normalBox.IsInside(position - nodes[EN_NODE2])){
					m_mouseMode = MM_SIZE2;
				}
				else if (normalBox.IsInside(position - nodes[EN_NODE3])){
					m_mouseMode = MM_SIZE3;
				}
				else if (normalBox.IsInside(position - nodes[EN_NODE4])){
					m_mouseMode = MM_SIZE4;
				}
			}
			if (!m_isProportionalScaled){
				if (m_isEditableWidth){
					if (normalBox.IsInside(position - nodes[EN_MIDDLE12])){
						m_mouseMode = MM_HEIGHT1;
					}
					else if (normalBox.IsInside(position - nodes[EN_MIDDLE34])){
						m_mouseMode = MM_HEIGHT2;
					}
				}
				if (m_isEditableHeight){
					if (normalBox.IsInside(position - nodes[EN_MIDDLE23])){
						m_mouseMode = MM_WIDTH1;
					}
					else if (normalBox.IsInside(position - nodes[EN_MIDDLE41])){
						m_mouseMode = MM_WIDTH2;
					}
				}
			}
		}

		if (m_mouseMode != MM_NONE){
			BeginModelChanges();
			return true;
		}
	}

	EndModelChanges();

	return false;
}


bool CInteractiveTransformableRectangleShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		i2d::CVector2d cp = transform.GetClientPosition(position);

		parallelogramPtr->SetTransform(CalcMoveTransform(cp, parallTransform).GetApply(parallTransform));

		UpdateModelChanges();

		return true;
	}

	return true;
}


// protected methods

void CInteractiveTransformableRectangleShape::CalcNodes(const i2d::CAffine2d& parallTransform) const
{
	const i2d::CMatrix2d& parallDeform = parallTransform.GetDeformMatrix();
	const i2d::CVector2d& axisX = parallDeform.GetAxisX();
	const i2d::CVector2d& axisY = parallDeform.GetAxisY();

	const i2d::CVector2d& node1 = parallTransform.GetTranslation();
	const i2d::CVector2d& node2 = node1 + axisX;
	const i2d::CVector2d& node3 = node2 + axisY;
	const i2d::CVector2d& node4 = node1 + axisY;
	const i2d::CVector2d& center = (node1 + node3) * 0.5;
	const i2d::CVector2d& middle12 = (node1 + node2) * 0.5;
	const i2d::CVector2d& middle23 = (node2 + node3) * 0.5;
	const i2d::CVector2d& middle34 = (node3 + node4) * 0.5;
	const i2d::CVector2d& middle41 = (node4 + node1) * 0.5;
	const i2d::CVector2d& rotation1 = node1 + axisX * 0.5 + axisY * 0.25;
	const i2d::CVector2d& rotation2 = node1 + axisX * 0.25 + axisY * 0.5;
	m_nodes[EN_NODE1] = node1.ToIndex2d();
	m_nodes[EN_NODE2] = node2.ToIndex2d();
	m_nodes[EN_NODE3] = node3.ToIndex2d();
	m_nodes[EN_NODE4] = node4.ToIndex2d();
	m_nodes[EN_CENTER] = center.ToIndex2d();
	m_nodes[EN_MIDDLE12] = middle12.ToIndex2d();
	m_nodes[EN_MIDDLE23] = middle23.ToIndex2d();
	m_nodes[EN_MIDDLE34] = middle34.ToIndex2d();
	m_nodes[EN_MIDDLE41] = middle41.ToIndex2d();
	m_nodes[EN_ROTATION1] = rotation1.ToIndex2d();
	m_nodes[EN_ROTATION2] = rotation2.ToIndex2d();

	m_areNodesValid = true;
}


void CInteractiveTransformableRectangleShape::ResetNodes() const
{
	for (int i = 0; i < EN_LAST; ++i){
		m_nodes[i].Reset();
	}
	m_areNodesValid = true;
}


void CInteractiveTransformableRectangleShape::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetModelPtr();
		const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
		if (parallelogramPtr != NULL){
			const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			CalcNodes(transform.GetApply(parallTransform));
		}
		else{
			ResetNodes();
		}
	}
}


i2d::CAffine2d CInteractiveTransformableRectangleShape::CalcMoveTransform(i2d::CVector2d position, const i2d::CAffine2d& transform)
{
	const i2d::CMatrix2d& parallDeform= transform.GetDeformMatrix();
	const i2d::CVector2d& parallPos = transform.GetTranslation();

	const i2d::CVector2d& axisX  = parallDeform.GetAxisX();
	const i2d::CVector2d& axisY  = parallDeform.GetAxisY();
	i2d::CVector2d center = parallPos + (axisX + axisY) * 0.5;

	switch(m_mouseMode){
	case MM_WIDTH1:
		return CalcScaleTransform(parallPos + axisY * 0.5, parallPos + axisX + axisY * 0.5, position);

	case MM_WIDTH2:
		return CalcScaleTransform(parallPos + axisX + axisY * 0.5, parallPos + axisY * 0.5, position);

	case MM_HEIGHT1:
		return CalcScaleTransform(parallPos + axisY + axisX * 0.5, parallPos + axisX * 0.5, position);

	case MM_HEIGHT2:
		return CalcScaleTransform(parallPos + axisX * 0.5, parallPos + axisY + axisX * 0.5, position);

	case MM_SIZE1:
		return CalcSizeTransform(parallPos + axisX + axisY, parallPos, position);

	case MM_SIZE2:
		return CalcSizeTransform(parallPos + axisY, parallPos + axisX, position);

	case MM_SIZE3:
		return CalcSizeTransform(parallPos, parallPos + axisX + axisY, position);

	case MM_SIZE4:
		return CalcSizeTransform(parallPos + axisX, parallPos + axisY, position);

	case MM_ROTATION1:
		return CalcRotatedTransform(center, parallPos + axisX * 0.5 + axisY * 0.25, position);

	case MM_ROTATION2:
		return CalcRotatedTransform(center, parallPos + axisX * 0.25 + axisY * 0.5, position);

	default:
		return i2d::CAffine2d::GetIdentity();
	}
}


bool CInteractiveTransformableRectangleShape::IsTickerTouched(istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorShema& colorShema = GetColorShema();

		const i2d::CRect& normalBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);
		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();

		if (m_isEditableRotation){
			const i2d::CRect& rotateBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_ROTATE: IColorShema::TT_INACTIVE);

			if (			rotateBox.IsInside(position - nodes[EN_ROTATION1]) ||
							rotateBox.IsInside(position - nodes[EN_ROTATION2])){
				return true;
			}
		}
		if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
			if (			normalBox.IsInside(position - nodes[EN_NODE1]) ||
							normalBox.IsInside(position - nodes[EN_NODE2]) ||
							normalBox.IsInside(position - nodes[EN_NODE3]) ||
							normalBox.IsInside(position - nodes[EN_NODE4])){
				return true;
			}
		}
		if (!m_isProportionalScaled){
			if (m_isEditableWidth){
				if (			normalBox.IsInside(position - nodes[EN_MIDDLE12]) ||
								normalBox.IsInside(position - nodes[EN_MIDDLE34])){
					return true;
				}
			}
			if (m_isEditableHeight){
				if (			normalBox.IsInside(position - nodes[EN_MIDDLE23]) ||
								normalBox.IsInside(position - nodes[EN_MIDDLE41])){
					return true;
				}
			}
		}
	}

	return false;
}


bool CInteractiveTransformableRectangleShape::IsFigureTouched(istd::CIndex2d position) const
{
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(GetModelPtr());
	if (parallelogramPtr != NULL){
		return IsParallTouched(parallelogramPtr->GetTransform(), position);
	}

	return false;
}


void CInteractiveTransformableRectangleShape::DrawTickers(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		const IColorShema& colorShema = GetColorShema();

		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
			if (m_isEditableRotation){
				drawContext.drawLine(iqt::GetQPoint(nodes[EN_CENTER]), iqt::GetQPoint(nodes[EN_ROTATION1]));
				drawContext.drawLine(iqt::GetQPoint(nodes[EN_CENTER]), iqt::GetQPoint(nodes[EN_ROTATION2]));
				colorShema.DrawTicker(drawContext, nodes[EN_ROTATION1], IColorShema::TT_ROTATE);
				colorShema.DrawTicker(drawContext, nodes[EN_ROTATION2], IColorShema::TT_ROTATE);
			}

			colorShema.DrawTicker(drawContext, nodes[EN_CENTER], IColorShema::TT_SELECTED_INACTIVE);

			if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
				colorShema.DrawTicker(drawContext, nodes[EN_NODE1], IColorShema::TT_NORMAL);
				colorShema.DrawTicker(drawContext, nodes[EN_NODE2], IColorShema::TT_NORMAL);
				colorShema.DrawTicker(drawContext, nodes[EN_NODE3], IColorShema::TT_NORMAL);
				colorShema.DrawTicker(drawContext, nodes[EN_NODE4], IColorShema::TT_NORMAL);
			}
			if (!m_isProportionalScaled){
				if (m_isEditableWidth){
					colorShema.DrawTicker(drawContext, nodes[EN_MIDDLE12], IColorShema::TT_NORMAL);
					colorShema.DrawTicker(drawContext, nodes[EN_MIDDLE34], IColorShema::TT_NORMAL);
				}
				if (m_isEditableHeight){
					colorShema.DrawTicker(drawContext, nodes[EN_MIDDLE23], IColorShema::TT_NORMAL);
					colorShema.DrawTicker(drawContext, nodes[EN_MIDDLE41], IColorShema::TT_NORMAL);
				}
			}

			drawContext.restore();
		}
	}
}


void CInteractiveTransformableRectangleShape::DrawFigure(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		const IColorShema& colorShema = GetColorShema();

		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		}

		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE1]), iqt::GetQPoint(nodes[EN_NODE2]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE2]), iqt::GetQPoint(nodes[EN_NODE3]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE3]), iqt::GetQPoint(nodes[EN_NODE4]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE4]), iqt::GetQPoint(nodes[EN_NODE1]));

		drawContext.restore();
	}
}


bool CInteractiveTransformableRectangleShape::IsParallTouched(
				const i2d::CAffine2d& parallTransform,
				istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorShema& colorShema = GetColorShema();

		const i2d::CVector2d& parallPosition = parallTransform.GetTranslation();
		const i2d::CMatrix2d& parallDeform = parallTransform.GetDeformMatrix();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		const i2d::CVector2d& axisX  = parallDeform.GetAxisX();
		const i2d::CVector2d& axisY  = parallDeform.GetAxisY();

		i2d::CVector2d cp = transform.GetClientPosition(position);
		double proportions = ::sqrt(transform.GetDeformMatrix().GetDet());

		double logicalLineWidth = colorShema.GetLogicalLineWidth();

		i2d::CLine2d line(parallPosition, parallPosition + axisX);

		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return true;
		}
		line.SetPoint1(parallPosition + axisX + axisY);
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return true;
		}
		line.SetPoint2(parallPosition + axisY);
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return true;
		}
		line.SetPoint1(parallPosition);
		if (proportions * line.GetDistance(cp) < logicalLineWidth){
			return true;
		}

		if (IsSelected()){
			line.SetPoint1(parallPosition + (axisX + axisY) * 0.5);
			line.SetPoint2(parallPosition + axisX * 0.5 + axisY * 0.25);
			if (proportions * line.GetDistance(cp) < logicalLineWidth){
				return true;
			}

			line.SetPoint2(parallPosition + axisX * 0.25 + axisY * 0.5);
			if (proportions * line.GetDistance(cp) < logicalLineWidth){
				return true;
			}
		}
	}

	return false;
}


i2d::CAffine2d CInteractiveTransformableRectangleShape::CalcScaleTransform(
				const i2d::CVector2d& center,
				const i2d::CVector2d& isPos,
				const i2d::CVector2d& shouldPos)
{
	i2d::CVector2d isDelta = isPos - center;
	double isLength2 = isDelta.GetLength2();

	if (isLength2 > I_EPSILON){
		i2d::CVector2d shouldDelta = shouldPos - center;
		i2d::CVector2d isNormalized = isDelta / ::sqrt(isLength2);
		double proportion = shouldDelta.GetDotProduct(isDelta) / isLength2;
		i2d::CMatrix2d isDeform(isNormalized, isNormalized.GetOrthogonal());
		i2d::CMatrix2d shouldDeform(
						isNormalized * proportion, 
						m_isProportionalScaled? isNormalized.GetOrthogonal() * proportion: isNormalized.GetOrthogonal());
		i2d::CMatrix2d rotation(shouldDeform.GetMultiplied(isDeform.GetInverted()));
		i2d::CAffine2d rotTransform(rotation, center - rotation.GetMultiplied(center));
		return rotTransform;
	}
	
	return i2d::CAffine2d::GetIdentity();
}


i2d::CAffine2d CInteractiveTransformableRectangleShape::CalcSizeTransform(
				const i2d::CVector2d& center,
				const i2d::CVector2d& isPos,
				const i2d::CVector2d& shouldPos)
{
	double isDistance = isPos.GetDistance(center);
	double shouldDistance = shouldPos.GetDistance(center);

	if ((isDistance > I_EPSILON) && (shouldDistance > I_EPSILON)){
		double scale = shouldDistance / isDistance;

		i2d::CMatrix2d complexDeform;
		complexDeform.Reset(0, scale);

		i2d::CAffine2d sizeTransform(complexDeform, center - complexDeform.GetMultiplied(center));
		return sizeTransform;
	}

	return i2d::CAffine2d::GetIdentity();
}


i2d::CAffine2d CInteractiveTransformableRectangleShape::CalcRotatedTransform(
				const i2d::CVector2d& center,
				const i2d::CVector2d& isPos,
				const i2d::CVector2d& shouldPos)
{
	i2d::CVector2d isDelta = isPos - center;
	i2d::CVector2d shouldDelta = shouldPos - center;
	if ((isDelta.GetLength() > I_EPSILON) && (shouldDelta.GetLength() > I_EPSILON)){
		i2d::CVector2d isVector = isDelta.GetNormalized();
		i2d::CMatrix2d isDeform(isVector, isVector.GetOrthogonal());
		i2d::CVector2d shouldVector = shouldDelta.GetNormalized();
		i2d::CMatrix2d shouldDeform(shouldVector, shouldVector.GetOrthogonal());
		i2d::CMatrix2d rotation(shouldDeform.GetMultiplied(isDeform.GetInverted()));
		i2d::CAffine2d rotTransform(rotation, center - rotation.GetMultiplied(center));
		return rotTransform;
	}

	return i2d::CAffine2d::GetIdentity();
}


// reimplemented (iview::CInteractiveShapeBase)

i2d::CRect CInteractiveTransformableRectangleShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

    const IColorShema& colorShema = GetColorShema();
	const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

	EnsureValidNodes();
	const istd::CIndex2d* nodes = GetNodes();

	i2d::CRect boundingBox(nodes[EN_NODE1], nodes[EN_NODE1]);
	for (int nodeIndex = 0; nodeIndex <= EN_LAST; ++nodeIndex){
		boundingBox.Union(nodes[nodeIndex]);
	}

	boundingBox.Expand(tickerBox);
	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


void CInteractiveTransformableRectangleShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		m_referencePosition = parallelogramPtr->GetCenter() - reference;
	}
}


void CInteractiveTransformableRectangleShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		BeginModelChanges();

		parallelogramPtr->MoveCenterTo(m_referencePosition + position);

		EndModelChanges();
	}
}


} // namespace iview

