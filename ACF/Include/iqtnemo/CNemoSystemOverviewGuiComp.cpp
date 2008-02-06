#include "QtAcf/QtAcf.h"


#include "inemo/INemoSensors.h"

#include "iqtnemo/CNemoSystemOverviewGuiComp.h"


namespace iqtnemo
{


CNemoSystemOverviewGuiComp::CNemoSystemOverviewGuiComp()
	:BaseClass(), 
	m_sensorListGuiIfPtr(this, "SensorListGui"),
	m_systemOverviewGuiIfPtr(this, "SystemOverviewGui"),
	m_sensorDataGuiIfPtr(this, "SensorInfoGui"),
	m_sensorsObserverIfPtr(this, "SensorListObserver")
{

}


CNemoSystemOverviewGuiComp::~CNemoSystemOverviewGuiComp()
{

}


// reimplemented (acf::ObserverInterface)

bool CNemoSystemOverviewGuiComp::onAttached(acf::ModelInterface* modelPtr)
{
	if (BaseClass::onAttached(modelPtr)){
		if (m_sensorsObserverIfPtr.isValid()){
			inemo::INemoSensors* sensorsModelPtr = m_objectPtr->GetNemoSensorsModel();
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
