#include "iview/CColorSchemaBase.h"


// ACF includes
#include "iqt/iqt.h"


namespace iview
{


void CColorSchemaBase::DrawTicker(QPainter& drawContext, istd::CIndex2d point, TickerType tickerType) const
{
	const i2d::CRect& tickerBox = GetTickerBox(tickerType);
	i2d::CRect tickerRect;

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
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		drawContext.drawRect(iqt::GetQRect(tickerBox.GetTranslated(point)));
		drawContext.drawPoint(iqt::GetQPoint(point));
		drawContext.restore();
		drawContext.restore();
		break;

	case TT_MOVE_CENTER:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.drawLine(QPoint(point.GetX() + tickerBox.GetLeft(), point.GetY()), QPoint(point.GetX() + tickerBox.GetRight(), point.GetY()));
		drawContext.drawLine(QPoint(point.GetX(), point.GetY() + tickerBox.GetTop()), QPoint(point.GetX(), point.GetY() + tickerBox.GetBottom()));
		drawContext.restore();
		break;

	case TT_INSERT:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		tickerRect = tickerBox.GetTranslated(point);
		drawContext.drawRect(iqt::GetQRect(tickerRect));
		drawContext.drawLine(QPoint(tickerRect.GetLeft(), point.GetY()), QPoint(tickerRect.GetRight(), point.GetY()));
		drawContext.drawLine(QPoint(point.GetX(), tickerRect.GetTop()), QPoint(point.GetX(), tickerRect.GetBottom()));
		drawContext.restore();
		drawContext.restore();
		break;

	case TT_DELETE:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		tickerRect = tickerBox.GetTranslated(point);
		drawContext.drawRect(iqt::GetQRect(tickerRect));
		drawContext.drawLine(QPoint(tickerRect.GetLeft(), tickerRect.GetTop()), QPoint(tickerRect.GetRight() - 1, tickerRect.GetBottom() - 1));
		drawContext.drawLine(QPoint(tickerRect.GetRight() - 1, tickerRect.GetTop()), QPoint(tickerRect.GetLeft(), tickerRect.GetBottom() - 1));
		drawContext.restore();
		drawContext.restore();
		break;

	case TT_ROTATE:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		tickerRect = tickerBox.GetTranslated(point);
		drawContext.drawEllipse(iqt::GetQRect(tickerRect));
		drawContext.restore();
		drawContext.restore();
		break;

	case TT_SKEW:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.drawLine(QPoint(point.GetX() + tickerBox.GetLeft(), point.GetY() + tickerBox.GetTop()), QPoint(point.GetX() + tickerBox.GetRight(), point.GetY() + tickerBox.GetBottom()));
		drawContext.drawLine(QPoint(point.GetX() + tickerBox.GetRight() - 1, point.GetY() + tickerBox.GetTop()), QPoint(point.GetX() + tickerBox.GetLeft() - 1, point.GetY() + tickerBox.GetBottom()));
		drawContext.restore();
		break;

	case TT_CHECKBOX_ON:
		drawContext.save();
		drawContext.setPen(GetPen(SP_IMPORTANT));
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_IMPORTANT));
		tickerRect = tickerBox.GetTranslated(point);
		drawContext.drawRect(iqt::GetQRect(tickerRect));
		drawContext.restore();
		drawContext.restore();
		break;

	case TT_CHECKBOX_OFF:
		drawContext.save();
		drawContext.setPen(GetPen(SP_IMPORTANT));
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		tickerRect = tickerBox.GetTranslated(point);
		drawContext.drawRect(iqt::GetQRect(tickerRect));
		drawContext.restore();
		drawContext.restore();
		break;

	default:
		drawContext.save();
		drawContext.setPen(GetPen(SP_SELECTED_TICKER));
		drawContext.save();
		drawContext.setBrush(GetBrush(SB_SELECTED_TICKER));
		drawContext.drawRect(iqt::GetQRect(tickerBox.GetTranslated(point)));
		drawContext.restore();
		drawContext.restore();
		break;
	}
}


const i2d::CRect& CColorSchemaBase::GetTickerBox(TickerType tickerType) const
{
	Q_ASSERT(tickerType >= 0);
	Q_ASSERT(tickerType <= TT_LAST);

	return s_tickersBoxes[tickerType];
}


// static attributes

i2d::CRect CColorSchemaBase::s_tickersBoxes[TT_LAST + 1] = {
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
	i2d::CRect(-TICKER_SIZE, -TICKER_SIZE, TICKER_SIZE + 1, TICKER_SIZE + 1)
};


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


} // namespace iview



