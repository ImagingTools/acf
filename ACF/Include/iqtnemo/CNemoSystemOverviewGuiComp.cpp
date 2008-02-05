#include "QtAcf/QtAcf.h"


#include "iqtnemo/CNemoSystemOverviewGuiComp.h"


namespace iqtnemo
{


CNemoSystemOverviewGuiComp::CNemoSystemOverviewGuiComp()
	:BaseClass(), 
	m_sensorListGuiIfPtr(this, "SensorTreeGui"),
	m_systemOverviewGuiIfPtr(this, "SystemOverviewGui"),
	m_sensorDataGuiIfPtr(this, "SensorInfoGui")
{

}


CNemoSystemOverviewGuiComp::~CNemoSystemOverviewGuiComp()
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
