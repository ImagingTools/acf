#include <iview/CRectControlledShapeBase.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>
#include <i2d/CLine2d.h>
#include <iview/IColorSchema.h>


namespace iview
{


CRectControlledShapeBase::CRectControlledShapeBase()
:	m_mouseMode(MM_NONE),
	m_referencePosition(0.0, 0.0)
{
	m_isEditableRotation = true;
	m_isEditableWidth = true;
	m_isEditableHeight = true;
	m_isProportionalScaled = false;

	m_areNodesValid = false;
}


// reimplemented (iview::CInteractiveShapeBase)

void CRectControlledShapeBase::InvalidateBoundingBox()
{
	m_areNodesValid = false;
	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CRectControlledShapeBase::IsTouched(istd::CIndex2d position) const
{
	if (IsSelected() && IsTickerTouched(position)){
		return TS_TICKER;
	}

	if (IsCurveTouched(position)){
		bool isEditablePosition = IsEditablePosition();
		return isEditablePosition? TS_DRAGGABLE: TS_INACTIVE;
	}

	return TS_NONE;
}


// reimplemented (iview::IShape)

void CRectControlledShapeBase::Draw(QPainter& drawContext) const
{
	DrawFigure(drawContext);
	DrawTickers(drawContext);
}


// reimplemented (iview::IMouseActionObserver)

bool CRectControlledShapeBase::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	if (IsDisplayConnected()){
		ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

		m_mouseMode = MM_NONE;

		if (downFlag && IsSelected()){
			EnsureValidNodes();
			const IColorSchema& colorSchema = GetColorSchema();

			const i2d::CRect& normalBox = colorSchema.GetTickerBox(IColorSchema::TT_NORMAL);

			if (m_isEditableRotation){
				const i2d::CRect& rotateBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_ROTATE: IColorSchema::TT_INACTIVE);

				if (rotateBox.IsInside(position - m_screenNodes[EN_ROTATION1].ToIndex2d())){
					m_mouseMode = MM_ROTATION1;
				}
				else if (rotateBox.IsInside(position - m_screenNodes[EN_ROTATION2].ToIndex2d())){
					m_mouseMode = MM_ROTATION2;
				}
			}
			if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
				if (normalBox.IsInside(position - m_screenNodes[EN_NODE1].ToIndex2d())){
					m_mouseMode = MM_SIZE1;
				}
				else if (normalBox.IsInside(position - m_screenNodes[EN_NODE2].ToIndex2d())){
					m_mouseMode = MM_SIZE2;
				}
				else if (normalBox.IsInside(position - m_screenNodes[EN_NODE3].ToIndex2d())){
					m_mouseMode = MM_SIZE3;
				}
				else if (normalBox.IsInside(position - m_screenNodes[EN_NODE4].ToIndex2d())){
					m_mouseMode = MM_SIZE4;
				}
			}
			if (!m_isProportionalScaled){
				if (m_isEditableWidth){
					if (normalBox.IsInside(position - m_screenNodes[EN_MIDDLE12].ToIndex2d())){
						m_mouseMode = MM_HEIGHT1;
					}
					else if (normalBox.IsInside(position - m_screenNodes[EN_MIDDLE34].ToIndex2d())){
						m_mouseMode = MM_HEIGHT2;
					}
				}
				if (m_isEditableHeight){
					if (normalBox.IsInside(position - m_screenNodes[EN_MIDDLE23].ToIndex2d())){
						m_mouseMode = MM_WIDTH1;
					}
					else if (normalBox.IsInside(position - m_screenNodes[EN_MIDDLE41].ToIndex2d())){
						m_mouseMode = MM_WIDTH2;
					}
				}
			}
		}

		if (m_mouseMode != MM_NONE){
			BeginTickerDrag();

			return true;
		}
	}

	EndTickerDrag();

	return false;
}


// protected methods

void CRectControlledShapeBase::CalcNodes(const i2d::CAffine2d& parallTransform) const
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
	m_screenNodes[EN_NODE1] = GetScreenPosition(node1);
	m_screenNodes[EN_NODE2] = GetScreenPosition(node2);
	m_screenNodes[EN_NODE3] = GetScreenPosition(node3);
	m_screenNodes[EN_NODE4] = GetScreenPosition(node4);
	m_screenNodes[EN_CENTER] = GetScreenPosition(center);
	m_screenNodes[EN_MIDDLE12] = GetScreenPosition(middle12);
	m_screenNodes[EN_MIDDLE23] = GetScreenPosition(middle23);
	m_screenNodes[EN_MIDDLE34] = GetScreenPosition(middle34);
	m_screenNodes[EN_MIDDLE41] = GetScreenPosition(middle41);
	m_screenNodes[EN_ROTATION1] = GetScreenPosition(rotation1);
	m_screenNodes[EN_ROTATION2] = GetScreenPosition(rotation2);

	m_areNodesValid = true;
}


void CRectControlledShapeBase::ResetNodes() const
{
	for (int i = 0; i < EN_LAST; ++i){
		m_screenNodes[i].Reset();
	}
	m_areNodesValid = true;
}


i2d::CAffine2d CRectControlledShapeBase::CalcMoveTransform(i2d::CVector2d position, const i2d::CAffine2d& transform)
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


bool CRectControlledShapeBase::IsTickerTouched(istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CRect& normalBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);
		EnsureValidNodes();

		if (m_isEditableRotation){
			const i2d::CRect& rotateBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_ROTATE: IColorSchema::TT_INACTIVE);

			if (			rotateBox.IsInside(position - m_screenNodes[EN_ROTATION1].ToIndex2d()) ||
							rotateBox.IsInside(position - m_screenNodes[EN_ROTATION2].ToIndex2d())){
				return true;
			}
		}
		if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
			if (			normalBox.IsInside(position - m_screenNodes[EN_NODE1].ToIndex2d()) ||
							normalBox.IsInside(position - m_screenNodes[EN_NODE2].ToIndex2d()) ||
							normalBox.IsInside(position - m_screenNodes[EN_NODE3].ToIndex2d()) ||
							normalBox.IsInside(position - m_screenNodes[EN_NODE4].ToIndex2d())){
				return true;
			}
		}
		if (!m_isProportionalScaled){
			if (m_isEditableWidth){
				if (			normalBox.IsInside(position - m_screenNodes[EN_MIDDLE12].ToIndex2d()) ||
								normalBox.IsInside(position - m_screenNodes[EN_MIDDLE34].ToIndex2d())){
					return true;
				}
			}
			if (m_isEditableHeight){
				if (			normalBox.IsInside(position - m_screenNodes[EN_MIDDLE23].ToIndex2d()) ||
								normalBox.IsInside(position - m_screenNodes[EN_MIDDLE41].ToIndex2d())){
					return true;
				}
			}
		}
	}

	return false;
}


void CRectControlledShapeBase::DrawTickers(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();

		EnsureValidNodes();

		if (IsSelected() && IsEditablePosition()){
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
			if (m_isEditableRotation){
				drawContext.drawLine(m_screenNodes[EN_CENTER], m_screenNodes[EN_ROTATION1]);
				drawContext.drawLine(m_screenNodes[EN_CENTER], m_screenNodes[EN_ROTATION2]);
				colorSchema.DrawTicker(drawContext, m_screenNodes[EN_ROTATION1].ToIndex2d(), IColorSchema::TT_ROTATE);
				colorSchema.DrawTicker(drawContext, m_screenNodes[EN_ROTATION2].ToIndex2d(), IColorSchema::TT_ROTATE);
			}

			colorSchema.DrawTicker(drawContext, m_screenNodes[EN_CENTER].ToIndex2d(), IColorSchema::TT_SELECTED_INACTIVE);

			if ((m_isEditableWidth && m_isEditableHeight) || m_isProportionalScaled){
				colorSchema.DrawTicker(drawContext, m_screenNodes[EN_NODE1].ToIndex2d(), IColorSchema::TT_NORMAL);
				colorSchema.DrawTicker(drawContext, m_screenNodes[EN_NODE2].ToIndex2d(), IColorSchema::TT_NORMAL);
				colorSchema.DrawTicker(drawContext, m_screenNodes[EN_NODE3].ToIndex2d(), IColorSchema::TT_NORMAL);
				colorSchema.DrawTicker(drawContext, m_screenNodes[EN_NODE4].ToIndex2d(), IColorSchema::TT_NORMAL);
			}
			if (!m_isProportionalScaled){
				if (m_isEditableWidth){
					colorSchema.DrawTicker(drawContext, m_screenNodes[EN_MIDDLE12].ToIndex2d(), IColorSchema::TT_NORMAL);
					colorSchema.DrawTicker(drawContext, m_screenNodes[EN_MIDDLE34].ToIndex2d(), IColorSchema::TT_NORMAL);
				}
				if (m_isEditableHeight){
					colorSchema.DrawTicker(drawContext, m_screenNodes[EN_MIDDLE23].ToIndex2d(), IColorSchema::TT_NORMAL);
					colorSchema.DrawTicker(drawContext, m_screenNodes[EN_MIDDLE41].ToIndex2d(), IColorSchema::TT_NORMAL);
				}
			}

			drawContext.restore();
		}
	}
}


