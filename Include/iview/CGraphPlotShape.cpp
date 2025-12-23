#include <iview/CGraphPlotShape.h>


// Qt includes
#include <QtGui/QPainter>
#include <QtCore/qmath.h>

// ACF includes
#include <imod/IModel.h>
#include <iview/IColorSchema.h>
#include <iqt/iqt.h>


namespace iview
{


CGraphPlotShape::CGraphPlotShape()
:	m_plotMargin(60),
	m_axisLabelFontSize(12),
	m_tickLabelFontSize(10),
	m_curveLineWidth(2)
{
}


// reimplemented (iview::IVisualizable)

void CGraphPlotShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CGraphData2d* graphPtr = dynamic_cast<const i2d::CGraphData2d*>(GetObservedModel());
	if (graphPtr == NULL){
		return;
	}

	drawContext.save();

	// Get the client rectangle in screen coordinates
	i2d::CRect clientRect = GetClientRect();
	QRectF screenRect(clientRect.GetLeft(), clientRect.GetTop(), 
					  clientRect.GetWidth(), clientRect.GetHeight());

	// Calculate plot area (inside margins)
	QRectF plotRect = screenRect.adjusted(m_plotMargin, m_plotMargin, 
										  -m_plotMargin, -m_plotMargin);

	// Draw background
	drawContext.fillRect(screenRect, Qt::white);

	// Draw title
	DrawTitle(drawContext, plotRect, graphPtr);

	// Draw grid if enabled
	if (graphPtr->IsGridVisible()){
		DrawGrid(drawContext, plotRect, graphPtr);
	}

	// Draw axes
	DrawAxes(drawContext, plotRect, graphPtr);

	// Draw curves
	DrawCurves(drawContext, plotRect, graphPtr);

	// Draw legend if enabled
	if (graphPtr->IsLegendVisible()){
		DrawLegend(drawContext, plotRect, graphPtr);
	}

	drawContext.restore();
}


// reimplemented (imod::IObserver)

