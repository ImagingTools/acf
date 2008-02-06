#include "inemo/INemoSensors.h"
#include "inemo/CNemoSystemModelComp.h"


namespace inemo
{


CNemoSystemModelComp::CNemoSystemModelComp()
	:BaseClass(),
	m_sensorsModelIfPtr(this, "SensorModel")
{
	registerInterface<acf::ModelInterface>(this);
}


// reimplemented (inemo::INemoSystemModel)

inemo::INemoSensors* CNemoSystemModelComp::GetNemoSensorsModel() const
{
	return m_sensorsModelIfPtr.getInterface();
}


// reimplemented (acf::ComponentInterface)

bool CNemoSystemModelComp::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::onInitialize(managerPtr)){
		if (m_sensorsModelIfPtr.isValid()){
			addChildModel(m_sensorsModelIfPtr.getInterface());
		}

		return true;
	}

	return false;
}


} // namespace inemo

