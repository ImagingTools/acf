#include "iview/CInteractiveParallelogramShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "imod/IModel.h"

#include "i2d/CLine2d.h"

#include "iqt/iqt.h"

#include "iview/CScreenTransform.h"


namespace iview
{


CInteractiveParallelogramShape::CInteractiveParallelogramShape()
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

void CInteractiveParallelogramShape::InvalidateBoundingBox()
{
	m_areNodesValid = false;

	BaseClass::InvalidateBoundingBox();
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveParallelogramShape::IsTouched(istd::CIndex2d position) const
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

void CInteractiveParallelogramShape::Draw(QPainter& drawContext) const
{
	DrawFigure(drawContext);
	DrawTickers(drawContext);
}


// reimplemented (imod::IObserver)

bool CInteractiveParallelogramShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CParallelogram*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveParallelogramShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton /*buttonType*/, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	if (downFlag){
		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();
        const IColorShema& colorShema = GetColorShema();

		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);
		
		if ((m_isEditableHeight || m_isEditableAngle) && tickerBox.IsInside(position - nodes[EN_NODE12])){
			m_editNode = EN_NODE12;

			BeginModelChanges();

			return true;
		}
		if ((m_isEditableWidth || m_isEditableAngle) && tickerBox.IsInside(position - nodes[EN_NODE21])){
			m_editNode = EN_NODE21;

			BeginModelChanges();

			return true;
		}
		if (m_isEditableRotation && tickerBox.IsInside(position - nodes[EN_NODE22])){
			m_editNode = EN_NODE22;

			BeginModelChanges();

			return true;
		}
	}

	EndModelChanges();

	return false;
}


bool CInteractiveParallelogramShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetModelPtr();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();
		const i2d::CMatrix2d& parallDeform= parallTransform.GetDeformMatrix();
		const i2d::CVector2d& parallPos = parallTransform.GetTranslation();

		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		
		const i2d::CVector2d& axisX  = parallDeform.GetAxisX();
		const i2d::CVector2d& axisY  = parallDeform.GetAxisY();
		i2d::CVector2d cp = transform.GetClientPosition(position);

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

void CInteractiveParallelogramShape::CalcNodes(const i2d::CAffine2d& parallTransform) const
{
		const i2d::CMatrix2d& parallDeform= parallTransform.GetDeformMatrix();
		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		const i2d::CVector2d& cp11 = parallTransform.GetTranslation();
		m_nodes[EN_NODE11] = transform.GetScreenPosition(cp11);
		const i2d::CVector2d& cp12 = cp11 + parallDeform.GetAxisY();
		m_nodes[EN_NODE12] = transform.GetScreenPosition(cp12);
		const i2d::CVector2d& cp21 = cp11 + parallDeform.GetAxisX();
		m_nodes[EN_NODE21] = transform.GetScreenPosition(cp21);
		const i2d::CVector2d& cp22 = cp12 + parallDeform.GetAxisX();
		m_nodes[EN_NODE22] = transform.GetScreenPosition(cp22);
}


void CInteractiveParallelogramShape::ResetNodes() const
{
	m_nodes[EN_NODE11].Reset();
	m_nodes[EN_NODE12].Reset();
	m_nodes[EN_NODE21].Reset();
	m_nodes[EN_NODE22].Reset();

	m_areNodesValid = true;
}


void CInteractiveParallelogramShape::EnsureValidNodes() const
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


bool CInteractiveParallelogramShape::IsTickerTouched(istd::CIndex2d position) const
{
	if (IsDisplayConnected()){
		const IColorShema& colorShema = GetColorShema();
		const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();

		if (tickerBox.IsInside(position - nodes[EN_NODE12])){
			if (m_isEditableHeight || m_isEditableAngle){
				return true;
			}
		}
		if (tickerBox.IsInside(position - nodes[EN_NODE21])){
			if (m_isEditableWidth || m_isEditableAngle){
				return true;
			}
		}
		if (tickerBox.IsInside(position - nodes[EN_NODE22])){
			if (m_isEditableRotation){
				return true;
			}
		}
	}

	return false;
}


bool CInteractiveParallelogramShape::IsFigureTouched(istd::CIndex2d position) const
{
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(GetModelPtr());
	if (IsDisplayConnected() && (parallelogramPtr != NULL)){
		const IColorShema& colorShema = GetColorShema();

		const i2d::CVector2d& parallPosition = parallelogramPtr->GetCenter();
		const i2d::CMatrix2d& parallDeform = parallelogramPtr->GetDeformMatrix();
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
	}

	return false;
}


void CInteractiveParallelogramShape::DrawTickers(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		const IColorShema& colorShema = GetColorShema();

		EnsureValidNodes();
		const istd::CIndex2d* nodes = GetNodes();

		if (IsSelected()){
			colorShema.DrawTicker(drawContext, nodes[EN_NODE11], IColorShema::TT_SELECTED_INACTIVE);
			colorShema.DrawTicker(drawContext, nodes[EN_NODE12], (m_isEditableHeight || m_isEditableAngle)? IColorShema::TT_NORMAL: IColorShema::TT_SELECTED_INACTIVE);
			colorShema.DrawTicker(drawContext, nodes[EN_NODE21], (m_isEditableWidth || m_isEditableAngle)? IColorShema::TT_NORMAL: IColorShema::TT_SELECTED_INACTIVE);
			if (m_isEditableRotation){
				colorShema.DrawTicker(drawContext, nodes[EN_NODE22], IColorShema::TT_ROTATE);
			}
		}
	}
}


void CInteractiveParallelogramShape::DrawFigure(QPainter& drawContext) const
{
	if (IsDisplayConnected()){
		EnsureValidNodes();

		const IColorShema& colorShema = GetColorShema();
		const istd::CIndex2d* nodes = GetNodes();

		if (IsSelected()){
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_SELECTED));
		}
		else{
			drawContext.save();
			drawContext.setPen(colorShema.GetPen(IColorShema::SP_NORMAL));
		}

		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE11]), iqt::GetQPoint(nodes[EN_NODE12]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE12]), iqt::GetQPoint(nodes[EN_NODE22]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE22]), iqt::GetQPoint(nodes[EN_NODE21]));
		drawContext.drawLine(iqt::GetQPoint(nodes[EN_NODE21]), iqt::GetQPoint(nodes[EN_NODE11]));

		drawContext.restore();
	}
}


// reimplemented (iview::CInteractiveShapeBase)

i2d::CRect CInteractiveParallelogramShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const IColorShema& colorShema = GetColorShema();
	const i2d::CRect& tickerBox = colorShema.GetTickerBox(IsSelected()? IColorShema::TT_NORMAL: IColorShema::TT_INACTIVE);

	EnsureValidNodes();
	const istd::CIndex2d* nodes = GetNodes();

	i2d::CRect boundingBox(nodes[EN_NODE11], nodes[EN_NODE11]);
	for (int nodeIndex = EN_NODE11 + 1; nodeIndex <= EN_LAST; ++nodeIndex){
		boundingBox.Union(nodes[nodeIndex]);
	}

	boundingBox.Expand(tickerBox);

	boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

	return boundingBox;
}


void CInteractiveParallelogramShape::BeginLogDrag(const i2d::CVector2d& reference)
{
	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		m_referencePosition = parallelogramPtr->GetCenter() - reference;
	}
}


void CInteractiveParallelogramShape::SetLogDragPosition(const i2d::CVector2d& position)
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


