#include "iview/CTransformableRectangleCalibrationShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/TModelWrap.h"

#include "i2d/CLine2d.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CTransformableRectangleCalibrationShape::CTransformableRectangleCalibrationShape()
{
	m_isEditableRotation = true;
	m_isEditableWidth = true;
	m_isEditableHeight = true;
	m_isProportionalScaled = false;

	m_areNodesValid = false;
}


// reimplemented (iview::CInteractiveShapeBase)

void CTransformableRectangleCalibrationShape::InvalidateBoundingBox()
{
	m_areNodesValid = false;
	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::IInteractiveShape)

iview::ITouchable::TouchState CTransformableRectangleCalibrationShape::IsTouched(istd::CIndex2d position) const
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

void CTransformableRectangleCalibrationShape::Draw(QPainter& drawContext) const
{
	DrawFigure(drawContext);
	DrawTickers(drawContext);
}


// reimplemented (imod::IObserver)

bool CTransformableRectangleCalibrationShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CParallelogram*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CTransformableRectangleCalibrationShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	if (IsDisplayConnected()){
		m_mouseMode = MM_NONE;

		const IColorSchema& colorSchema = GetColorSchema();

		if (downFlag && IsSelected()){
			EnsureValidNodes();
			const istd::CIndex2d* nodes = GetNodes();
			const i2d::CRect& normalBox = colorSchema.GetTickerBox(IColorSchema::TT_NORMAL);

			if (m_isEditableRotation){
				const i2d::CRect& rotateBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_ROTATE: IColorSchema::TT_INACTIVE);

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


bool CTransformableRectangleCalibrationShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (IsDisplayConnected() && (parallelogramPtr != NULL)){
		const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();
		i2d::CVector2d viewPos = transform.GetClientPosition(position);
		i2d::CVector2d logPos;
		calib.GetPositionAt(viewPos, logPos);

		parallelogramPtr->SetTransform(CalcMoveTransform(logPos, parallTransform).GetApply(parallTransform));

		UpdateModelChanges();

		return true;
	}
	else{
		return false;
	}
}


// protected methods

void CTransformableRectangleCalibrationShape::CalcNodes(const i2d::CAffine2d& parallTransform) const
{
	if (IsDisplayConnected()){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		const i2d::ITransformation2d& calib = GetIsomorphCalib();
		i2d::CVector2d viewPos;

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

		calib.GetInvPositionAt(node1, viewPos);
		m_nodes[EN_NODE1] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(node2, viewPos);
		m_nodes[EN_NODE2] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(node3, viewPos);
		m_nodes[EN_NODE3] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(node4, viewPos);
		m_nodes[EN_NODE4] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(center, viewPos);
		m_nodes[EN_CENTER] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(middle12, viewPos);
		m_nodes[EN_MIDDLE12] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(middle23, viewPos);
		m_nodes[EN_MIDDLE23] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(middle34, viewPos);
		m_nodes[EN_MIDDLE34] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(middle41, viewPos);
		m_nodes[EN_MIDDLE41] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(rotation1, viewPos);
		m_nodes[EN_ROTATION1] = transform.GetScreenPosition(viewPos);
		calib.GetInvPositionAt(rotation2, viewPos);
		m_nodes[EN_ROTATION2] = transform.GetScreenPosition(viewPos);

		m_areNodesValid = true;
	}
}


void CTransformableRectangleCalibrationShape::ResetNodes() const
{
	for (int i = 0; i < EN_LAST; ++i){
		m_nodes[i].Reset();
	}
	m_areNodesValid = true;
}


void CTransformableRectangleCalibrationShape::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetModelPtr();
		const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
		if (parallelogramPtr != NULL){
			const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();
			CalcNodes(parallTransform);
		}
		else{
			ResetNodes();
		}
	}
}


i2d::CAffine2d CTransformableRectangleCalibrationShape::CalcMoveTransform(i2d::CVector2d position, const i2d::CAffine2d& transform)
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


