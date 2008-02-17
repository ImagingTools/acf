#include "iqtnemo/CNemoSensorPlotGuiComp.h"

#include <QPainter>

#include "qwt_plot_grid.h"
#include "qwt_scale_widget.h"
#include "qwt_color_map.h"
#include "qwt_plot_layout.h"

#include "QtAcf/QtAcf.h"

#include "Base/SequenceInterface.h"

#include "inemo/INemoSensorData.h"
#include "inemo/INemoSensorSpecification.h"


namespace iqtnemo
{


// reimplemented (acf::ObserverInterface)

bool CNemoSensorPlotGuiComp::onAttached(acf::ModelInterface* modelPtr)
{
	if (BaseClass::onAttached(modelPtr)){
		return true;
	}

	return false;
}


void CNemoSensorPlotGuiComp::onDetached(acf::ModelInterface* modelPtr)
{
	ResetView();

	BaseClass::onDetached(modelPtr);
}


void CNemoSensorPlotGuiComp::update(acf::ModelInterface* modelPtr, int updateFlags, acf::PolymorphicInterface* updateParamsPtr)
{
	UpdateView();
}


// reimplemented (acf::ModelEditorInterface)

void CNemoSensorPlotGuiComp::updateEditor()
{

}


void CNemoSensorPlotGuiComp::updateModel()
{

}


// protected methods

// reimplemented (acf::QtAbstractGuiComponent)

void CNemoSensorPlotGuiComp::initializeGui()
{
	if (m_widget != NULL){
		QLayout* layoutPtr = PlotFrame->layout();

		QwtPlotGrid* gridPtr = new QwtPlotGrid();
		QPen gridPen(QBrush(Qt::lightGray), 0, Qt::DashLine);
		gridPtr->setMajPen(gridPen);

		m_plotPtr = new QwtPlot(PlotFrame);
		m_plotPtr->setCanvasLineWidth(0);
		m_plotPtr->setAxisScale(QwtPlot::yLeft, 0, 100);
		m_plotPtr->plotLayout()->setAlignCanvasToScales(true);
		m_background.attach(m_plotPtr);

		QColor c(Qt::white);
		QColor c2(Qt::darkBlue);
        c.setAlpha(128);
        m_plotCurve.setPen(c2);
		m_plotCurve.setBrush(c);
		m_plotCurve.setRenderHint(QwtPlotItem::RenderAntialiased);

		m_plotCurve.attach(m_plotPtr);
		gridPtr->attach(m_plotPtr);

		if (layoutPtr == NULL){
			layoutPtr = new QGridLayout(PlotFrame);
		}

		layoutPtr->addWidget(m_plotPtr);
	}
}


// private methods

void CNemoSensorPlotGuiComp::UpdateView()
{
	if (m_objectPtr == NULL){
		return;
	}

	istd::CRange sensorRange = m_objectPtr->GetSpecification().GetRange();

	m_background.SetMeasurementRange(m_objectPtr->GetMeasurementRange(), sensorRange);
	m_plotPtr->setAxisScale(QwtPlot::yLeft, sensorRange.GetBottomValue(), sensorRange.GetTopValue());

	const acf::SequenceInterface& measurementData = m_objectPtr->GetData().GetMeasurementData();
	int sequenceSize = measurementData.count();
	if (sequenceSize > 0){
		double* xData = new double [sequenceSize];
		double* yData = new double [sequenceSize];

		for (int i = 0; i < sequenceSize; i++){
			xData[i] = measurementData.x(i);
			yData[i] = measurementData.y(i);
		}

		m_plotCurve.setData(xData, yData, sequenceSize);
		delete [] xData;
		delete [] yData;

		m_plotPtr->replot();
	}
}


void CNemoSensorPlotGuiComp::ResetView()
{
	m_plotCurve.setData(NULL, NULL, 0);

	m_plotPtr->replot();
}


// public methods of embedded class CBackgound

CNemoSensorPlotGuiComp::CBackgroundItem::CBackgroundItem()
{
	setZ(0.0);
}


void CNemoSensorPlotGuiComp::CBackgroundItem::SetMeasurementRange(const imeas::CMeasurementRange& measurementRange, const istd::CRange& sensorRange)
{
	m_measurementRange = measurementRange;

	m_sensorRange = sensorRange;
}


int CNemoSensorPlotGuiComp::CBackgroundItem::rtti() const
{
	return QwtPlotItem::Rtti_PlotUserItem;
}


// reimplemented (QwtPlotItem)

void CNemoSensorPlotGuiComp::CBackgroundItem::draw(QPainter *painter, const QwtScaleMap &, const QwtScaleMap &yMap, const QRect &rect) const
{
	if (m_measurementRange.IsValid()){
		
	}
	
	QColor c(Qt::red);
	QRect r = rect;

	c.setAlpha(192);
	r.setBottom(yMap.transform(m_sensorRange.GetBottomValue()));
	r.setTop(yMap.transform(m_measurementRange.GetLowerErrorLimit()));
	painter->fillRect(r, c);

	QColor c2(Qt::yellow);
	c2.setAlpha(192);
	r.setBottom(yMap.transform(m_measurementRange.GetLowerErrorLimit()));
	r.setTop(yMap.transform(m_measurementRange.GetLowerWarningLimit()));
	painter->fillRect(r, c2);

	QColor c3(Qt::green);
	c3.setAlpha(192);
	r.setBottom(yMap.transform(m_measurementRange.GetLowerWarningLimit()));
	r.setTop(yMap.transform(m_measurementRange.GetUpperWarningLimit()));
	painter->fillRect(r, c3);

	r.setBottom(yMap.transform(m_measurementRange.GetUpperWarningLimit()));
	r.setTop(yMap.transform(m_measurementRange.GetUpperErrorLimit()));
	painter->fillRect(r, c2);

	r.setBottom(yMap.transform(m_measurementRange.GetUpperErrorLimit()));
	r.setTop(yMap.transform(m_sensorRange.GetTopValue()));
	painter->fillRect(r, c);
}


CNemoSensorPlotGuiComp::CNemoSensorPlotGuiComp()
	:BaseClass()
{
}


} // namespace iqtnemo