void CRectControlledShapeBase::DrawFigure(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();

		EnsureValidNodes();

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		drawContext.drawLine(m_screenNodes[EN_NODE1], m_screenNodes[EN_NODE2]);
		drawContext.drawLine(m_screenNodes[EN_NODE2], m_screenNodes[EN_NODE3]);
		drawContext.drawLine(m_screenNodes[EN_NODE3], m_screenNodes[EN_NODE4]);
		drawContext.drawLine(m_screenNodes[EN_NODE4], m_screenNodes[EN_NODE1]);

		drawContext.restore();
	}
}


bool CRectControlledShapeBase::IsParallTouched(
				const i2d::CAffine2d& parallTransform,
				istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CVector2d& parallPosition = parallTransform.GetTranslation();
		const i2d::CMatrix2d& parallDeform = parallTransform.GetDeformMatrix();

		const i2d::CVector2d& axisX  = parallDeform.GetAxisX();
		const i2d::CVector2d& axisY  = parallDeform.GetAxisY();

		i2d::CVector2d screenPosition(position);

		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

		i2d::CLine2d line(GetScreenPosition(parallPosition), GetScreenPosition(parallPosition + axisX));

		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}
		line.SetPoint1(GetScreenPosition(parallPosition + axisX + axisY));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}
		line.SetPoint2(GetScreenPosition(parallPosition + axisY));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}
		line.SetPoint1(GetScreenPosition(parallPosition));
		if (line.GetDistance(screenPosition) < logicalLineWidth){
			return true;
		}

		if (IsSelected()){
			line.SetPoint1(GetScreenPosition(parallPosition + (axisX + axisY) * 0.5));
			line.SetPoint2(GetScreenPosition(parallPosition + axisX * 0.5 + axisY * 0.25));
			if (line.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}

			line.SetPoint2(GetScreenPosition(parallPosition + axisX * 0.25 + axisY * 0.5));
			if (line.GetDistance(screenPosition) < logicalLineWidth){
				return true;
			}
		}
	}

	return false;
}


i2d::CAffine2d CRectControlledShapeBase::CalcScaleTransform(
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


i2d::CAffine2d CRectControlledShapeBase::CalcSizeTransform(
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


i2d::CAffine2d CRectControlledShapeBase::CalcRotatedTransform(
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

void CRectControlledShapeBase::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetObservedModel();
	const i2d::IObject2d* objectPtr = dynamic_cast<const i2d::IObject2d*>(modelPtr);
	if (objectPtr != NULL){
		m_referencePosition = objectPtr->GetCenter() - reference;
	}
}


void CRectControlledShapeBase::SetLogDragPosition(const i2d::CVector2d& position)
{
	imod::IModel* modelPtr = GetObservedModel();
	i2d::IObject2d* objectPtr = dynamic_cast<i2d::IObject2d*>(modelPtr);
	if (objectPtr != NULL){
		objectPtr->MoveCenterTo(m_referencePosition + position);

		UpdateModelChanges();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CRectControlledShapeBase::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const IColorSchema& colorSchema = GetColorSchema();
	const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

	EnsureValidNodes();

	i2d::CRect boundingBox(m_screenNodes[EN_NODE1].ToIndex2d(), m_screenNodes[EN_NODE1].ToIndex2d());
	for (int nodeIndex = 0; nodeIndex <= EN_LAST; ++nodeIndex){
		boundingBox.Union(m_screenNodes[nodeIndex].ToIndex2d());
	}

	boundingBox.Expand(tickerBox);
	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


} // namespace iview

