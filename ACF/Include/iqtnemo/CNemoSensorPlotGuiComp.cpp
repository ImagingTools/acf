#include "iqtnemo/CNemoSensorPlotGuiComp.h"


#include "qwt_plot_grid.h"

#include "QtAcf/QtAcf.h"

#include "Base/SequenceInterface.h"

#include "inemo/INemoSensor.h"


namespace iqtnemo
{


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


void CNemoSensorPlotGuiComp::update(acf::ModelInterface* modelPtr)
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
		m_plotPtr->setBackgroundColor(Qt::white);

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

	const acf::SequenceInterface& measurementData = m_objectPtr->GetMeasurementData();
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
