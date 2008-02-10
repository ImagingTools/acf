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

#include "inemo/INemoSystemModel.h"

#include "iqtnemo/INemoSensorSelectionListener.h"

#include "generatedfiles/ui_CNemoSensorTreeGuiComp.h"


namespace iqtnemo
{


class CNemoSensorTreeGuiComp: public acf::QtAbstractGuiObserverComp<inemo::INemoSystemModel, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSensorTreeGuiComp> >
{
	Q_OBJECT

public:
	typedef acf::QtAbstractGuiObserverComp<inemo::INemoSystemModel, acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSensorTreeGuiComp> > BaseClass;

	CNemoSensorTreeGuiComp();

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

private:
	virtual void UpdateView();
	virtual void ResetView();

private slots:
	void OnSelected();

private:
	acf::ComponentDependency<acf::QtIconProviderInterface> m_stateIconsProviderIfPtr;
	acf::MultipleComponentDependency<iqtnemo::INemoSensorSelectionListener> m_selectionListenersIfPtr;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoSensorTreeGuiComp_h_included

