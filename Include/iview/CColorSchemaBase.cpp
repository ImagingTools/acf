#include <iview/CColorSchemaBase.h>


// ACF includes
#include <iqt/iqt.h>


namespace iview
{


// reimplemented (iview::IColorSchema)

void CColorSchemaBase::Assign(const IColorSchema& colorSchema)
{
	for (int penIndex = IColorSchema::SP_NORMAL; penIndex <= IColorSchema::SP_LAST; ++penIndex){
		SetPen(IColorSchema::StandardPen(penIndex), colorSchema.GetPen(IColorSchema::StandardPen(penIndex)));
	}

	for (int brushIndex = IColorSchema::SB_NORMAL; brushIndex <= IColorSchema::SB_LAST; ++brushIndex){
		SetBrush(IColorSchema::StandardBrush(brushIndex), colorSchema.GetBrush(IColorSchema::StandardBrush(brushIndex)));
	}

	for (int fontIndex = IColorSchema::SF_NORMAL; fontIndex <= IColorSchema::SF_LAST; ++fontIndex){
		SetFont(IColorSchema::StandardFont(fontIndex), colorSchema.GetFont(IColorSchema::StandardFont(fontIndex)));
	}
}


void CColorSchemaBase::DrawTicker(QPainter& drawContext, istd::CIndex2d point, TickerType tickerType) const
{
	const i2d::CRect& tickerBox = s_tickerDrawBoxes[tickerType];
	QRect tickerRect = iqt::GetQRect(tickerBox.GetTranslated(point));

	switch (tickerType){
	case TT_INACTIVE:
	case TT_INACTIVE_SMALL:
		drawContext.save();
		drawContext.setPen(GetPen(SP_TICKER));
		drawContext.drawLine(QPoint(point.GetX() + tickerBox.GetLeft(), point.GetY()), QPoint(point.GetX() + tickerBox.GetRight(), point.GetY()));
		drawContext.drawLine(QPoint(point.GetX(), point.GetY() + tickerBox.GetTop()), QPoint(point.GetX(), point.GetY() + tickerBox.GetBottom()));
		drawContext.restore();
		break;

	case TT_SELECTED_INACTIVE:
	case TT_SELECTED_INACTIVE_SMALL:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.drawLine(QPoint(point.GetX() + tickerBox.GetLeft(), point.GetY()), QPoint(point.GetX() + tickerBox.GetRight(), point.GetY()));
		drawContext.drawLine(QPoint(point.GetX(), point.GetY() + tickerBox.GetTop()), QPoint(point.GetX(), point.GetY() + tickerBox.GetBottom()));
		drawContext.restore();
		break;

	case TT_MOVE:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		drawContext.drawEllipse(tickerRect);
		drawContext.restore();
		break;

	case TT_MOVE_CENTER:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.drawLine(QPoint(point.GetX() + tickerBox.GetLeft(), point.GetY()), QPoint(point.GetX() + tickerBox.GetRight(), point.GetY()));
		drawContext.drawLine(QPoint(point.GetX(), point.GetY() + tickerBox.GetTop()), QPoint(point.GetX(), point.GetY() + tickerBox.GetBottom()));
		drawContext.restore();
		break;

	case TT_INSERT:{
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		drawContext.drawEllipse(tickerRect);

		QRectF insertMarkerRect = QRectF(tickerRect).adjusted(1, 1, -1, -1);
		drawContext.drawLine(QPointF(insertMarkerRect.left(), point.GetY() + 0.5), QPointF(insertMarkerRect.right(), point.GetY() + 0.5));
		drawContext.drawLine(QPointF(point.GetX() + 0.5, insertMarkerRect.top()), QPointF(point.GetX() + 0.5, insertMarkerRect.bottom()));
		drawContext.restore();
		break;
	}

	case TT_DELETE:{
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		drawContext.drawEllipse(tickerRect);

		QRectF deleteMarkerRect = QRectF(tickerRect).adjusted(2.42, 2.42, -2.42, -2.42);
		drawContext.drawLine(QPointF(deleteMarkerRect.left(), deleteMarkerRect.top()), QPointF(deleteMarkerRect.right(), deleteMarkerRect.bottom()));
		drawContext.drawLine(QPointF(deleteMarkerRect.right(), deleteMarkerRect.top()), QPointF(deleteMarkerRect.left(), deleteMarkerRect.bottom()));
		drawContext.restore();
		break;
	}

	case TT_CHECKBOX_ON:
		drawContext.save();
		drawContext.setPen(GetPen(SP_IMPORTANT));
		drawContext.setBrush(GetBrush(SB_IMPORTANT));
		drawContext.drawRect(tickerRect);
		drawContext.restore();
		break;

	case TT_CHECKBOX_OFF:
		drawContext.save();
		drawContext.setPen(GetPen(SP_IMPORTANT));
		drawContext.setBrush(GetBrush(SB_BACKGROUND));
		drawContext.drawRect(tickerRect);
		drawContext.restore();
		break;

	case TT_ROTATE:
		drawContext.save();
		drawContext.setPen(GetPen(SP_TICKER));
		drawContext.setBrush(GetBrush(SB_TICKER));
		drawContext.drawRect(tickerRect);
		drawContext.restore();
		break;

	case TT_SKEW:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.setBrush(GetBrush(SB_TICKER));
		drawContext.drawEllipse(tickerRect);
		drawContext.restore();
		break;

	default:
		drawContext.save();
		drawContext.setPen(GetPen(SP_TICKER));
		drawContext.setBrush(GetBrush(SB_TICKER));
		drawContext.drawEllipse(tickerRect);
		drawContext.restore();
		break;
	}
}


const i2d::CRect& CColorSchemaBase::GetTickerBox(TickerType tickerType) const
{
	Q_ASSERT(tickerType >= 0);
	Q_ASSERT(tickerType <= TT_LAST);

	return s_tickerBoundingBoxes[tickerType];
}


// static attributes

i2d::CRect CColorSchemaBase::s_tickerDrawBoxes[TT_LAST + 1] = {
	i2d::CRect(-SMALL_TICKER_SIZE, -SMALL_TICKER_SIZE, SMALL_TICKER_SIZE + 1, SMALL_TICKER_SIZE + 1),
	i2d::CRect(-SMALL_TICKER_SIZE, -SMALL_TICKER_SIZE, SMALL_TICKER_SIZE + 1, SMALL_TICKER_SIZE + 1),
	i2d::CRect(-SMALL_TICKER_SIZE, -SMALL_TICKER_SIZE, SMALL_TICKER_SIZE + 1, SMALL_TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1),
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1)
};
i2d::CRect CColorSchemaBase::s_tickerBoundingBoxes[TT_LAST + 1] = {
	i2d::CRect(-SMALL_TICKER_SIZE - 1, -SMALL_TICKER_SIZE - 1, SMALL_TICKER_SIZE + 2, SMALL_TICKER_SIZE + 2),
	i2d::CRect(-SMALL_TICKER_SIZE - 1, -SMALL_TICKER_SIZE - 1, SMALL_TICKER_SIZE + 2, SMALL_TICKER_SIZE + 2),
	i2d::CRect(-SMALL_TICKER_SIZE - 1, -SMALL_TICKER_SIZE - 1, SMALL_TICKER_SIZE + 2, SMALL_TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2),
	i2d::CRect(-TICKER_SIZE - 1, -TICKER_SIZE - 1, TICKER_SIZE + 2, TICKER_SIZE + 2)
};


} // namespace iview



