#ifndef iqtnemo_CNemoSensorPlotGuiComp_h_included
#define iqtnemo_CNemoSensorPlotGuiComp_h_included


#include "iqtnemo.h"


#include "qwt_plot.h"
#include "qwt_plot_curve.h"

#include "QtAcf/QtAbstractGuiObserverComp.h"
#include "QtAcf/QtGuiTemplateBasedComponent.h"

#include "Model/AbstractSingleModelObserver.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "inemo/INemoSensor.h"

#include "generatedfiles/ui_CNemoSensorPlotGuiComp.h"


namespace iqtnemo
{


class CNemoSensorPlotGuiComp: public acf::QtAbstractGuiObserverComp<inemo::INemoSensor, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSensorPlotGuiComp> >
{
	Q_OBJECT

public:
	typedef acf::QtAbstractGuiObserverComp<inemo::INemoSensor, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSensorPlotGuiComp> > BaseClass;

	CNemoSensorPlotGuiComp();

	// reimplemented (acf::ObserverInterface)
	virtual bool onAttached(acf::ModelInterface* modelPtr);
	virtual void onDetached(acf::ModelInterface* modelPtr);
	virtual void update(acf::ModelInterface* modelPtr);

	// reimplemented (acf::ModelEditorInterface)
	virtual void updateEditor();
	virtual void updateModel();

protected:
	// reimplemented (acf::QtAbstractGuiComponent)
	virtual void initializeGui();

private:
	virtual void UpdateView();
	virtual void ResetView();

private:
	QwtPlot* m_plotPtr;
	QwtPlotCurve m_plotCurve;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoSensorPlotGuiComp_h_included

