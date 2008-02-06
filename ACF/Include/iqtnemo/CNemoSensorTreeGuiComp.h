#ifndef iqtnemo_CNemoSensorTreeGuiComp_h_included
#define iqtnemo_CNemoSensorTreeGuiComp_h_included


#include "iqtnemo.h"


#include "QtAcf/QtAbstractGuiObserverComp.h"
#include "QtAcf/QtGuiTemplateBasedComponent.h"

#include "Model/AbstractSingleModelObserver.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "QtCompLib/QtIconProviderInterface.h"

#include "inemo/INemoSensors.h"

#include "generatedfiles/ui_CNemoSensorTreeGuiComp.h"


namespace iqtnemo
{


class CNemoSensorTreeGuiComp: public acf::QtAbstractGuiObserverComp<inemo::INemoSensors, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSensorTreeGuiComp> >
{
public:
	typedef acf::QtAbstractGuiObserverComp<inemo::INemoSensors, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSensorTreeGuiComp> > BaseClass;

	CNemoSensorTreeGuiComp();

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
	inemo::INemoSensor* m_selectedSensorPtr;

	acf::ComponentDependency<acf::QtIconProviderInterface> m_stateIconsProviderIfPtr;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoSensorTreeGuiComp_h_included