bool CGraphPlotShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass::OnModelAttached(modelPtr, changeMask)){
		const i2d::CGraphData2d* graphPtr = dynamic_cast<const i2d::CGraphData2d*>(modelPtr);
		if (graphPtr != NULL){
			return true;
		}
	}
	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CGraphPlotShape::IsTouched(istd::CIndex2d position) const
{
	Q_ASSERT(IsDisplayConnected());

	i2d::CRect bbox = GetBoundingBox();
	if (bbox.IsInside(position)){
		return TS_INACTIVE;
	}

	return TS_NONE;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CGraphPlotShape::CalcBoundingBox() const
{
	if (IsDisplayConnected()){
		return GetClientRect();
	}
	return i2d::CRect();
}


// private methods

void CGraphPlotShape::DrawTitle(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const
{
	const QString& title = graphPtr->GetTitle();
	if (title.isEmpty()){
		return;
	}

	QFont font = drawContext.font();
	font.setPointSize(m_axisLabelFontSize + 2);
	font.setBold(true);
	drawContext.setFont(font);

	QRectF titleRect(plotRect.left(), plotRect.top() - m_plotMargin + 10, 
					 plotRect.width(), m_plotMargin - 20);
	drawContext.drawText(titleRect, Qt::AlignHCenter | Qt::AlignTop, title);
}


void CGraphPlotShape::DrawAxes(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const
{
	istd::CRange xRange = graphPtr->GetXAxisRange();
	istd::CRange yRange = graphPtr->GetYAxisRange();

	if (!xRange.IsValid() || !yRange.IsValid()){
		return;
	}

	// Draw axis lines
	QPen axisPen(Qt::black, 2);
	drawContext.setPen(axisPen);
	
	// X-axis
	drawContext.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
	// Y-axis
	drawContext.drawLine(plotRect.bottomLeft(), plotRect.topLeft());

	// Draw tick marks and labels
	QFont tickFont = drawContext.font();
	tickFont.setPointSize(m_tickLabelFontSize);
	drawContext.setFont(tickFont);

	QPen tickPen(Qt::black, 1);
	drawContext.setPen(tickPen);

	// X-axis ticks
	QVector<double> xTicks = CalculateTickPositions(xRange);
	for (int i = 0; i < xTicks.count(); ++i){
		double xValue = xTicks[i];
		double xScreen = plotRect.left() + (xValue - xRange.GetMin()) / xRange.GetLength() * plotRect.width();
		
		// Draw tick mark
		drawContext.drawLine(QPointF(xScreen, plotRect.bottom()), 
							QPointF(xScreen, plotRect.bottom() + 5));
		
		// Draw tick label
		QString label = FormatTickLabel(xValue);
		QRectF labelRect(xScreen - 40, plotRect.bottom() + 8, 80, 20);
		drawContext.drawText(labelRect, Qt::AlignHCenter | Qt::AlignTop, label);
	}

	// Y-axis ticks
	QVector<double> yTicks = CalculateTickPositions(yRange);
	for (int i = 0; i < yTicks.count(); ++i){
		double yValue = yTicks[i];
		double yScreen = plotRect.bottom() - (yValue - yRange.GetMin()) / yRange.GetLength() * plotRect.height();
		
		// Draw tick mark
		drawContext.drawLine(QPointF(plotRect.left(), yScreen), 
							QPointF(plotRect.left() - 5, yScreen));
		
		// Draw tick label
		QString label = FormatTickLabel(yValue);
		QRectF labelRect(plotRect.left() - 50, yScreen - 10, 45, 20);
		drawContext.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, label);
	}

	// Draw axis labels
	QFont labelFont = drawContext.font();
	labelFont.setPointSize(m_axisLabelFontSize);
	drawContext.setFont(labelFont);

	const QString& xLabel = graphPtr->GetXAxisLabel();
	if (!xLabel.isEmpty()){
		QRectF xLabelRect(plotRect.left(), plotRect.bottom() + 35, plotRect.width(), 20);
		drawContext.drawText(xLabelRect, Qt::AlignHCenter | Qt::AlignTop, xLabel);
	}

	const QString& yLabel = graphPtr->GetYAxisLabel();
	if (!yLabel.isEmpty()){
		drawContext.save();
		drawContext.translate(plotRect.left() - 45, plotRect.top() + plotRect.height() / 2);
		drawContext.rotate(-90);
		QRectF yLabelRect(-100, -10, 200, 20);
		drawContext.drawText(yLabelRect, Qt::AlignHCenter | Qt::AlignVCenter, yLabel);
		drawContext.restore();
	}
}


void CGraphPlotShape::DrawGrid(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const
{
	istd::CRange xRange = graphPtr->GetXAxisRange();
	istd::CRange yRange = graphPtr->GetYAxisRange();

	if (!xRange.IsValid() || !yRange.IsValid()){
		return;
	}

	QPen gridPen(QColor(220, 220, 220), 1);
	drawContext.setPen(gridPen);

	// Vertical grid lines
	QVector<double> xTicks = CalculateTickPositions(xRange);
	for (int i = 0; i < xTicks.count(); ++i){
		double xValue = xTicks[i];
		double xScreen = plotRect.left() + (xValue - xRange.GetMin()) / xRange.GetLength() * plotRect.width();
		drawContext.drawLine(QPointF(xScreen, plotRect.top()), 
							QPointF(xScreen, plotRect.bottom()));
	}

	// Horizontal grid lines
	QVector<double> yTicks = CalculateTickPositions(yRange);
	for (int i = 0; i < yTicks.count(); ++i){
		double yValue = yTicks[i];
		double yScreen = plotRect.bottom() - (yValue - yRange.GetMin()) / yRange.GetLength() * plotRect.height();
		drawContext.drawLine(QPointF(plotRect.left(), yScreen), 
							QPointF(plotRect.right(), yScreen));
	}
}


void CGraphPlotShape::DrawCurves(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const
{
	istd::CRange xRange = graphPtr->GetXAxisRange();
	istd::CRange yRange = graphPtr->GetYAxisRange();

	if (!xRange.IsValid() || !yRange.IsValid()){
		return;
	}

	// Set clipping to plot area
	drawContext.setClipRect(plotRect);

	int curvesCount = graphPtr->GetCurvesCount();
	for (int i = 0; i < curvesCount; ++i){
		const i2d::CGraphData2d::Curve& curve = graphPtr->GetCurve(i);
		
		if (curve.points.isEmpty()){
			continue;
		}

		QPen curvePen(curve.color, m_curveLineWidth);
		drawContext.setPen(curvePen);

		// Draw lines between consecutive points
		for (int j = 1; j < curve.points.count(); ++j){
			QPointF p1 = DataToScreen(curve.points[j - 1], plotRect, xRange, yRange);
			QPointF p2 = DataToScreen(curve.points[j], plotRect, xRange, yRange);
			drawContext.drawLine(p1, p2);
		}
	}

	drawContext.setClipping(false);
}


void CGraphPlotShape::DrawLegend(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const
{
	int curvesCount = graphPtr->GetCurvesCount();
	if (curvesCount == 0){
		return;
	}

	// Calculate legend size
	int lineHeight = 20;
	int legendHeight = curvesCount * lineHeight + 10;
	int legendWidth = 150;

	// Position legend in top-right corner of plot area
	QRectF legendRect(plotRect.right() - legendWidth - 10, 
					  plotRect.top() + 10, 
					  legendWidth, 
					  legendHeight);

	// Draw legend background
	drawContext.fillRect(legendRect, QColor(255, 255, 255, 200));
	drawContext.setPen(QPen(Qt::black, 1));
	drawContext.drawRect(legendRect);

	// Draw legend entries
	QFont legendFont = drawContext.font();
	legendFont.setPointSize(m_tickLabelFontSize);
	drawContext.setFont(legendFont);

	for (int i = 0; i < curvesCount; ++i){
		const i2d::CGraphData2d::Curve& curve = graphPtr->GetCurve(i);
		
		double yPos = legendRect.top() + 5 + i * lineHeight + lineHeight / 2;
		
		// Draw color line
		QPen linePen(curve.color, 2);
		drawContext.setPen(linePen);
		drawContext.drawLine(QPointF(legendRect.left() + 5, yPos), 
							QPointF(legendRect.left() + 25, yPos));
		
		// Draw curve name
		drawContext.setPen(QPen(Qt::black));
		QRectF textRect(legendRect.left() + 30, yPos - lineHeight / 2, 
						legendWidth - 35, lineHeight);
		drawContext.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, curve.name);
	}
}


QPointF CGraphPlotShape::DataToScreen(const i2d::CVector2d& dataPoint, const QRectF& plotRect, 
									  const istd::CRange& xRange, const istd::CRange& yRange) const
{
	double xNorm = (dataPoint.GetX() - xRange.GetMin()) / xRange.GetLength();
	double yNorm = (dataPoint.GetY() - yRange.GetMin()) / yRange.GetLength();
	
	double xScreen = plotRect.left() + xNorm * plotRect.width();
	double yScreen = plotRect.bottom() - yNorm * plotRect.height();
	
	return QPointF(xScreen, yScreen);
}


QVector<double> CGraphPlotShape::CalculateTickPositions(const istd::CRange& range, int maxTicks) const
{
	QVector<double> ticks;
	
	if (!range.IsValid() || range.GetLength() == 0.0){
		return ticks;
	}

	// Calculate nice tick spacing
	double roughStep = range.GetLength() / (maxTicks - 1);
	double magnitude = qPow(10.0, qFloor(qLn(roughStep) / qLn(10.0)));
	double normalized = roughStep / magnitude;
	
	double niceStep;
	if (normalized < 1.5){
		niceStep = 1.0 * magnitude;
	}
	else if (normalized < 3.0){
		niceStep = 2.0 * magnitude;
	}
	else if (normalized < 7.0){
		niceStep = 5.0 * magnitude;
	}
	else{
		niceStep = 10.0 * magnitude;
	}

	// Generate tick positions
	double firstTick = qCeil(range.GetMin() / niceStep) * niceStep;
	double tick = firstTick;
	
	while (tick <= range.GetMax()){
		ticks.append(tick);
		tick += niceStep;
	}

	return ticks;
}


QString CGraphPlotShape::FormatTickLabel(double value) const
{
	// Format the value with appropriate precision
	if (qAbs(value) < 0.001 && value != 0.0){
		return QString::number(value, 'e', 2);
	}
	else if (qAbs(value) > 10000.0){
		return QString::number(value, 'e', 2);
	}
	else{
		// Remove trailing zeros
		QString str = QString::number(value, 'f', 4);
		while (str.contains('.') && (str.endsWith('0') || str.endsWith('.'))){
			if (str.endsWith('.')){
				str.chop(1);
				break;
			}
			str.chop(1);
		}
		return str;
	}
}


} // namespace iview
