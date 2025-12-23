#ifndef iview_CGraphPlotShape_included
#define iview_CGraphPlotShape_included


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <iview/CRectControlledShapeBase.h>
#include <i2d/CGraphData2d.h>


namespace iview
{


/**
	Shape for displaying 2D graph plots with multiple curves.
	This shape observes a CGraphData2d model and renders:
	- Multiple curves with different colors
	- Axis with labels and tick marks
	- Grid (optional)
	- Legend (optional)
	- Title
	
	The shape is interactive and supports rotation, moving, and scaling.
*/
class CGraphPlotShape: public CRectControlledShapeBase
{
public:
	typedef CRectControlledShapeBase BaseClass;

	CGraphPlotShape();

	/**
		Get the margin around the plot area (in pixels).
	*/
	int GetPlotMargin() const;

	/**
		Set the margin around the plot area (in pixels).
	*/
	void SetPlotMargin(int margin);

	/**
		Get the font size for axis labels.
	*/
	int GetAxisLabelFontSize() const;

	/**
		Set the font size for axis labels.
	*/
	void SetAxisLabelFontSize(int size);

	/**
		Get the font size for tick labels.
	*/
	int GetTickLabelFontSize() const;

	/**
		Set the font size for tick labels.
	*/
	void SetTickLabelFontSize(int size);

	/**
		Get line width for curves (in pixels).
	*/
	int GetCurveLineWidth() const;

	/**
		Set line width for curves (in pixels).
	*/
	void SetCurveLineWidth(int width);

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

protected:
	// reimplemented (iview::CRectControlledShapeBase)
	virtual void EnsureValidNodes() const override;
	virtual bool IsCurveTouched(istd::CIndex2d position) const override;
	virtual void DrawFigure(QPainter& drawContext) const override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

private:
	/**
		Draw the plot axes with labels and tick marks.
	*/
	void DrawAxes(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const;

	/**
		Draw the grid lines.
	*/
	void DrawGrid(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const;

	/**
		Draw all curves.
	*/
	void DrawCurves(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const;

	/**
		Draw the legend.
	*/
	void DrawLegend(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const;

	/**
		Draw the title.
	*/
	void DrawTitle(QPainter& drawContext, const QRectF& plotRect, const i2d::CGraphData2d* graphPtr) const;

	/**
		Convert data coordinates to screen coordinates within the plot rect.
	*/
	QPointF DataToScreen(const i2d::CVector2d& dataPoint, const QRectF& plotRect, 
						 const istd::CRange& xRange, const istd::CRange& yRange) const;

	/**
		Calculate nice tick positions for an axis.
	*/
	QVector<double> CalculateTickPositions(const istd::CRange& range, int maxTicks = 10) const;

	/**
		Format a tick label value.
	*/
	QString FormatTickLabel(double value) const;

	int m_plotMargin;
	int m_axisLabelFontSize;
	int m_tickLabelFontSize;
	int m_curveLineWidth;
};


// inline methods

inline int CGraphPlotShape::GetPlotMargin() const
{
	return m_plotMargin;
}


inline void CGraphPlotShape::SetPlotMargin(int margin)
{
	if (m_plotMargin != margin){
		m_plotMargin = margin;
		Invalidate();
	}
}


inline int CGraphPlotShape::GetAxisLabelFontSize() const
{
	return m_axisLabelFontSize;
}


inline void CGraphPlotShape::SetAxisLabelFontSize(int size)
{
	if (m_axisLabelFontSize != size){
		m_axisLabelFontSize = size;
		Invalidate();
	}
}


inline int CGraphPlotShape::GetTickLabelFontSize() const
{
	return m_tickLabelFontSize;
}


inline void CGraphPlotShape::SetTickLabelFontSize(int size)
{
	if (m_tickLabelFontSize != size){
		m_tickLabelFontSize = size;
		Invalidate();
	}
}


inline int CGraphPlotShape::GetCurveLineWidth() const
{
	return m_curveLineWidth;
}


inline void CGraphPlotShape::SetCurveLineWidth(int width)
{
	if (m_curveLineWidth != width){
		m_curveLineWidth = width;
		Invalidate();
	}
}


} // namespace iview


#endif // !iview_CGraphPlotShape_included
