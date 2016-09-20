#include <iview/CLabelShape.h>


// Qt includes
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>

#include <iqt/iqt.h>

#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


CLabelShape::CLabelShape(TextAlign align, const istd::CIndex2d& offset)
	:m_referenceOffset(0, 0),
	m_textAlign(align),
	m_drawOffset(offset)
{
	m_isPositionVisible = true;
	m_isBackgroundTransparent = true;
	m_isEditableOffset = true;
	m_editMode = EM_NONE;
	m_isAlignFixed = false;
}


// reimplemented (iview::IMouseActionObserver)

bool CLabelShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	Q_ASSERT(IsDisplayConnected());

	ShapeBaseClass::OnMouseButton(position, buttonType, downFlag);

	const i2d::CLabel* labelPtr = dynamic_cast<const i2d::CLabel*>(GetObservedModel());
	if (labelPtr != NULL){
		if (downFlag){
			const IColorSchema& colorSchema = GetColorSchema();

			const i2d::CVector2d& cp = labelPtr->GetPosition();
			istd::CIndex2d sp = GetScreenPosition(cp).ToIndex2d();
			istd::CIndex2d offsetSp = sp + m_drawOffset;

			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_MOVE: IColorSchema::TT_INACTIVE);

			if (IsEditableOffset() && tickerBox.IsInside(position - offsetSp)){
				m_editMode = EM_OFFSET;
				m_referenceOffset = position;
				m_isAlignFixed = true;

				BeginTickerDrag();

				return true;
			}

			if (IsEditablePosition() && tickerBox.IsInside(position - sp)){
				m_editMode = EM_POSITION;
				m_referencePosition = cp - GetLogPosition(position);

				BeginTickerDrag();

				return true;
			}
		}
	}

	EndTickerDrag();

	m_isAlignFixed = false;

	return false; 
}



bool CLabelShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		i2d::CLabel& label = *dynamic_cast<i2d::CLabel*>(modelPtr);
		Q_ASSERT(&label != NULL);

		switch(m_editMode){
		case EM_POSITION:
			if (IsEditablePosition()){
				label.SetPosition(m_referencePosition + GetLogPosition(position));
			}
			UpdateModelChanges();
			break;

		case EM_OFFSET:
			if (IsEditableOffset()){
				m_drawOffset = m_drawOffset + position - m_referenceOffset;
				m_referenceOffset = position;
				Invalidate();
			}
			break;
		default:
			return true;
		}

		return true;
	}

	return false;
}



// reimplemented (iview::IVisualizable)

void CLabelShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CLabel* labelPtr = dynamic_cast<const i2d::CLabel*>(GetObservedModel());
	if (labelPtr != NULL){
		const IColorSchema& colorSchema = GetColorSchema();

		istd::CIndex2d sp = GetScreenPosition(labelPtr->GetPosition()).ToIndex2d();
		istd::CIndex2d offsetSp = sp + m_drawOffset;

		i2d::CRect textBox;
		CalculateTextOriginSize(textBox);
		i2d::CRect expandedBox = textBox.GetExpanded(i2d::CRect(-2, -2, 2, 2));

		drawContext.save();

		if (IsSelected()){
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_SELECTED));
		}
		else{
			drawContext.setPen(colorSchema.GetPen(IColorSchema::SP_NORMAL));
		}

		if (IsBackgroundTransparent()){
			drawContext.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_TRANSPARENT));
			if (IsSelected()){
				drawContext.drawRect(iqt::GetQRect(expandedBox));
			}
		}
		else{
			drawContext.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_NORMAL));
			drawContext.drawRect(iqt::GetQRect(expandedBox));
		}

		//draw text
		drawContext.setFont(colorSchema.GetFont(iview::IColorSchema::SF_NORMAL));
		DrawText(drawContext, textBox.GetLeftTop(), labelPtr->GetText());

		//draw line
		if (IsPositionVisible() && !expandedBox.IsInside(sp)){
			istd::CIndex2d linePoint(sp);
			if (linePoint.GetX() < expandedBox.GetLeft()){
				linePoint.SetX(expandedBox.GetLeft());
			}
			else if (linePoint.GetX() >= expandedBox.GetRight()){
				linePoint.SetX(expandedBox.GetRight() - 1);
			}
			if (linePoint.GetY() < expandedBox.GetTop()){
				linePoint.SetY(expandedBox.GetTop());
			}
			else if (linePoint.GetY() >= expandedBox.GetBottom()){
				linePoint.SetY(expandedBox.GetBottom() - 1);
			}
			drawContext.drawLine(iqt::GetQPoint(sp), iqt::GetQPoint(linePoint));
		}

		drawContext.restore();

		//draw ticker
		if (IsPositionVisible()){
			BaseClass::Draw(drawContext);
			if (IsSelected() && IsEditableOffset()){
				colorSchema.DrawTicker(drawContext, offsetSp, IColorSchema::TT_MOVE);
			}
		}
	}
}


// reimplemented (imod::IObserver)

bool CLabelShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CLabel*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CLabelShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CLabel* labelPtr = dynamic_cast<const i2d::CLabel*>(GetObservedModel());
	if (labelPtr != NULL){
		if (IsPositionVisible()){
			ITouchable::TouchState touchState = BaseClass::IsTouched(position);
			if (touchState != TS_NONE){
				return touchState;
			}

			if (IsSelected() && IsEditableOffset()){
				const IColorSchema& colorSchema = GetColorSchema();
				istd::CIndex2d sp = GetScreenPosition(labelPtr->GetPosition()).ToIndex2d();

				istd::CIndex2d offsetSp = sp + m_drawOffset;

				const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_MOVE);
				if (tickerBox.GetTranslated(offsetSp).IsInside(position))
					return TS_TICKER;
			}
		}

		i2d::CRect textBox;
		CalculateTextOriginSize(textBox);

		if (textBox.IsInside(position)){
			return IsEditablePosition()? TS_DRAGGABLE: TS_INACTIVE;
		}
	}

	return TS_NONE;
}


