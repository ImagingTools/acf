#include <assert.h>


#include "inemo/INemoDatabaseAccessor.h"
#include "inemo/CNemoSystemModelComp.h"
#include "inemo/CNemoSensor.h"


namespace inemo
{


CNemoSystemModelComp::CNemoSystemModelComp()
	:BaseClass(),
	BaseClass2(),
	BaseClass3(),
	m_databaseAccessorIfPtr(this, "DatabaseAccessor")
{
	m_sensorFactory.registerFactory(this, "Sensor");

	registerItemFactory(&m_sensorFactory);

	registerInterface<acf::ModelInterface>(this);
}


// reimplemented (inemo::INemoSystemModel)

int CNemoSystemModelComp::GetSensorCount() const
{
	return BaseClass2::itemCount();
}


inemo::INemoSensor& CNemoSystemModelComp::GetSensor(int sensorIndex) const
{
	static inemo::CNemoSensor emptySensor(this);

	assert(sensorIndex >= 0);
	assert(sensorIndex < itemCount());

	if (sensorIndex >= 0 && sensorIndex < itemCount()){
		return *getElement(sensorIndex);
	}

	return emptySensor;
}


// reimplemented (acf::ObserverInterface)

void CNemoSystemModelComp::update(acf::ModelInterface* object)
{

}


// reimplemented (acf::ObjectFactoryInterface)

inemo::INemoSensor* CNemoSystemModelComp::createObject()
{
	return new inemo::CNemoSensor(this);
}


// reimplemented (acf::ComponentInterface)

bool CNemoSystemModelComp::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass3::onInitialize(managerPtr)){
		GetFromDatabase();

		return true;
	}

	return false;
}


// protected methods:

void CNemoSystemModelComp::GetFromDatabase()
{
	BaseClass2::reset();

	if (m_databaseAccessorIfPtr.isValid()){
		int sensorCount = m_databaseAccessorIfPtr->GetSensorCount();
		for (int sensorIndex = 0; sensorIndex < sensorCount; sensorIndex++){
			inemo::INemoSensor* sensorPtr = addElement("Sensor");

			acf::ModelInterface* modelPtr = dynamic_cast<acf::ModelInterface*>(sensorPtr);

			sensorPtr->setName(m_databaseAccessorIfPtr->GetSensorName(sensorIndex));
			
			connectModel(modelPtr);			
		}
	}
}




} // namespace inemo

