#ifndef iqtnemo_CNemoSystemOverviewGuiComp_h_included
#define iqtnemo_CNemoSystemOverviewGuiComp_h_included


#include "iqtnemo.h"


#include "QtAcf/QtGuiTemplateBasedComponent.h"
#include "QtAcf/QtAbstractGuiObserverComp.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "QtCompLib/QtIconProviderInterface.h"

#include "generatedfiles/ui_CNemoSystemOverviewGuiComp.h"

#include "inemo/INemoSystemModel.h"
#include "iqtnemo/INemoSensorSelectionListener.h"


namespace iqtnemo
{


class CNemoSystemOverviewGuiComp:	public acf::QtAbstractGuiObserverComp<inemo::INemoSystemModel, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSystemOverviewGuiComp> >,
									public iqtnemo::INemoSensorSelectionListener
{
public:
	typedef acf::QtAbstractGuiObserverComp<inemo::INemoSystemModel, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSystemOverviewGuiComp> > BaseClass;

	CNemoSystemOverviewGuiComp();
	virtual ~CNemoSystemOverviewGuiComp();

	// reimplemented (iqtnemo::INemoSensorSelectionListener)
	virtual void OnSensorSelected(inemo::INemoSensor* selectedSensorPtr);

	// reimplemented (acf::ObserverInterface)
	virtual bool onAttached(acf::ModelInterface* modelPtr);
	virtual void onDetached(acf::ModelInterface* modelPtr);
	virtual void update(acf::ModelInterface* modelPtr, int updateFlags = 0, acf::PolymorphicInterface* updateParamsPtr = NULL);

	// reimplemented (acf::ModelEditorInterface)
	virtual void updateEditor();
	virtual void updateModel();

protected:
	// reimplemented (acf::QtAbstractGuiComponent)
	virtual void initializeGui();
	virtual void destroyGui();

protected:
	acf::ComponentDependency<acf::QtGuiInterface> m_sensorListGuiIfPtr;
	acf::ComponentDependency<acf::QtGuiInterface> m_systemOverviewGuiIfPtr;
	acf::ComponentDependency<acf::QtGuiInterface> m_sensorDataGuiIfPtr;
	acf::ComponentDependency<acf::ObserverInterface> m_sensorListObserverIfPtr;
	acf::MultipleComponentDependency<acf::ObserverInterface> m_sensorObserversIfPtr;

	inemo::INemoSensor* m_selectedSensorPtr;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoSystemOverviewGuiComp_h_included

