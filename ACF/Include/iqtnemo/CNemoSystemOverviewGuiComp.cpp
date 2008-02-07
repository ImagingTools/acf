#include "QtAcf/QtAcf.h"


#include "inemo/INemoSensors.h"
#include "inemo/INemoSensor.h"

#include "iqtnemo/CNemoSystemOverviewGuiComp.h"


namespace iqtnemo
{


CNemoSystemOverviewGuiComp::CNemoSystemOverviewGuiComp()
	:BaseClass(), 
	m_sensorListGuiIfPtr(this, "SensorListGui"),
	m_systemOverviewGuiIfPtr(this, "SystemOverviewGui"),
	m_sensorDataGuiIfPtr(this, "SensorDataGui"),
	m_sensorsObserverIfPtr(this, "SensorListObserver"),
	m_sensorObserversIfPtr(this, "SensorObservers"),
	m_selectedSensorPtr(NULL)
{
	registerInterface<iqtnemo::INemoSensorSelectionListener>(this);
}


CNemoSystemOverviewGuiComp::~CNemoSystemOverviewGuiComp()
{

}


// reimplemented (iqtnemo::INemoSensorSelectionListener)

void CNemoSystemOverviewGuiComp::OnSensorSelected(inemo::INemoSensor* selectedSensorPtr)
{
	if (m_selectedSensorPtr == selectedSensorPtr){
		return;
	}

	// detach old model from connected observers:
	acf::ModelInterface* selectedModelPtr = dynamic_cast<acf::ModelInterface*>(m_selectedSensorPtr);
	if (selectedModelPtr != NULL){
		for (int observerIndex = 0; observerIndex < m_sensorObserversIfPtr.dependencyCount(); observerIndex++){
			if (m_sensorObserversIfPtr.isValid(observerIndex)){
				acf::ObserverInterface* observerPtr = m_sensorObserversIfPtr.interfacePtr(observerIndex);
				if (observerPtr != NULL && selectedModelPtr->isAttached(*observerPtr)){
					selectedModelPtr->detachObserver(observerPtr);
				}
			}
		}
	}

	// attach new selected model to registered observers:
	selectedModelPtr = dynamic_cast<acf::ModelInterface*>(selectedSensorPtr);
	if (selectedModelPtr != NULL){
		for (int observerIndex = 0; observerIndex < m_sensorObserversIfPtr.dependencyCount(); observerIndex++){
			if (m_sensorObserversIfPtr.isValid(observerIndex)){
				acf::ObserverInterface* observerPtr = m_sensorObserversIfPtr.interfacePtr(observerIndex);
				if (observerPtr != NULL && !selectedModelPtr->isAttached(*observerPtr)){
					selectedModelPtr->attachObserver(observerPtr);

					selectedModelPtr->notifyUpdate();
				}
			}
		}
	}

	m_selectedSensorPtr = selectedSensorPtr;
}


// reimplemented (acf::ObserverInterface)

bool CNemoSystemOverviewGuiComp::onAttached(acf::ModelInterface* modelPtr)
{
	if (BaseClass::onAttached(modelPtr)){
		if (m_sensorsObserverIfPtr.isValid()){
			acf::ModelInterface* sensorsModelPtr = dynamic_cast<acf::ModelInterface*>(m_objectPtr->GetNemoSensorsModel());
			if (sensorsModelPtr != NULL){
				sensorsModelPtr->attachObserver(m_sensorsObserverIfPtr.getInterface());
			}
		}

		return true;
	}

	return false;
}


void CNemoSystemOverviewGuiComp::onDetached(acf::ModelInterface* modelPtr)
{
	BaseClass::onDetached(modelPtr);
}


void CNemoSystemOverviewGuiComp::update(acf::ModelInterface* modelPtr)
{
}


// reimplemented (acf::ModelEditorInterface)

void CNemoSystemOverviewGuiComp::updateEditor()
{
}


void CNemoSystemOverviewGuiComp::updateModel()
{
}


// reimplemented (acf::QtAbstractGuiComponent)

void CNemoSystemOverviewGuiComp::initializeGui()
{
	if (m_widget != NULL){
		if (m_sensorListGuiIfPtr.isValid()){
			m_sensorListGuiIfPtr->attachTo(SensorListFrame);
		}
		
		if (m_systemOverviewGuiIfPtr.isValid()){
			m_systemOverviewGuiIfPtr->attachTo(SystemOverviewFrame);					
		}
		
		if (m_sensorDataGuiIfPtr.isValid()){
			m_sensorDataGuiIfPtr->attachTo(SensorDataFrame);
		}
	}
}


void CNemoSystemOverviewGuiComp::destroyGui()
{
	if (m_sensorListGuiIfPtr.isValid()){
		m_sensorListGuiIfPtr->detachFrom(SensorListFrame);
	}

	if (m_systemOverviewGuiIfPtr.isValid()){
		m_systemOverviewGuiIfPtr->detachFrom(SystemOverviewFrame);					
	}

	if (m_sensorDataGuiIfPtr.isValid()){
		m_sensorDataGuiIfPtr->detachFrom(SensorDataFrame);
	}
}


} // namespace iqtnemo