//	protected methods

void CLabelShape::CalculateTextOriginSize(i2d::CRect& textBox) const
{
	const IDisplay* displayPtr = GetDisplayPtr();
	const i2d::CLabel* labelPtr = dynamic_cast<const i2d::CLabel*>(GetObservedModel());
	if (IsDisplayConnected() && (displayPtr != NULL) && (labelPtr != NULL)){
		const IColorSchema& colorSchema = GetColorSchema();
		const QFont& font = colorSchema.GetFont(iview::IColorSchema::SF_NORMAL);

		istd::CIndex2d sp = GetScreenPosition(labelPtr->GetPosition()).ToIndex2d();

		QFontMetrics metrics(font);
		ibase::CSize textSize = iqt::GetCIndex2d(metrics.size(0, labelPtr->GetText()));
		istd::CIndex2d leftTop;

		const i2d::CRect& clientArea = GetClientRect();

		TextAlign textAlign = m_textAlign;
		
		if (textAlign == TAL_AUTOMATIC){
			istd::CIndex2d newOffset = m_drawOffset;
			if (!m_isAlignFixed){
				int absX = qAbs(newOffset.GetX());
				int absY = qAbs(newOffset.GetY());

				if (			(sp.GetX() + newOffset.GetX() - textSize.GetX() < clientArea.GetLeft()) &&
								(sp.GetX() + absX + textSize.GetX() < clientArea.GetRight())){
					newOffset.SetX(absX);
				}
				else if (		(sp.GetX() + newOffset.GetX() + textSize.GetX() >= clientArea.GetRight()) &&
								(sp.GetX() - absX - textSize.GetX() >= clientArea.GetLeft())){
					newOffset.SetX(-absX);
				}
				if (			(sp.GetY() + newOffset.GetY() - textSize.GetY() < clientArea.GetTop()) &&
								(sp.GetY() + absY + textSize.GetY() < clientArea.GetBottom())){
					newOffset.SetY(absY);
				}
				else if (		(sp.GetY() + newOffset.GetY() + textSize.GetY() >= clientArea.GetBottom()) &&
								(sp.GetY() - absY - textSize.GetY() >= clientArea.GetTop())){
					newOffset.SetY(-absY);
				}
				if (newOffset != m_drawOffset){
					m_drawOffset = newOffset;
				}
			}

			if (newOffset == istd::CIndex2d(0, 0)){
				textAlign = TAL_CENTER;
			}
			else{
				if (newOffset.GetX() > 0){
					if (newOffset.GetY() > 0){
						textAlign = TAL_RIGHT_BOTTOM;
					}
					else{
						textAlign = TAL_RIGHT_TOP;
					}
				}
				else{
					if (newOffset.GetY() > 0){
						textAlign = TAL_LEFT_BOTTOM;
					}
					else{
						textAlign = TAL_LEFT_TOP;
					}
				}
			}
		}

		istd::CIndex2d offsetSp = sp + m_drawOffset;

		switch (textAlign){
			case TAL_CENTER:
				leftTop.SetX(offsetSp.GetX() - textSize.GetX() / 2);
				leftTop.SetY(offsetSp.GetY() - textSize.GetY() / 2);
				break;
			case TAL_LEFT_TOP:
				leftTop.SetX(offsetSp.GetX() - textSize.GetX());
				leftTop.SetY(offsetSp.GetY() - textSize.GetY());
				break;
			case TAL_RIGHT_TOP:
				leftTop.SetX(offsetSp.GetX());
				leftTop.SetY(offsetSp.GetY() - textSize.GetY());
				break;
			case TAL_LEFT_BOTTOM:
				leftTop.SetX(offsetSp.GetX() - textSize.GetX());
				leftTop.SetY(offsetSp.GetY());
				break;
			case TAL_RIGHT_BOTTOM:
				leftTop = offsetSp;
				break;

			default:
				break;
		}

		textBox = i2d::CRect(leftTop, textSize);
	}
	else{
		textBox.Reset();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CLabelShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CLabel* labelPtr = dynamic_cast<const i2d::CLabel*>(GetObservedModel());
	if (labelPtr != NULL){
		i2d::CRect boundingBox = i2d::CRect::GetEmpty();

		CalculateTextOriginSize(boundingBox);
		boundingBox.Expand(i2d::CRect(-3, -3, 3, 3));

		const IColorSchema& colorSchema = GetColorSchema();
		istd::CIndex2d sp = GetScreenPosition(labelPtr->GetPosition()).ToIndex2d();
		istd::CIndex2d offsetSp = sp + m_drawOffset;

		if (IsPositionVisible()){
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IsSelected()? IColorSchema::TT_MOVE: IColorSchema::TT_INACTIVE);
			boundingBox.Union(tickerBox.GetTranslated(sp));
			if (IsEditableOffset())
				boundingBox.Union(tickerBox.GetTranslated(offsetSp));
		}

		boundingBox.Expand(i2d::CRect(istd::CIndex2d(-1, -1), istd::CIndex2d(1, 1)));

		return boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview


