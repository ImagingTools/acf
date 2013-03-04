#include "iview/CParallelogramShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CLine2d.h"

#include "iqt/iqt.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CParallelogramShape::CParallelogramShape()
{
	m_isEditableRotation = true;
	m_isEditableWidth = true;
	m_isEditableHeight = true;
	m_isEditableAngle = false;
	m_isProportionalScaled = true;
	m_areAxesEqual = false;
	m_areAxesOrthogonal = false;

	m_areNodesValid = false;
}


// reimplemented (iview::CInteractiveShapeBase)

void CParallelogramShape::InvalidateBoundingBox()
{
	m_areNodesValid = false;

	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CParallelogramShape::IsTouched(istd::CIndex2d position) const
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


// reimplemented (iview::IVisualizable)

void CParallelogramShape::Draw(QPainter& drawContext) const
{
	DrawFigure(drawContext);
	DrawTickers(drawContext);
}


// reimplemented (imod::IObserver)

bool CParallelogramShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CParallelogram*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CParallelogramShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	if (downFlag){
		EnsureValidNodes();
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);
		
		if ((m_isEditableHeight || m_isEditableAngle) && tickerBox.IsInside(position - m_nodes[EN_NODE12].ToIndex2d())){
			m_editNode = EN_NODE12;

			BeginModelChanges();

			return true;
		}
		if ((m_isEditableWidth || m_isEditableAngle) && tickerBox.IsInside(position - m_nodes[EN_NODE21].ToIndex2d())){
			m_editNode = EN_NODE21;

			BeginModelChanges();

			return true;
		}
		if (m_isEditableRotation && tickerBox.IsInside(position - m_nodes[EN_NODE22].ToIndex2d())){
			m_editNode = EN_NODE22;

			BeginModelChanges();

			return true;
		}
	}

	EndModelChanges();

	return false;
}


bool CParallelogramShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();
		const i2d::CMatrix2d& parallDeform= parallTransform.GetDeformMatrix();
		const i2d::CVector2d& parallPos = parallTransform.GetTranslation();

		const i2d::CVector2d& axisX  = parallDeform.GetAxisX();
		const i2d::CVector2d& axisY  = parallDeform.GetAxisY();
		i2d::CVector2d cp = GetLogPosition(position);

		i2d::CVector2d newAxisX = axisX;
		i2d::CVector2d newAxisY = axisY;

		switch(m_editNode){
		case EN_NODE12:
			if (m_isEditableHeight){
				if (m_isEditableAngle){
					newAxisY = cp - parallPos;
				}
				else{
					double axisYLength = axisY.GetLength();
					if (qAbs(axisYLength) > I_EPSILON){
						double newAxisYLength = axisY.GetDotProduct(cp - parallPos) / axisYLength;
						if (qAbs(newAxisYLength) > I_EPSILON * 2){
							newAxisY = i2d::CVector2d(axisY * newAxisYLength / axisYLength);
						}
						else{
							return true;
						}
					}
					else{
						return true;
					}
				}
			}
			else if (m_isEditableAngle){
				newAxisY = (cp - parallPos).GetNormalized(axisY.GetLength());
				if (m_isEditableWidth){
					newAxisX = axisX + axisY - newAxisY;
				}
				else{
				}
			}

			if (m_isProportionalScaled){
				if (qAbs(axisY.GetLength()) > I_EPSILON){
					newAxisX *= newAxisY.GetLength() / axisY.GetLength();
				}
			}
			else if (m_areAxesEqual){
				newAxisX.Normalize(newAxisY.GetLength());
			}
			
			break;

		case EN_NODE21:
			if (m_isEditableWidth){
				if (m_isEditableAngle){
					newAxisX = cp - parallPos;
				}
				else{
					double axisXLength = axisX.GetLength();
					if (qAbs(axisXLength) > I_EPSILON){
						double newAxisXLength = axisX.GetDotProduct(cp - parallPos) / axisXLength;
						if (qAbs(newAxisXLength) > I_EPSILON * 2){
							newAxisX = i2d::CVector2d(axisX * newAxisXLength / axisXLength);
						}
						else{
							return true;
						}
					}
					else{
						return true;
					}
				}
			}
			else if (m_isEditableAngle){
				newAxisX = (cp - parallPos).GetNormalized(axisX.GetLength());
				newAxisY = axisX + axisY - newAxisX;
			}

			if (m_isProportionalScaled){
				if (qAbs(axisX.GetLength()) > I_EPSILON){
					newAxisY *= newAxisX.GetLength() / axisX.GetLength();
				}
			}
			else if (m_areAxesEqual){
				newAxisY.Normalize(newAxisX.GetLength());
			}
			
			break;

		case EN_NODE22:
			if (m_isEditableRotation){
				i2d::CVector2d diagonal = axisX + axisY;
				double diagonalLength = diagonal.GetLength();
				i2d::CVector2d newDiagonal = cp - parallPos;
				double newDiagonalLength = newDiagonal.GetLength();
				if ((diagonalLength > I_EPSILON) && (newDiagonalLength > I_EPSILON * 2)){
					i2d::CVector2d castAxisX = diagonal / diagonalLength;
					i2d::CMatrix2d castDeform(castAxisX, castAxisX.GetOrthogonal());
					i2d::CVector2d newCastAxisX = newDiagonal / newDiagonalLength;
					i2d::CMatrix2d newCastDeform(newCastAxisX, newCastAxisX.GetOrthogonal());
					parallelogramPtr->SetDeformMatrix(newCastDeform.GetMultiplied(castDeform.GetInverted()).GetMultiplied(parallDeform));

					UpdateModelChanges();
				}
			}
			return true;

		case EN_NONE:
		case EN_NODE11:
			break;
		}

		i2d::CMatrix2d newParallDeform(newAxisX, newAxisY);
		parallelogramPtr->SetDeformMatrix(newParallDeform);

		UpdateModelChanges();

		return true;
	}
	else{
		return false;
	}
}


// protected methods

void CParallelogramShape::CalcNodes(const i2d::CAffine2d& parallTransform) const
{
	const i2d::CMatrix2d& parallDeform = parallTransform.GetDeformMatrix();

	const i2d::CVector2d& cp11 = parallTransform.GetTranslation();
	m_nodes[EN_NODE11] = GetScreenPosition(cp11);
	const i2d::CVector2d& cp12 = cp11 + parallDeform.GetAxisY();
	m_nodes[EN_NODE12] = GetScreenPosition(cp12);
	const i2d::CVector2d& cp21 = cp11 + parallDeform.GetAxisX();
	m_nodes[EN_NODE21] = GetScreenPosition(cp21);
	const i2d::CVector2d& cp22 = cp12 + parallDeform.GetAxisX();
	m_nodes[EN_NODE22] = GetScreenPosition(cp22);
}


void CParallelogramShape::ResetNodes() const
{
	m_nodes[EN_NODE11].Reset();
	m_nodes[EN_NODE12].Reset();
	m_nodes[EN_NODE21].Reset();
	m_nodes[EN_NODE22].Reset();

	m_areNodesValid = true;
}


void CParallelogramShape::EnsureValidNodes() const
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


bool CParallelogramShape::IsTickerTouched(istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();
		const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

		EnsureValidNodes();

		if (tickerBox.IsInside(position - m_nodes[EN_NODE12].ToIndex2d())){
			if (m_isEditableHeight || m_isEditableAngle){
				return true;
			}
		}
		if (tickerBox.IsInside(position - m_nodes[EN_NODE21].ToIndex2d())){
			if (m_isEditableWidth || m_isEditableAngle){
				return true;
			}
		}
		if (tickerBox.IsInside(position - m_nodes[EN_NODE22].ToIndex2d())){
			if (m_isEditableRotation){
				return true;
			}
		}
	}

	return false;
}


bool CParallelogramShape::IsFigureTouched(istd::CIndex2d position) const
{
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(GetModelPtr());
	if (IsDisplayConnected() && (parallelogramPtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();

		const i2d::CVector2d& parallPosition = parallelogramPtr->GetCenter();
		const i2d::CMatrix2d& parallDeform = parallelogramPtr->GetDeformMatrix();

		const i2d::CVector2d& axisX  = parallDeform.GetAxisX();
		const i2d::CVector2d& axisY  = parallDeform.GetAxisY();

		i2d::CVector2d cp = GetLogPosition(position);

		const CScreenTransform& transform = GetViewToScreenTransform();
		double proportions = transform.GetDeformMatrix().GetApproxScale();

		double logicalLineWidth = colorSchema.GetLogicalLineWidth();

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
	}

	return false;
}


void CParallelogramShape::DrawTickers(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		const IColorSchema& colorSchema = GetColorSchema();

		EnsureValidNodes();

		if (IsSelected()){
			colorSchema.DrawTicker(drawContext, m_nodes[EN_NODE11].ToIndex2d(), IColorSchema::TT_SELECTED_INACTIVE);
			colorSchema.DrawTicker(drawContext, m_nodes[EN_NODE12].ToIndex2d(), (m_isEditableHeight || m_isEditableAngle)? IColorSchema::TT_NORMAL: IColorSchema::TT_SELECTED_INACTIVE);
			colorSchema.DrawTicker(drawContext, m_nodes[EN_NODE21].ToIndex2d(), (m_isEditableWidth || m_isEditableAngle)? IColorSchema::TT_NORMAL: IColorSchema::TT_SELECTED_INACTIVE);

			if (m_isEditableRotation){
				colorSchema.DrawTicker(drawContext, m_nodes[EN_NODE22].ToIndex2d(), IColorSchema::TT_ROTATE);
			}
		}
	}
}


void CParallelogramShape::DrawFigure(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		EnsureValidNodes();

		const IColorSchema& colorSchema = GetColorSchema();

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		drawContext.drawLine(m_nodes[EN_NODE11], m_nodes[EN_NODE12]);
		drawContext.drawLine(m_nodes[EN_NODE12], m_nodes[EN_NODE22]);
		drawContext.drawLine(m_nodes[EN_NODE22], m_nodes[EN_NODE21]);
		drawContext.drawLine(m_nodes[EN_NODE21], m_nodes[EN_NODE11]);

		drawContext.restore();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CParallelogramShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const IColorSchema& colorSchema = GetColorSchema();
	const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_NORMAL: IColorSchema::TT_INACTIVE);

	EnsureValidNodes();

	i2d::CRect boundingBox(m_nodes[EN_NODE11].ToIndex2d(), m_nodes[EN_NODE11].ToIndex2d());
	for (int nodeIndex = EN_NODE11 + 1; nodeIndex <= EN_LAST; ++nodeIndex){
		boundingBox.Union(m_nodes[nodeIndex].ToIndex2d());
	}

	boundingBox.Expand(tickerBox);

	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


// reimplemented (iview::CInteractiveShapeBase)

void CParallelogramShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		m_referencePosition = parallelogramPtr->GetCenter() - reference;
	}
}


void CParallelogramShape::SetLogDragPosition(const i2d::CVector2d& position)
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


