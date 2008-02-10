#include "iqtnemo/CNemoSensorPlotGuiComp.h"

#include <QPainter>

#include "qwt_plot_grid.h"
#include "qwt_scale_widget.h"
#include "qwt_color_map.h"
#include "qwt_plot_layout.h"

#include "QtAcf/QtAcf.h"

#include "Base/SequenceInterface.h"

#include "inemo/INemoSensorData.h"


namespace iqtnemo
{


class Background: public QwtPlotItem
{
public:
    Background()
    {
        setZ(0.0);
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRect &rect) const
    {
        QColor c(Qt::red);
        QRect r = rect;

		c.setAlpha(70);
		r.setBottom(yMap.transform(90));
        r.setTop(yMap.transform(100));
        painter->fillRect(r, c);

        QColor c2(Qt::yellow);
		c2.setAlpha(70);
		r.setBottom(yMap.transform(70));
        r.setTop(yMap.transform(90));
        painter->fillRect(r, c2);

		QColor c3(Qt::green);
		c3.setAlpha(70);
		r.setBottom(yMap.transform(70));
        r.setTop(yMap.transform(30));
        painter->fillRect(r, c3);

		r.setBottom(yMap.transform(30));
        r.setTop(yMap.transform(10));
        painter->fillRect(r, c2);

		r.setBottom(yMap.transform(10));
        r.setTop(yMap.transform(0));
        painter->fillRect(r, c);
	}
};


CNemoSensorPlotGuiComp::CNemoSensorPlotGuiComp()
	:BaseClass()
{
}


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
		Background* background = new Background();
		background->attach(m_plotPtr);

		QColor c(Qt::blue);
		QColor c2(Qt::darkBlue);
        c.setAlpha(30);
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


} // namespace iqtnemo