bool CTransformableRectangleCalibrationShape::IsTickerTouched(istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CRect& normalBox = colorSchema.GetTickerBox(IsSelected()?
						IColorSchema::TT_NORMAL:
						IColorSchema::TT_INACTIVE);
		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();

		if (m_isEditableRotation){
			const i2d::CRect& rotateBox = colorSchema.GetTickerBox(IsSelected()?
							IColorSchema::TT_ROTATE:
							IColorSchema::TT_INACTIVE);

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


bool CTransformableRectangleCalibrationShape::IsFigureTouched(istd::CIndex2d position) const
{
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(GetModelPtr());
	if (parallelogramPtr != NULL){
		return IsParallTouched(parallelogramPtr->GetTransform(), position);
	}

	return false;
}


void CTransformableRectangleCalibrationShape::DrawTickers(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();
		const IColorSchema& colorSchema = GetColorSchema();

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
			
			if (m_isEditableRotation){
				drawContext.drawLine(iqt::GetQPoint(nodes[EN_CENTER]), iqt::GetQPoint(nodes[EN_ROTATION1]));
				drawContext.drawLine(iqt::GetQPoint(nodes[EN_CENTER]), iqt::GetQPoint(nodes[EN_ROTATION2]));
				colorSchema.DrawTicker(drawContext, nodes[EN_ROTATION1], IColorSchema::TT_ROTATE);
				colorSchema.DrawTicker(drawContext, nodes[EN_ROTATION2], IColorSchema::TT_ROTATE);
			}

			colorSchema.DrawTicker(drawContext, nodes[EN_CENTER], IColorSchema::TT_SELECTED_INACTIVE);

			if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
				colorSchema.DrawTicker(drawContext, nodes[EN_NODE1], IColorSchema::TT_NORMAL);
				colorSchema.DrawTicker(drawContext, nodes[EN_NODE2], IColorSchema::TT_NORMAL);
				colorSchema.DrawTicker(drawContext, nodes[EN_NODE3], IColorSchema::TT_NORMAL);
				colorSchema.DrawTicker(drawContext, nodes[EN_NODE4], IColorSchema::TT_NORMAL);
			}
			if (!m_isProportionalScaled){
				if (m_isEditableWidth){
					colorSchema.DrawTicker(drawContext, nodes[EN_MIDDLE12], IColorSchema::TT_NORMAL);
					colorSchema.DrawTicker(drawContext, nodes[EN_MIDDLE34], IColorSchema::TT_NORMAL);
				}

				if (m_isEditableHeight){
					colorSchema.DrawTicker(drawContext, nodes[EN_MIDDLE23], IColorSchema::TT_NORMAL);
					colorSchema.DrawTicker(drawContext, nodes[EN_MIDDLE41], IColorSchema::TT_NORMAL);
				}
			}

			drawContext.restore();
		}
	}
}


void CTransformableRectangleCalibrationShape::DrawFigure(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();
		const IColorSchema& colorSchema = GetColorSchema();

		drawContext.save();
		
		if (IsSelected()){
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE1]), iqt::GetQPoint(nodes[EN_NODE2]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE2]), iqt::GetQPoint(nodes[EN_NODE3]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE3]), iqt::GetQPoint(nodes[EN_NODE4]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE4]), iqt::GetQPoint(nodes[EN_NODE1]));

		drawContext.restore();
	}
}


bool CTransformableRectangleCalibrationShape::IsParallTouched(
				const i2d::CAffine2d& /*parallTransform*/,
				istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		EnsureValidNodes();

		const istd::CIndex2d* nodes = GetNodes();
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d screenPosition(position);
		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		i2d::CLine2d line;

		line.SetPoint1(i2d::CVector2d(nodes[EN_NODE1]));
		line.SetPoint2(i2d::CVector2d(nodes[EN_NODE2]));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}
		line.SetPoint1(i2d::CVector2d(nodes[EN_NODE3]));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}
		line.SetPoint2(i2d::CVector2d(nodes[EN_NODE4]));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}
		line.SetPoint1(i2d::CVector2d(nodes[EN_NODE1]));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}

		if (IsSelected()){
			line.SetPoint1(i2d::CVector2d(nodes[EN_CENTER]));
			line.SetPoint2(i2d::CVector2d(nodes[EN_ROTATION1]));
			if (line.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}

			line.SetPoint2(i2d::CVector2d(nodes[EN_ROTATION2]));
			if (line.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}
		}
	}

	return false;
}


i2d::CAffine2d CTransformableRectangleCalibrationShape::CalcScaleTransform(
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
	else{
		return i2d::CAffine2d::GetIdentity();
	}
}


i2d::CAffine2d CTransformableRectangleCalibrationShape::CalcSizeTransform(
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
	else{
		return i2d::CAffine2d::GetIdentity();
	}
}


i2d::CAffine2d CTransformableRectangleCalibrationShape::CalcRotatedTransform(
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
	else{
		return i2d::CAffine2d::GetIdentity();
	}
}


// reimplemented (iview::CInteractiveShapeBase)

void CTransformableRectangleCalibrationShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		m_referencePosition = parallelogramPtr->GetCenter() - reference;
	}
}


void CTransformableRectangleCalibrationShape::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		BeginModelChanges();

		parallelogramPtr->MoveCenterTo(m_referencePosition + position);

		EndModelChanges();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CTransformableRectangleCalibrationShape::CalcBoundingBox() const
{
	if (IsDisplayConnected()){
		EnsureValidNodes();

		const istd::CIndex2d* nodes = GetNodes();
		const IColorSchema& colorSchema = GetColorSchema();

		i2d::CRect boundingBox(nodes[EN_NODE1], nodes[EN_NODE1]);
		for (int nodeIndex = 0; nodeIndex <= EN_LAST; ++nodeIndex){
			boundingBox.Union(nodes[nodeIndex]);
		}

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()?
						IColorSchema::TT_NORMAL:
						IColorSchema::TT_INACTIVE);

		boundingBox.Expand(tickerBox);

		boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


